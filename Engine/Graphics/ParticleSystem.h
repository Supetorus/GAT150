#pragma once
#include "Math/Vector2.h"
#include "Math/Color.h"
#include "Framework/System.h"
#include <vector>

namespace nc
{
	class Texture;
	class ParticleSystem : public GraphicsSystem
	{
	private:
		struct Particle
		{
			Vector2 position;
			Vector2 prevPosition;
			Vector2 velocity;
			float direction;
			float speed;
			std::shared_ptr<Texture> texture;
			float lifetime;
			float rotation;
			bool isActive{ false };

			static bool IsNotActive(Particle particle) { return !particle.isActive; }
		};

	public:
		void Startup() override;
		void Shutdown() override;

		void Update(float dt) override;
		void Draw(class Renderer* renderer) override;

		//void Create(Vector2& position, size_t count, float lifetime, const Color& color, float thrust);
		void Create(Vector2& position, size_t count, float lifetime, std::shared_ptr<Texture> texture, float minSpeed, float maxSpeed, float direction, float rotation, float angleRange);
		//void Create(Vector2& position, size_t count, float lifetime, const std::vector<Color>& colors, float minSpeed, float maxSpeed, float direction, float rotation, float angleRange);
		//void Create(Vector2& position, size_t count, float minifetime, float maxLifetime, const Color& color, float minSpeed, float maxSpeed, float direction, float rotation, float angleRange);

	private:
		std::vector<Particle> particles;
	};
}