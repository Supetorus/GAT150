#pragma once
#include "Object/Actor.h"
#include "Math/Vector2.h"
#include "Math/Random.h"

class Asteroid : public nc::Actor
{
public:
	Asteroid(const nc::Transform& transform, std::shared_ptr<nc::Texture> texture, float maxSpeed) : nc::Actor{ transform, texture }, speed{ speed } {}
	void Intitialize() override;
	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	float speed = 0;
	float maxSpeed = 75;
	float rotationSpeed = 3 * nc::Random();
	nc::Vector2 velocity;
};