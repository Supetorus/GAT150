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

	engine.Get<nc::AudioSystem>()->AddAudio("explosion", "Audio/explosion.wav");
	engine.Get<nc::AudioSystem>()->AddAudio("music", "Audio/music.wav");
	nc::AudioChannel channel = engine.Get<nc::AudioSystem>()->PlayAudio("music", 1, 1, true);

	std::shared_ptr<nc::Texture> texture =
		engine.Get<nc::ResourceSystem>()->Get<nc::Texture>("sf2.png", engine.Get<nc::Renderer>());

	nc::Transform transform{ nc::Vector2{400, 300}, 0.0f, 1.0f };
	std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(transform, texture);
	scene.AddActor(std::move(actor));

	// Game loop
	bool quit = false;
	SDL_Event event;
	float quitTime = engine.time.time + 300.0f;
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
		//std::cout << engine.time.time << " " << quitTime << std::endl;
		if (engine.time.time >= quitTime) quit = true;
		engine.time.timeScale = 0.1f;
		if (!quit) quit = (engine.Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed);

		nc::Vector2 position = engine.Get<nc::InputSystem>()->GetMousePosition();
		if (engine.Get<nc::InputSystem>()->GetButtonState((int)nc::InputSystem::eMouseButton::Left) == nc::InputSystem::eKeyState::Pressed)
		{
			std::cout << position.x << " " << position.y << std::endl;
			engine.Get<nc::AudioSystem>()->PlayAudio("explosion", 1, nc::RandomRange(0.2f, 2.0f));
			channel.SetPitch(nc::RandomRange(0.2f, 2.0f));
		}

		// Draw
		engine.Get<nc::Renderer>()->BeginFrame();
		scene.Draw(engine.Get<nc::Renderer>());
		engine.Get<nc::Renderer>()->EndFrame();
	}

	SDL_Quit();

	return 0;
}
