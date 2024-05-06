#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <SDL_mixer.h>
#include "BaseSoundSystem.h"
#include "ResourceManager.h"

namespace dae
{

	class SoundSystem final : public BaseSoundSystem
	{
	public:

		SoundSystem();
		virtual ~SoundSystem() override;

		SoundSystem(const SoundSystem& other) = delete;
		SoundSystem(SoundSystem&& other) = delete;
		SoundSystem& operator=(const SoundSystem& other) = delete;
		SoundSystem& operator=(SoundSystem&& other) = delete;

		void playSound(const sound_id id, const int volume) override;
		void playMusic(const sound_id id, const int volume) override;
		void Load(const sound_id id, const std::string& path) override;
		void MuteUnmute() override;

	private:
		class SoundSystemImpl;
		std::unique_ptr<SoundSystemImpl> m_pImpl;
	};

	class LoggingSoundSystem final : public BaseSoundSystem
	{
		std::unique_ptr<SoundSystem> m_pSs;
	public:
		LoggingSoundSystem(std::unique_ptr<SoundSystem>&& ss) : m_pSs(std::move(ss)) {}
		virtual ~LoggingSoundSystem() override = default;

		LoggingSoundSystem(const LoggingSoundSystem& other) = delete;
		LoggingSoundSystem(LoggingSoundSystem&& other) = delete;
		LoggingSoundSystem& operator=(const LoggingSoundSystem& other) = delete;
		LoggingSoundSystem& operator=(LoggingSoundSystem&& other) = delete;

		void playSound(const sound_id id, const int volume) override
		{
			m_pSs->playSound(id, volume);
			std::cout << "Playing SoundId: " << id << " at volume: " << volume << "\n";
		}

		void playMusic(const sound_id id, const int volume) override
		{
			m_pSs->playMusic(id, volume);
			std::cout << "Playing MusicId: " << id << " at volume: " << volume << "\n";
		}

		void Load(const sound_id id, const std::string& path) override
		{
			m_pSs->Load(id, path);
			std::cout << "loading songId: " << id << " with path: " << path << "\n";
		}

		void MuteUnmute() override
		{
			m_pSs->MuteUnmute();
			std::cout << "pausing/resuming sounds\n";
		}
	};

	class NullSoundSystem final : public BaseSoundSystem
	{
		void playSound(const sound_id, const int) override {}
		void playMusic(const sound_id, const int) override {}
		void Load(const sound_id, const std::string&) override {}
		void MuteUnmute() override {}
	};
}