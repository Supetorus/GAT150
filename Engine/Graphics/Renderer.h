#pragma once
#include "Framework/System.h"
#include "Texture.h"
#include "Math/Vector2.h"
#include <SDL.h>
#include <string>

namespace nc
{
	class Renderer : public System
	{
	public:
		void Startup() override; // virtual means it can be "extended" or "inherited from". = 0 means it doesn't have any functionality by itself.
		void Shutdown() override;
		void Update(float dt) override;

		void Create(const std::string& name, int width, int height);
		void BeginFrame();
		void EndFrame();

		void Draw(std::shared_ptr<nc::Texture> texture, const Vector2& position);

		friend class Texture; // makes the texture class a friend of renderer, so texture has access to the renderer declared in private.

	private:
		SDL_Renderer* renderer{ nullptr };
		SDL_Window* window{ nullptr };
		
	};
}