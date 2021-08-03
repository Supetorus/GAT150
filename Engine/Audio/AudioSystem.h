#pragma once

#include "Framework/System.h"
#include <string>
#include <map>

namespace nc
{
	class AudioSystem : public System
	{
	public:
		void Startup();
		void Shutdown();

		void Update(float dt);

		void AddAudio(const std::string& name, const std::string& filename);
		void PlayAudio(const std::string& name);
		void PlayAudio(const std::string& name, bool loop);
	};
}

extern nc::AudioSystem audioSystem;