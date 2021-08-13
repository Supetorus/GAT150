#pragma once
#include "Object/Actor.h"
#include "Math/Random.h"

class Player : public nc::Actor
{
public:
	Player(const nc::Transform& transform, std::shared_ptr<nc::Texture> texture) : nc::Actor{ transform, texture} {}
	void Intitialize() override;
	void Update(float dt) override;
	void OnCollision(Actor* actor) override;
	void shoot(const nc::Transform& transform);
	void shootRocket(const nc::Transform& transform);
	
public:
	int health = 0;
	int startHealth = 100;
	float rocketFireTimer{ 0 };
	
private:
	float bulletFireTimer{ 0 };
	float bulletFireRate{ 0.1f }; // time between shots
	float rocketFireRate{ 5.0f }; // time between shots
	float acceleration = 5;
	float friction = 0.98f;
	float rotationSpeed = 7;
	float thrust = 0;
	float maxSpeed = 400;
	nc::Vector2 velocity{ 0,0 };
};