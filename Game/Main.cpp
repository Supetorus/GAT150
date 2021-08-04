#include "Engine.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>

int main(int, char**)
{

	nc::Engine engine;
	engine.Startup();

	engine.Get<nc::Renderer>()->Create("GAT150", 800, 600);
	
	std::cout << nc::GetFilePath() << std::endl;
	nc::SetFilePath("../Resources");
	std::cout << nc::GetFilePath() << std::endl;

	std::shared_ptr<nc::Texture> texture =
		engine.Get<nc::ResourceSystem>()->Get<nc::Texture>("sf2.png", engine.Get<nc::Renderer>());

	// Game loop
	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		engine.Get<nc::Renderer>()->BeginFrame();

		nc::Vector2 position{ 300, 400 };
		engine.Get<nc::Renderer>()->Draw(texture, position);

		engine.Get<nc::Renderer>()->EndFrame();

		/*for (size_t i = 0; i < 50; i++)
		{
			SDL_Rect src{ 32, 64, 32, 64 };
			SDL_Rect dest{ nc::RandomRangeInt(0, screen.x), nc::RandomRangeInt(0, screen.y), 16, 24 };
			SDL_RenderCopy(renderer, texture, &src, &dest);
		}*/

	}

	SDL_Quit();

	return 0;
}
