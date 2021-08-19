#pragma once

// Systems
#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"

// Core
#include "Core/Utilities.h"
#include "Core/FileSystem.h"
#include "Core/Timer.h"

// Framework
#include "Framework/EventSystem.h"
#include "Framework/Singleton.h"
#include "Framework/Factory.h"

// Math
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Math/Random.h"
#include "Math/MathUtils.h"
#include "Math/Transform.h"

//Graphics
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Graphics/ParticleSystem.h"
#include "Graphics/Font.h"

//Resource
#include "Resource/ResourceSystem.h"

// Objects
#include "Object/Actor.h"

// Components
#include "Component/SpriteAnimationComponent.h"
#include "Component/PhysicsComponent.h"

#include <vector>
#include <memory>
#include <algorithm>

namespace nc
{
	using ObjectFactory = Singleton<Factory<std::string, Object>>;

	class Engine
	{
	public:
		void Startup();
		void Shutdown();

		void Update();
		void Draw(Renderer* renderer);

		template<typename T>
		T* Get();

	public:
		FrameTimer time;

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