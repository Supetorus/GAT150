#pragma once

// Systems
#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"

// Core
#include "Core/FileSystem.h"

// Math
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Math/Random.h"
#include "Math/MathUtils.h"
#include "Math/Transform.h"

//Graphics
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"

//Resource
#include "Resource/ResourceSystem.h"

// Framework
#include "Framework/EventSystem.h"

// objects
#include "Object/Actor.h"

#include <vector>
#include <memory>
#include <algorithm>

namespace nc
{
	class Engine
	{
	public:
		void Startup();
		void Shutdown();

		void Update(float dt);
		void Draw();

		template<typename T>
		T* Get();

	private:
		std::vector<std::unique_ptr<System>> systems;

	};
	template<typename T>
	inline T* Engine::Get()
	{
		for (auto& system : systems)
		{
			if (dynamic_cast<T*>(system.get())) return dynamic_cast<T*>(system.get());// checks if dynamic cast returns something or null
		}
		return nullptr;
	}
}