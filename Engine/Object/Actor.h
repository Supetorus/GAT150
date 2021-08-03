#pragma once
#include "Object.h"
#include "Math/Transform.h"
#include "Scene.h"
#include <memory>
#include <vector>

namespace nc
{
	//class Scene;

	class Actor : public Object
	{
	public:
		Actor() {}
		Actor(const Transform& transform) : transform{ transform } {}

		virtual void Update(float dt);
		virtual void Draw();
		virtual void Intitialize();

		virtual void OnCollision(Actor* actor) {}
		void AddChild(std::unique_ptr<Actor> actor);

		float GetRadius();

		bool hasTag(std::string checkTag);
		void addTag(std::string tag);

	public:
		bool destroy{ false };
		std::vector<std::string> tags;

		Transform transform;
		Scene* scene{ nullptr };

		Actor* parent{ nullptr };
		std::vector<std::unique_ptr<Actor>> children;
		unsigned int id = 0;
	};
}