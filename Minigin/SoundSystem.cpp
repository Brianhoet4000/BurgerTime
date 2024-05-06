#pragma once
#include "SoundSystem.h"

#include <condition_variable>
#include <iostream>
#include <queue>
#include <thread>

#include "SDL.h"

namespace dae
{
	class SoundSystem::SoundSystemImpl
	{
	public:
		SoundSystemImpl()
		{
			Mix_Init(MIX_INIT_MP3);
			Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

			m_SoundThread = std::thread(&SoundSystemImpl::SoundThreadFunction, this);
		}

		~SoundSystemImpl()
		{
			StopSoundThread();

			// Clean up SDL_mixer resources and unload chunks
			for (const auto& pair : m_Sound)
			{
				Mix_FreeChunk(pair.second);
			}
			m_Sound.clear();

			Mix_CloseAudio();
			Mix_Quit();
		}

		SoundSystemImpl(const SoundSystemImpl& other) = delete;
		SoundSystemImpl(SoundSystemImpl&& other) = delete;
		SoundSystemImpl& operator=(const SoundSystemImpl& other) = delete;
		SoundSystemImpl& operator=(SoundSystemImpl&& other) = delete;

		void playSound(const sound_id id, const int volume)
		{
			if (m_Muted) return;

			auto playchunk = m_Sound.at(id);

			if (playchunk == nullptr)
				return;

			auto channel = Mix_PlayChannel(-1, playchunk, 0);
			Mix_Volume(channel, volume);
		}
		void playMusic(const sound_id id, const int volume)
		{
			auto playchunk = m_Sound.at(id);

			if (playchunk == nullptr)
				return;

			auto channel = Mix_PlayChannel(1, playchunk, -1);
			Mix_Volume(channel, volume);
		}
		void Load(const sound_id id, const std::string& path)
		{
			auto datapath = ResourceManager::GetInstance().ReturnDataPath() + "Audio/";
			auto fullpath = datapath + path;
			auto chunk = Mix_LoadWAV(fullpath.c_str());
			m_Sound.emplace(id, chunk);
		}
		void MuteUnmute()
		{
			m_Muted = !m_Muted;
			if (m_Muted)
			{
				//Mix_Volume(-1, 0);
				Mix_Pause(-1);
				std::cout << "Paused\n";
			}
			else
			{
				//Mix_Volume(-1, MIX_MAX_VOLUME);
				Mix_Resume(-1);
				std::cout << "Resumed\n";
			}
		}
		void SoundThreadFunction()
		{
			while (true)
			{
				std::unique_lock<std::mutex> lock(m_QueueMutex);
				m_QueueCondition.wait(lock, [this] { return !m_PlayQueue.empty() || m_QuitFlag; });

				if (m_QuitFlag)
				{
					break;
				}

				sound_id id = m_PlayQueue.front();
				m_PlayQueue.pop();

				lock.unlock(); // Release the lock before playing the sound

				// Play sound logic
				auto playchunk = m_Sound.at(id);

				if (playchunk != nullptr)
				{
					auto channel = Mix_PlayChannel(-1, playchunk, 0);
					Mix_Volume(channel, MIX_MAX_VOLUME); // Adjust volume as needed
				}

				// Sleep for a short duration to avoid busy-waiting
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

	private:
		enum SoundState
		{
			SoundPlay,
			SoundLoaded
		};

		std::map<sound_id, Mix_Chunk*> m_Sound{};
		bool m_Muted = false;

		std::thread m_SoundThread;
		bool m_QuitFlag = false;
		std::queue<sound_id> m_PlayQueue;
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