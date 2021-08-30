#include "PickupComponent.h"
#include "Engine.h"

using namespace nc;


void PickupComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter",
		std::bind(&PickupComponent::onCollisionEnter, this, std::placeholders::_1), owner);
	//owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit",
	//	std::bind(&PickupComponent::onCollisionEnter, this, std::placeholders::_1), owner);
}

void PickupComponent::Update()
{
}

bool PickupComponent::Read(const rapidjson::Value& value)
{
	return false;
}

bool PickupComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

void PickupComponent::onCollisionEnter(const Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);
	if (istring_compare(actor->tag, "player"))
	{
		owner->GetComponent<AudioComponent>()->Play();
		owner->destroy = true;
	}
}

void PickupComponent::onCollisionExit(const nc::Event& event)
{
}
