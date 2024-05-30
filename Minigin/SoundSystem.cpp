#include "SoundSystem.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <condition_variable>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

namespace dae
{
    class SoundSystem::SoundSystemImpl
    {
    public:
        SoundSystemImpl()
        {
            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
            {
                std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << '\n';
                return;
            }

            m_SoundThread = std::thread(&SoundSystemImpl::SoundThreadFunction, this);
        }

        ~SoundSystemImpl()
        {
            StopSoundThread();
            for (const auto& pair : m_Sound)
            {
                Mix_FreeChunk(pair.second);
            }
            m_Sound.clear();
            Mix_CloseAudio();
        }

        void playSound(const sound_id id, const int volume)
        {
            if (m_Muted) return;

            std::lock_guard<std::mutex> lock(m_QueueMutex);
            m_PlayQueue.push({ id, volume, 0, -1 });
            m_QueueCondition.notify_one();
        }

        void playMusic(const sound_id id, const int volume)
        {
            if (m_Muted) return;

            std::lock_guard<std::mutex> lock(m_QueueMutex);
            m_PlayQueue.push({ id, volume, -1, 1 });
            m_QueueCondition.notify_one();
        }

        void Load(const sound_id id, const std::string& path)
        {
            auto datapath = ResourceManager::GetInstance().ReturnDataPath() + "Audio/" + path;
            Mix_Chunk* chunk = Mix_LoadWAV(datapath.c_str());
            if (chunk)
            {
                m_Sound[id] = chunk;
            }
            else
            {
                std::cerr << "Failed to load sound: " << path << " SDL_mixer Error: " << Mix_GetError() << '\n';
            }
        }

        void MuteUnmute()
        {
            m_Muted = !m_Muted;
            if (m_Muted)
            {
                Mix_Volume(-1, 0);
                Mix_Pause(-1);
            }
            else
            {
                Mix_Resume(-1);
                Mix_Volume(-1, MIX_MAX_VOLUME);
            }
        }

    private:
        struct SoundCommand
        {
            sound_id id;
            int volume;
            int loops;
            int channel;
        };

        void SoundThreadFunction()
        {
            while (true)
            {
                std::unique_lock<std::mutex> lock(m_QueueMutex);
                m_QueueCondition.wait(lock, [this] { return !m_PlayQueue.empty() || m_QuitFlag; });

                if (m_QuitFlag) break;

                SoundCommand command = m_PlayQueue.front();
                m_PlayQueue.pop();
                lock.unlock();

                if (m_Sound.find(command.id) != m_Sound.end())
                {
                    Mix_Chunk* chunk = m_Sound[command.id];
                    int channel = Mix_PlayChannel(command.channel, chunk, command.loops);
                    Mix_Volume(channel, command.volume);
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        void StopSoundThread()
        {
            {
                std::lock_guard<std::mutex> lock(m_QueueMutex);
                m_QuitFlag = true;
            }
            m_QueueCondition.notify_one();
            if (m_SoundThread.joinable())
            {
                m_SoundThread.join();
            }
        }

        std::map<sound_id, Mix_Chunk*> m_Sound;
        bool m_Muted = false;
        bool m_QuitFlag = false;

        std::thread m_SoundThread;
        std::queue<SoundCommand> m_PlayQueue;
        std::mutex m_QueueMutex;
        std::condition_variable m_QueueCondition;
    };



	dae::SoundSystem::SoundSystem()
		:m_pImpl(std::make_unique<SoundSystemImpl>())
	{
		
	}

	SoundSystem::~SoundSystem()
	{
		m_pImpl.reset();
	}

	void dae::SoundSystem::playSound(const dae::sound_id id, const int volume)
	{
		m_pImpl->playSound(id, volume);
	}

	void dae::SoundSystem::playMusic(const sound_id id, const int volume)
	{
		m_pImpl->playMusic(id, volume);
	}

	void dae::SoundSystem::Load(const dae::sound_id id, const std::string& path)
	{
		m_pImpl->Load(id, path);
	}

	void dae::SoundSystem::MuteUnmute()
	{
		m_pImpl->MuteUnmute();
	}

}