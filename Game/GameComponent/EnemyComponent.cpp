#include "EnemyComponent.h"
#include "Engine.h"
#include "PlayerComponent.h"

using namespace nc;



EnemyComponent::~EnemyComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
}

void EnemyComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter",
		std::bind(&EnemyComponent::onCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<AudioSystem>()->AddAudio("splat", "splat.mp3");
}

void EnemyComponent::Update()
{
	Actor* player = owner->scene->FindActor("Player");
	if (player)
	{
		Vector2 direction = player->transform.position - owner->transform.position;
		Vector2 force = direction.Normalized() * speed;

		PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
		assert(physicsComponent);

		physicsComponent->ApplyForce(force);
	}

}

bool EnemyComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);
	return true;
}

bool EnemyComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

void EnemyComponent::onCollisionEnter(const nc::Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);
	if (istring_compare(actor->tag, "player"))
	{
		PlayerComponent* playerComponent = actor->GetComponent<PlayerComponent>();
		if (playerComponent)
		{
			if (playerComponent->isAttacking())
			{
				owner->destroy = true;
				owner->scene->engine->Get<AudioSystem>()->PlayAudio("splat");
			}
		}
	}
}
