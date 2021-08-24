#pragma once
#include "Object.h"
#include "Math/Transform.h"
#include "Scene.h"
#include "Component/Component.h"
#include <memory>
#include <vector>

namespace nc
{
	class Scene;
	class Renderer;

	class Actor : public Object, ISerializable
	{
	public:
		Actor() {}
		Actor(const Transform& transform) : transform{ transform } {}

		virtual void Update(float dt);
		virtual void Draw(Renderer* renderer);
		virtual void Intitialize();

		virtual void OnCollision(Actor* actor) {}
		void AddChild(std::unique_ptr<Actor> actor);

		float GetRadius();

		bool hasTag(std::string checkTag);
		void addTag(std::string tag);

		void AddComponent(std::unique_ptr<Component> component);
		template<class T>
		T* AddComponent();

		// Inherited via ISerializable
		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		bool destroy{ false };
		std::vector<std::string> tags;

		std::string tag; // temporary remove later

		Transform transform;
		Scene* scene{ nullptr };

		Actor* parent{ nullptr };
		std::vector<std::unique_ptr<Actor>> children;
		unsigned int id = 0;

		std::vector<std::unique_ptr<Component>> components;
	};
	
	template<class T>
	inline T* Actor::AddComponent()
	{
		std::unique_ptr<T> component = std::make_unique<T>();
		component->owner = this;

		components.push_back(move(component));

		return dynamic_cast<T*>(components.back().get());
	}
}