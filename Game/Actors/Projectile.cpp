#include "Projectile.h"
#include "Math/MathUtils.h"

void Projectile::Update(float dt)
{
	Actor::Update(dt);

	lifetime -= dt;
	if (lifetime <= 0) SelfDestruct(false);

	transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * speed * dt;
	/*if (transform.position.x < nc::WindowInfo::minWidth || transform.position.x > nc::WindowInfo::maxWidth ||
		transform.position.y < nc::WindowInfo::minHeight || transform.position.y > nc::WindowInfo::maxHeight)
	{
		destroy = true;
	}*/

	//scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 10, 0.5f, colors[nc::RandomRangeInt(0, colors.size())], 150);
	if (hasTag("rocket")) scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 5, 0.2f,
		scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Images/particle_1.png", scene->engine->Get<nc::Renderer>()),
		0, speed, transform.rotation + nc::Pi, 0.0f, 0.4f);
}

void Projectile::OnCollision(Actor* actor)
{
	if (hasTag("enemy") && actor->hasTag("player") ||
		hasTag("player") && actor->hasTag("enemy") ||
		actor->hasTag("asteroid"))
	{
		SelfDestruct(true);
	}
	
}

void Projectile::SelfDestruct(bool impact)
{
	if (hasTag("rocket"))
	{
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion_1");
		for (size_t i = 0; i < 10; i++)
		{
			//scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 30, 0.5f, shape->color, 50, 70, 0.0f, 0.0f, nc::TwoPi);

			// create the projectile
			nc::Transform projectileTransform = nc::Transform{ transform.position, nc::Random() * nc::TwoPi, 0.15f };
			std::shared_ptr<nc::Texture> projectileShape = scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Images/rocket.png", scene->engine->Get<nc::Renderer>());
			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(projectileTransform, projectileShape, 100.0f, 20);
			projectile->addTag("player");
			projectile->addTag("mini_rocket");
			projectile->lifetime = 1.0f;

			// add projectile to the scene
			scene->AddActor(std::move(projectile));
		}
	}
	else if (impact)
	{
		//scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 8, 0.5f, shape->color, 50, 70, 0.0f, 0.0f, nc::TwoPi);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");
	}
	destroy = true;
}
