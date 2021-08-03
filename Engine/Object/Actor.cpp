#include "Actor.h"
#include <algorithm>

namespace nc
{
	void Actor::Update(float dt)
	{
		transform.Update();
		std::for_each(children.begin(), children.end(), [](auto& child) { child->transform.Update(child->parent->transform.matrix); });
	}

	void Actor::Draw()
	{}

	void Actor::Intitialize()
	{
	}

	void Actor::AddChild(std::unique_ptr<Actor> actor)
	{
		actor->parent = this;
		children.push_back(std::move(actor));
	}

	float Actor::GetRadius()
	{
		return 0;
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
}