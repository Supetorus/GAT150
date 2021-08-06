#include "Engine.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>

int main(int, char**)
{
	nc::Timer timer;
	
	nc::Engine engine;
	engine.Startup();

	engine.Get<nc::Renderer>()->Create("GAT150", 800, 600);
	std::cout << timer.ElapsedTicks() << std::endl;


	nc::Scene scene;
	scene.engine = &engine;

	nc::SetFilePath("../Resources");
	std::shared_ptr<nc::Texture> texture =
		engine.Get<nc::ResourceSystem>()->Get<nc::Texture>("sf2.png", engine.Get<nc::Renderer>());

	nc::Transform transform{ nc::Vector2{400, 300}, 0.0f, 1.0f };
	std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(transform, texture);
	scene.AddActor(std::move(actor));

	// Game loop
	bool quit = false;
	SDL_Event event;
	float quitTime = engine.time.time + 3.0f;
	while (!quit)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		// Update
		engine.Update();
		scene.Update(engine.time.deltaTime);

		//std::cout << engine.time.time << std::endl;
		std::cout << engine.time.time << " " << quitTime << std::endl;
		if (engine.time.time >= quitTime) quit = true;
		engine.time.timeScale = 0.1f;
		quit = (engine.Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed);

		// Draw
		engine.Get<nc::Renderer>()->BeginFrame();
		scene.Draw(engine.Get<nc::Renderer>());
		engine.Get<nc::Renderer>()->EndFrame();
	}

	SDL_Quit();

	return 0;
}
