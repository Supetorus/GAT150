#include "Actor.h"
#include "Engine.h"
#include "Graphics/Renderer.h"
#include "Component/GraphicsComponent.h"
#include <algorithm>

namespace nc
{
	void Actor::Update(float dt)
	{
		std::for_each(components.begin(), components.end(), [](auto& component) { component->Update(); });

		transform.Update();
		std::for_each(children.begin(), children.end(), [](auto& child) { child->transform.Update(child->parent->transform.matrix); });
	}

	void Actor::Draw(Renderer* renderer)
	{
		std::for_each(components.begin(), components.end(), [renderer](auto& component)
			{
				if (dynamic_cast<GraphicsComponent*>(component.get()))
				{
					dynamic_cast<GraphicsComponent*>(component.get())->Draw(renderer);
				}
			});
		std::for_each(children.begin(), children.end(), [renderer](auto& child) { child->Draw(renderer); });
	}

	void Actor::Intitialize()
	{
	}

	void Actor::BeginContact(Actor* other)
	{
		Event event;
		
		event.name = "collision_enter";
		event.data = other;
		event.receiver = this;

		if (!destroy) scene->engine->Get<EventSystem>()->Notify(event);
	}

	void Actor::EndContact(Actor* other)
	{
		Event event;

		event.name = "collision_exit";
		event.data = other;
		event.receiver = this;

		if (!destroy) scene->engine->Get<EventSystem>()->Notify(event);
	}

	void Actor::AddChild(std::unique_ptr<Actor> actor)
	{
		actor->parent = this;
		children.push_back(std::move(actor));
	}

	bool Actor::hasTag(std::string checkTag)
	{
		for (std::string tag : tags)
		{
			if (tag == checkTag) return true;
		}
		return false;
	}

	void Actor::addTag(std::string tag)
	{
		tags.push_back(tag);
	}

	void Actor::AddComponent(std::unique_ptr<Component> component)
	{
		component->owner = this;
		components.push_back(std::move(component));
	}

	bool Actor::Write(const rapidjson::Value& value) const
	{
		return false;
	}

	bool Actor::Read(const rapidjson::Value& value)
	{
		JSON_READ(value, tag);
		JSON_READ(value, name);
		if (value.HasMember("transform"))
		{
			transform.Read(value["transform"]);
		}

		if (value.HasMember("components") && value["components"].IsArray())
		{
			for (auto& componentValue : value["components"].GetArray())
			{
				std::string type;
				JSON_READ(componentValue, type);

				auto component = ObjectFactory::Instance().Create<Component>(type);
				if (component)
				{
					component->owner = this;
					component->Read(componentValue);
					component->Create();
					AddComponent(std::move(component));
				}
			}
		}

		return true;
	}
}