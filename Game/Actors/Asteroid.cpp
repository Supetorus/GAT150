#include "Asteroid.h"
#include "Projectile.h"
#include "Player.h"
#include "Math/MathUtils.h"
#include "Math/Random.h"


void Asteroid::Intitialize()
{
	addTag("asteroid");
	speed = nc::RandomRange(0, maxSpeed);
	velocity = { nc::RandomRange(-1, 1), nc::RandomRange(-1, 1) };
	velocity *= speed;
}

void Asteroid::Update(float dt)
{
	Actor::Update(dt);

	if (scene->GetActor<Player>())
	{
		transform.position += velocity * dt;
		transform.rotation += rotationSpeed * dt;
		transform.position.x = nc::Wrap(transform.position.x, 0.0f, scene->engine->Get<nc::Renderer>()->GetWidth() * 1.0f);
		transform.position.y = nc::Wrap(transform.position.y, 0.0f, scene->engine->Get<nc::Renderer>()->GetHeight() * 1.0f);
	}

	//scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 30, 0.5f, nc::Color::yellow, thrust, thrust * 0.25f, transform.rotation + nc::Pi, 0.0f, 0.4f);
}

void Asteroid::OnCollision(Actor* actor)
{
}
