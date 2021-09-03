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
	//engine->Get<nc::Renderer>()->Create("GAT150", 1500, 800);
	engine->Get<nc::Renderer>()->Create("GAT150", 768, 640);

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

	// Events
	engine->Get<nc::EventSystem>()->Subscribe("add_score", 
		std::bind(&Game::OnAddScore, this, std::placeholders::_1));
	// Player Death
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

	switch (state)
	{
	case Game::eState::Reset:
		Reset();
		break;
	case Game::eState::Title:
		Title();
		break;
	case Game::eState::StartGame:
		StartGame();
		break;
	case Game::eState::StartLevel:
		StartLevel();
		break;
	case Game::eState::Level:
		Level();
		break;
	case Game::eState::PlayerDead:
		PlayerDead();
		break;
	case Game::eState::GameOver:
		GameOver();
		break;
	default:
		break;
	}

	// Update Score
	auto scoreActor = scene->FindActor("Score");
	if (scoreActor) scoreActor->GetComponent<nc::TextComponent>()->SetText(std::to_string(score));

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

void Game::Reset()
{
	scene->RemoveAllActors();

	rapidjson::Document document;
	bool success = nc::json::Load("title.txt", document);
	assert(success);
	scene->Read(document);

	state = eState::Title;
}

void Game::Title()
{
	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed)
	{
		auto title = scene->FindActor("Title");
		title->active = false;
		state = eState::StartGame;

	}
}

void Game::StartGame()
{
	rapidjson::Document document;
	bool success = nc::json::Load("scene.txt", document);
	assert(success);
	scene->Read(document);

	nc::Tilemap tilemap;
	tilemap.scene = scene.get();
	success = nc::json::Load("map.txt", document);
	assert(success);
	tilemap.Read(document);
	tilemap.Create();

	state = eState::StartLevel;
	stateTimer = 0;
}

void Game::StartLevel()
{
	stateTimer += engine->time.deltaTime;
	if (stateTimer >= 1)
	{
		auto player = nc::ObjectFactory::Instance().Create<nc::Actor>("Player");
		player->transform.position = { 400, 200 };
		scene->AddActor(std::move(player));

		spawnTimer = 2;
		state = eState::Level;
	}
}

void Game::Level()
{
	spawnTimer -= engine->time.deltaTime;
	if (spawnTimer <= 0)
	{
		spawnTimer = nc::RandomRange(2, 4);

		auto coin = nc::ObjectFactory::Instance().Create<nc::Actor>("Coin");
		coin->transform.position = spawnLocations[nc::RandomRangeInt(0, spawnLocations.size()-1)];
		scene->AddActor(std::move(coin));
	}
	
	//auto enemy = nc::ObjectFactory::Instance().Create<nc::Actor>("Enemy");
	//enemy->transform.position = spawnLocations[nc::RandomRangeInt(0, spawnLocations.size() - 1)];
	//scene->AddActor(std::move(enemy));

	if (!scene->FindActor("Enemy"))
	{
		auto enemy = nc::ObjectFactory::Instance().Create<nc::Actor>("Enemy");
		enemy->transform.position = spawnLocations[nc::RandomRangeInt(0, spawnLocations.size() - 1)];
		scene->AddActor(std::move(enemy));
	}
}

void Game::PlayerDead()
{

}

void Game::GameOver()
{

}

void Game::OnAddScore(const nc::Event& event)
{
	score += std::get<int>(event.data);
}
