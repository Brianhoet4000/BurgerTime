#pragma once
#include <algorithm>
#include <memory>
#include "BaseSoundSystem.h"

namespace dae
{

	class servicelocator final
	{
	public:
		static BaseSoundSystem& get_sound_system() { return *_ss_instance; }
		static void register_sound_system(std::unique_ptr<BaseSoundSystem>&& ss) { _ss_instance = std::move(ss); }
	private:
		inline static std::unique_ptr<BaseSoundSystem> _ss_instance;
	};
}
