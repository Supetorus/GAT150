#include "Player.h"
#include "Enemy.h"
#include "Math/MathUtils.h"
#include "Projectile.h"
#include <memory>
#include <cmath>

void Player::Intitialize()
{
	addTag("player");
	health = startHealth;

	// turret children[0]
	std::unique_ptr turret = std::make_unique<Actor>(nc::Transform{}, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Images/turret_barrel.png", scene->engine->Get<nc::Renderer>()));
	turret->transform.localPosition = nc::Vector2{ 0, 0 };
	AddChild(std::move(turret));

	// rocket particles children[1]
	std::unique_ptr locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ -5, 0 };
	AddChild(std::move(locator));

	/*locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 0, 5 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 0, -5 };
	AddChild(std::move(locator));*/

}

void Player::Update(float dt)
{
	Actor::Update(dt);

	// Update the mouse position
	nc::Vector2 mouseP = scene->engine->Get<nc::InputSystem>()->GetMousePosition();
	
	// Rotates the barrel to the direction of the mouse
	nc::Vector2 direction = mouseP - transform.position;
	children[0]->transform.localRotation = direction.Angle() - transform.rotation;
	
	// Control inputs
	float turnSpeed = 0;
	if ((scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == nc::InputSystem::eKeyState::Held)) turnSpeed -= rotationSpeed * dt;
	if ((scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == nc::InputSystem::eKeyState::Held)) turnSpeed += rotationSpeed * dt;
	transform.rotation += turnSpeed;
	if ((scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == nc::InputSystem::eKeyState::Held)) thrust = thrust + acceleration < maxSpeed ? thrust + acceleration : maxSpeed;
	else thrust = thrust - acceleration > 0 ? thrust - acceleration : 0;

	velocity = velocity + nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * thrust;
	velocity.CircleClamp(300);
	velocity *= friction;
	
	//std::cout << velocity.x << " " << velocity.y << std::endl;
	nc::Vector2 translation = nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * thrust;
	transform.position += velocity * dt;
	transform.position.x = nc::Wrap(transform.position.x, 0.0f, static_cast <float>(scene->engine->Get<nc::Renderer>()->GetWidth()));
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, static_cast<float>(scene->engine->Get<nc::Renderer>()->GetHeight()));

	// shoot bullets
	bulletFireTimer -= dt;
	if (bulletFireTimer <= 0 && (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Held))
	{
		bulletFireTimer = bulletFireRate;

		nc::Transform bulletTransform = children[0]->transform;
		//nc::Transform bulletTransform = transform;
		bulletTransform.rotation = (mouseP - transform.position).Angle();
		shoot(bulletTransform);
	}

	// shoot rockets
	rocketFireTimer -= dt;
	if (rocketFireTimer <= 0 && scene->engine->Get<nc::InputSystem>()->GetButtonState((int)nc::InputSystem::eMouseButton::Left) == nc::InputSystem::eKeyState::Held)
	{
		rocketFireTimer = rocketFireRate;

		nc::Transform rocketTransform = children[0]->transform;
		//nc::Transform rocketTransform = transform;
		rocketTransform.rotation = (mouseP - transform.position).Angle();
		shootRocket(rocketTransform);
	}

	// death
	if (health < 0)
	{
		destroy = true;
		//scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 200, 1, nc::Color::white, thrust * 1.25f, thrust * 1.25f, 0.0f, 0.0f, nc::TwoPi);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("destroy");

		nc::Event event;
		event.name = "PlayerDead";
		event.data = std::string("yes i'm dead!");
		scene->engine->Get<nc::EventSystem>()->Notify(event);
	}

	// rocket particles
	//scene->engine->Get<nc::ParticleSystem>()->Create(children[1]->transform.position, 30, 0.25f, nc::Color::cyan, thrust, thrust * 0.25f, transform.rotation + nc::Pi, 0.0f, 0.4f);
}

void Player::OnCollision(Actor* actor)
{
	//return;
	if (actor->hasTag("enemy") && actor->hasTag("bullet")) health -= dynamic_cast<Projectile*>(actor)->damage;
	else if (actor->hasTag("enemy") || actor->hasTag("asteroid")) health -= INT_MAX;
}

void Player::shoot(const nc::Transform& transform)
{
	//// create the projectile
	nc::Transform projectileTransform = transform;
	projectileTransform.scale = 0.2f;
	std::shared_ptr<nc::Texture> texture =
		scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Images/bullet.png", scene->engine->Get<nc::Renderer>());
	std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(projectileTransform, texture, 400.0f, 2);
	projectile->transform.rotation += nc::Random() * 0.2f;
	projectile->addTag("player");
	projectile->addTag("bullet");

	// add projectile to the scene
	scene->AddActor(std::move(projectile));
	scene->engine->Get<nc::AudioSystem>()->PlayAudio("player_fire");
}

void Player::shootRocket(const nc::Transform& transform)
{
	// create the projectile
	nc::Transform projectileTransform = transform;
	projectileTransform.scale = 0.3f;
	std::shared_ptr<nc::Texture> texture =
		scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Images/rocket.png", scene->engine->Get<nc::Renderer>());
	std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(projectileTransform, texture, 50.0f, 20);
	projectile->addTag("player");
	projectile->addTag("rocket");

	//// add projectile to the scene
	scene->AddActor(std::move(projectile));
	scene->engine->Get<nc::AudioSystem>()->PlayAudio("rocket");
}