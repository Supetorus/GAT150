#include "Enemy.h"
#include "Projectile.h"
#include "Player.h"
#include "Math/MathUtils.h"


void Enemy::Intitialize()
{
	addTag("enemy");
	health = startHealth;

	// shooting children[0]
	std::unique_ptr locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 2, 0 };
	AddChild(std::move(locator));

	// rocket particles children[1]
	locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ -1, 0 };
	AddChild(std::move(locator));
}

void Enemy::Update(float dt)
{
	Actor::Update(dt);

	if (scene->GetActor<Player>())
	{
		// Turns the enemy ship towards the player
		nc::Vector2 direction = scene->GetActor<Player>()->transform.position - transform.position; // the direction of the player from the enemy
		nc::Vector2 forward = nc::Vector2::Rotate(nc::Vector2::right, transform.rotation); // the direction the enemy is currently facing
		float turnAngle = nc::Vector2::SignedAngle(forward, direction.Normalized());
		transform.rotation = transform.rotation + turnAngle * (3 * dt); // turns the enemy ship toward the player

		float angle = nc::Vector2::Angle(forward, direction.Normalized());

		//shooting
		fireTimer -= dt;
		if (angle <= nc::DegToRad(10))
		{
			if (fireTimer <= 0) // only fires if the enemy ship is facing the player
			{
				fireTimer = fireRate;
				//std::vector<nc::Vector2> points = { {-5,-5}, {5, -5}, {0, 10}, {-5,-5} };
				std::shared_ptr<nc::Texture> enemyBulletTexture = scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Images/enemy_bullet.png", scene->engine->Get<nc::Renderer>());

				nc::Transform t = children[0]->transform;
				t.scale = 0.2f;
				std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, enemyBulletTexture, 200.0f, 10);
				projectile->addTag( "id:" + std::to_string(id));
				projectile->addTag("enemy");
				projectile->addTag("bullet");
				scene->AddActor(std::move(projectile));
				scene->engine->Get<nc::AudioSystem>()->PlayAudio("enemy_fire");
			}
			
		}
		if (angle <= nc::DegToRad(45))
		{
			speed = speed + acceleration < maxSpeed ? speed + acceleration : maxSpeed;
		}
		else speed = speed - acceleration > maxSpeed ? speed - acceleration : 0;


		transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * speed * dt;
		transform.position.x = nc::Wrap(transform.position.x, 0.0f, scene->engine->Get<nc::Renderer>()->GetWidth() * 1.0f);
		transform.position.y = nc::Wrap(transform.position.y, 0.0f, scene->engine->Get<nc::Renderer>()->GetWidth() * 1.0f);
	}

	// death
	if (health <= 0)
	{
		destroy = true;
		scene->RemoveByTag("id:" + std::to_string(id));
		scene->engine->Get<nc::ParticleSystem>()->CreateBurst(transform.position, 10, 0.5f, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Images/flame_blade.png", scene->engine->Get<nc::Renderer>()));
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("destroy_1");

		nc::Event event;
		event.name = "AddPoints";
		event.data = 1;
		scene->engine->Get<nc::EventSystem>()->Notify(event);
	}

	// rocket particles
	scene->engine->Get<nc::ParticleSystem>()->Create(children[1]->transform.position, 10, 0.5f,
		scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Images/particle_2.png", scene->engine->Get<nc::Renderer>()),
		speed, speed * 0.25f, transform.rotation + nc::Pi, 0.0f, 0.4f);
}

void Enemy::OnCollision(Actor* actor)
{
	if (actor->hasTag("player") && dynamic_cast<Projectile*>(actor))
	{
		health -= dynamic_cast<Projectile*>(actor)->damage;
	}
}
