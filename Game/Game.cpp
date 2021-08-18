#include "Game.h"

void Game::Initialize()
{
	// Create Engine
	engine = std::make_unique<nc::Engine>();
	engine->Startup();
	engine->time.timeScale = 1.0f;

	// Create renderer
	engine->Get<nc::Renderer>()->Create("GAT150", 1500, 800);

	// Create Scene
	scene = std::make_unique<nc::Scene>();
	scene->engine = engine.get();

	// Seed RNG
	nc::SeedRandom(static_cast<unsigned int>(time(nullptr)));

	// Set File Path
	nc::SetFilePath("../Resources");

	// Actors
	//std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(nc::Transform{ { 400, 300 } });
	//{
	//	nc::SpriteComponent* component = actor->AddComponent<nc::SpriteComponent>();
	//	component->texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Images/link_1.png", engine->Get < nc::Renderer>());
	//}
	//{
	//	nc::PhysicsComponent* component = actor->AddComponent<nc::PhysicsComponent>();
	//	//component->ApplyForce(nc::Vector2::right * 200);
	//}
	//scene->AddActor(std::move(actor));

	std::unique_ptr<nc::Actor> actor = std::make_unique <nc::Actor>(nc::Transform{ nc::Vector2{400, 300}, 0, 1 });
	{
		nc::SpriteAnimationComponent* component = actor->AddComponent<nc::SpriteAnimationComponent>();
		component->texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Images/sparkle.png", engine->Get<nc::Renderer>());
		component->fps = 24;
		component->numFramesX = 8;
		component->numFramesY = 8;
	}
	scene->AddActor(std::move(actor));

}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update()
{
	engine->Update();

	if (!quit) quit = (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed);
	// Update
	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<nc::Renderer>()->BeginFrame();
	
	//End Draw
	engine->Draw(engine->Get<nc::Renderer>());
	scene->Draw(engine->Get<nc::Renderer>());
	engine->Get<nc::Renderer>()->EndFrame();
}