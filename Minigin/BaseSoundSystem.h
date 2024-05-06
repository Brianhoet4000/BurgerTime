#pragma once
#include <string>

namespace dae
{

	using sound_id = unsigned short;

	class BaseSoundSystem
	{
	public:
		BaseSoundSystem() = default;
		virtual ~BaseSoundSystem() = default;

		BaseSoundSystem(const BaseSoundSystem& other) = delete;
		BaseSoundSystem(BaseSoundSystem&& other) = delete;
		BaseSoundSystem& operator=(const BaseSoundSystem& other) = delete;
		BaseSoundSystem& operator=(BaseSoundSystem&& other) = delete;

		virtual void playSound(const sound_id id, const int volume) = 0;
		virtual void playMusic(const sound_id id, const int volume) = 0;
		virtual void Load(const sound_id id, const std::string& path) = 0;
		virtual void MuteUnmute() = 0;
	};

}

