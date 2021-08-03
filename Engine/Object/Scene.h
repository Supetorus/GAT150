#pragma once
#include "Object.h"
#include "../Math/Vector2.h"
#include <list>
#include <memory>
#include <vector>
#include <string>

namespace nc
{
	class Engine;

	class Actor;

	class Scene
	{
	public:
		void Update(float dt);
		void Draw();

		void AddActor(std::unique_ptr <Actor> actor);
		void RemoveActor(Actor* actor);
		void RemoveAllActors();
		void RemoveByTag(const std::string& tag);
		Vector2 SafeLocation(float radius, float buffer);

		template<typename T>
		T* GetActor();

		template<typename T>
		std::vector<T*> GetActors();

		int ActorCount();

	public:
		Engine* engine{ nullptr };
		unsigned int id = 0;

	private:
		std::vector<std::unique_ptr<Actor>> actors;
		std::vector<std::unique_ptr<Actor>> newActors;
	};

	template<typename T>
	inline T* Scene::GetActor()
	{
		for (auto& actor : actors)
		{
			if (dynamic_cast<T*>(actor.get())) return dynamic_cast<T*>(actor.get());
		}
		return nullptr;
	}

	template<typename T>
	inline std::vector<T*> Scene::GetActors()
	{
		std::vector<T*> result;

		for (auto& actor : actors)
		{
			if (dynamic_cast<T*>(actor.get())) result.push_back(dynamic_cast<T*>(actor.get()));
		}
		return result;
	}
}