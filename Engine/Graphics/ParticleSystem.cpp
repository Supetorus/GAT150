#include "ParticleSystem.h"
#include "Renderer.h"
#include "Texture.h"
#include "Math/Random.h"

namespace nc
{

	void ParticleSystem::Startup()
	{
		particles.resize(10000);
	}

	void ParticleSystem::Shutdown()
	{
		particles.clear();
	}

	void ParticleSystem::Update(float dt)
	{
		for (Particle& particle : particles)
		{
			if (particle.isActive)
			{
				particle.lifetime -= dt;
				particle.isActive = particle.lifetime > 0;
				particle.prevPosition = particle.position;
				particle.direction += particle.rotation;
				particle.velocity = Vector2{ std::cos(particle.direction), std::sin(particle.direction)} * particle.speed * dt;
				particle.position += particle.velocity;
			}
		}
	}

	// Draws every particle in the system
	void ParticleSystem::Draw(Renderer* renderer)
	{
		for (const Particle& particle : particles)
		{
			if (particle.isActive)
			{
				
			}
		}
	}

	// creates a group of particles in a direction (in radians)
	void ParticleSystem::Create(Vector2& position, size_t count, float lifetime, std::shared_ptr<Texture> texture, float minSpeed, float maxSpeed, float direction, float rotation, float angleRange)
	{
		for (size_t i = 0; i < count; i++)
		{
			auto particle = std::find_if(particles.begin(), particles.end(), Particle::IsNotActive);
			if (particle != particles.end())
			{
				particle->isActive = true;
				particle->lifetime = lifetime;
				particle->position = position;
				particle->prevPosition = position;
				particle->texture = texture;

				particle->rotation = rotation;
				particle->direction = direction + RandomRange(-angleRange / 2, angleRange / 2);
				particle->speed = RandomRange(minSpeed, maxSpeed);
			}
		}
	}
	/*void ParticleSystem::Create(Vector2& position, size_t count, float lifetime, const std::vector<Color>& colors, float minSpeed, float maxSpeed, float direction, float rotation, float angleRange)
	{
		for (size_t i = 0; i < count; i++)
		{
			auto particle = std::find_if(particles.begin(), particles.end(), Particle::IsNotActive);
			if (particle != particles.end())
			{
				particle->isActive = true;
				particle->lifetime = lifetime;
				particle->position = position;
				particle->prevPosition = position;
				particle->color = colors[rand() % colors.size()];
				particle->rotation = rotation;
				particle->direction = direction + RandomRange(-angleRange / 2, angleRange / 2);
				particle->speed = RandomRange(minSpeed, maxSpeed);
			}
		}
	}*/
}