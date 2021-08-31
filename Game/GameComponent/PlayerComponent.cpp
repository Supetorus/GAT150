#include "PlayerComponent.h"
#include "Engine.h"

using namespace nc;


PlayerComponent::~PlayerComponent()
{
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_enter", owner);
	owner->scene->engine->Get<EventSystem>()->Unsubscribe("collision_exit", owner);
}

void PlayerComponent::Create()
{
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_enter",
		std::bind(&PlayerComponent::onCollisionEnter, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<EventSystem>()->Subscribe("collision_exit",
		std::bind(&PlayerComponent::onCollisionExit, this, std::placeholders::_1), owner);
	owner->scene->engine->Get<AudioSystem>()->AddAudio("hurt", "hurt.wav");
}

void PlayerComponent::Update()
{
	Vector2 force = Vector2::zero;
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_A) == InputSystem::eKeyState::Held)
	{
		force.x -= speed;
	}
	if (owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_D) == InputSystem::eKeyState::Held)
	{
		force.x += speed;
	}
	if (contacts.size() > 0 && owner->scene->engine->Get<InputSystem>()->GetKeyState(SDL_SCANCODE_W) == InputSystem::eKeyState::Pressed)
	{
		force.y -= 200;
	}

	PhysicsComponent* physicsComponent = owner->GetComponent<PhysicsComponent>();
	assert(physicsComponent);

	physicsComponent->ApplyForce(force);

	SpriteAnimationComponent* spriteAnimationComponent = owner->GetComponent<SpriteAnimationComponent>();
	assert(spriteAnimationComponent);
	//if (force.x > 0) spriteAnimationComponent->StartSequence("walk_right");
	//else if (force.x < 0) spriteAnimationComponent->StartSequence("walk_left");
	//else spriteAnimationComponent->StartSequence("idle");

	if (owner->GetComponent<PhysicsComponent>()->velocity.x > 0.1) spriteAnimationComponent->StartSequence("walk_right");
	else if (owner->GetComponent<PhysicsComponent>()->velocity.x < -0.1) spriteAnimationComponent->StartSequence("walk_left");
	else spriteAnimationComponent->StartSequence("idle");
}

bool PlayerComponent::Read(const rapidjson::Value& value)
{
	JSON_READ(value, speed);
	return true;
}

bool PlayerComponent::Write(const rapidjson::Value& value) const
{
	return false;
}

void PlayerComponent::onCollisionEnter(const Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);
	if (istring_compare(actor->tag, "ground"))
	{
		contacts.push_back(actor);
	}

	if (istring_compare(actor->tag, "enemy"))
	{
		owner->scene->engine->Get<AudioSystem>()->PlayAudio("hurt");
	}
}

void PlayerComponent::onCollisionExit(const Event& event)
{
	void* p = std::get<void*>(event.data);
	Actor* actor = reinterpret_cast<Actor*>(p);
	if (istring_compare(actor->tag, "ground"))
	{
		contacts.remove(actor);
	}
}