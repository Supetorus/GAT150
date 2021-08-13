#pragma once
#include "Object/Actor.h"

class Enemy : public nc::Actor
{
public:
	Enemy(const nc::Transform& transform, std::shared_ptr<nc::Texture> texture, float speed) : nc::Actor{ transform, texture }, speed{ speed } {}
	void Intitialize() override;
	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	float fireTimer{ 0 };
	float fireRate{ 2.0f }; // time between shots
	float acceleration = 25;
	float speed = 0;
	float maxSpeed = 75;
	float rotationSpeed = 7;
	int health = 0;
	int startHealth = 30;
};