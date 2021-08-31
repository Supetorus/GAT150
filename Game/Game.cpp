#include "Game.h"
#include "GameComponent/PlayerComponent.h"
#include "GameComponent/EnemyComponent.h"
#include "GameComponent/PickupComponent.h"


void Game::Initialize()
{
	// Create Engine
	engine = std::make_unique<nc::Engine>();
	engine->Startup();
	engine->time.timeScale = 1.0f;

	// Create renderer
	engine->Get<nc::Renderer>()->Create("GAT150", 1500, 800);

	// register classes
	REGISTER_CLASS(PlayerComponent);
	REGISTER_CLASS(EnemyComponent);
	REGISTER_CLASS(PickupComponent);

	// Create Scene
	scene = std::make_unique<nc::Scene>();
	scene->engine = engine.get();

	// Seed RNG
	nc::SeedRandom(static_cast<unsigned int>(time(nullptr)));

	// Set File Path
	nc::SetFilePath("../Resources");

	// Actors
	rapidjson::Document document;
	bool success = nc::json::Load("scene.txt", document);
	assert(success);
	scene->Read(document);

	for (int i = 0; i < 0; i++)
	{
		auto actor = nc::ObjectFactory::Instance().Create<nc::Actor>("Coin");
		actor->transform.position = nc::Vector2{ nc::RandomRange(0, 800), nc::RandomRange(100, 300) };
		scene->AddActor(std::move(actor));
	}
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