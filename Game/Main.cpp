#include "Engine.h"
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>

#include <cassert>

#define MSG(message) std::cout << #message << std::endl;

int main(int, char**)
{
	//Initialize Game Stuff//////////////
	nc::Timer timer;
	
	nc::Engine engine;
	engine.Startup();

	engine.Get<nc::Renderer>()->Create("GAT150", 800, 600);
	std::cout << timer.ElapsedTicks() << std::endl;

	nc::Scene scene;
	scene.engine = &engine;

	nc::SetFilePath("../Resources");

	// Audio Stuff
	engine.Get<nc::AudioSystem>()->AddAudio("explosion", "Audio/explosion.wav");
	engine.Get<nc::AudioSystem>()->AddAudio("music", "Audio/music.wav");
	nc::AudioChannel channel = engine.Get<nc::AudioSystem>()->PlayAudio("muSic", 1, 1, true);

	// Texture Stuff
	std::shared_ptr<nc::Texture> texture =
		engine.Get<nc::ResourceSystem>()->Get<nc::Texture>("sf2.png", engine.Get<nc::Renderer>());

	// Adding the spinning dude to the screen
	nc::Transform transform{ nc::Vector2{400, 300}, 0.0f, 1.0f };
	std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(transform, texture);
	scene.AddActor(std::move(actor));

	int size = 16;
	std::shared_ptr<nc::Font> font = engine.Get<nc::ResourceSystem>()->Get<nc::Font>("fonts/times.ttf", &size);

	// create font texture
	std::shared_ptr<nc::Texture> textTexture = std::make_shared<nc::Texture>(engine.Get<nc::Renderer>());
	// set font texture with font surface
	textTexture->Create(font->CreateSurface("hello world", nc::Color{ 1, 1, 1 }));
	// add font texture to resource system
	engine.Get<nc::ResourceSystem>()->Add("textTexture", textTexture);

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
		engine.time.timeScale = 1.0f;
		if (!quit) quit = (engine.Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed);

		// Draw
		engine.Get<nc::Renderer>()->BeginFrame();
		scene.Draw(engine.Get<nc::Renderer>());

		nc::Vector2 position = engine.Get<nc::InputSystem>()->GetMousePosition();
		if (engine.Get<nc::InputSystem>()->GetButtonState((int)nc::InputSystem::eMouseButton::Left) == nc::InputSystem::eKeyState::Pressed)
		{
			std::cout << position.x << " " << position.y << std::endl;
			engine.Get<nc::AudioSystem>()->PlayAudio("explosion", 1, nc::RandomRange(0.2f, 2.0f));
			//Create Particles
			channel.SetPitch(nc::RandomRange(0.2f, 2.0f));
		}



		nc::Transform t;
		t.position = { 30, 30 };







		//End Draw
		engine.Get<nc::Renderer>()->Draw(textTexture, t);
		engine.Draw(engine.Get<nc::Renderer>());
		engine.Get<nc::Renderer>()->EndFrame();
	}

	SDL_Quit();

	return 0;
}
