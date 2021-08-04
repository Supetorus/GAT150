#include "Renderer.h"
#include <SDL_image.h>
#include <iostream>

namespace nc
{
	void Renderer::Startup()
	{
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		}
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	}

	void Renderer::Shutdown()
	{
		IMG_Quit();
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}

	void Renderer::Update(float dt)
	{
	}

	void Renderer::Create(const std::string& name, int width, int height)
	{
		window = SDL_CreateWindow(name.c_str(), 100, 100, width, height, SDL_WINDOW_SHOWN);
		if (window == nullptr)
		{
			std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
			SDL_Quit();
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
	}

	void Renderer::BeginFrame()
	{
		SDL_RenderClear(renderer);
	}

	void Renderer::EndFrame()
	{
		SDL_RenderPresent(renderer);
	}

	void Renderer::Draw(std::shared_ptr<nc::Texture> texture, const Vector2& position)
	{
		SDL_Rect dest{ (int)position.x, (int)position.y, 64, 96};
		SDL_RenderCopy(renderer, texture->texture, nullptr, &dest);
	}
}
