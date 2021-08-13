#pragma once
#include "Object/Actor.h"
#include "Engine.h"

class Projectile : public nc::Actor
{
public:
	Projectile(const nc::Transform& transform, std::shared_ptr<nc::Texture> texture, float speed, int damage) : 
		nc::Actor{ transform, texture }, speed{ speed }, damage{ damage } { addTag("bullet"); }
	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	void SelfDestruct(bool impact);

public:
	int damage = 0;
	float lifetime{ 4 };

private:
	float speed = 300;
	float acceleration = 20;
};