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

	// Seed Random
	nc::SeedRandom(static_cast<unsigned int>(time(nullptr)));

	// Set File Path
	nc::SetFilePath("../Resources");

	// load sounds
	engine->Get<nc::AudioSystem>()->AddAudio("explosion", "Audio/explosion.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("explosion_1", "Audio/explosion_1.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("enemy_fire", "Audio/enemy_shoot.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("player_fire", "Audio/player_shoot.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("music", "Audio/music.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("destroy", "Audio/destroy.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("destroy_1", "Audio/destroy_1.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("rocket", "Audio/rocket.wav");
	musicChannel = engine->Get<nc::AudioSystem>()->PlayAudio("music", 1, 1, true);

	// Subscribe to events
	engine->Get<nc::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1));
	engine->Get<nc::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead, this, std::placeholders::_1));
	
	//load highscore
	std::ifstream input("high_score.txt");
	if (input.is_open())
	{
		std::string s;
		std::getline(input, s);
		highscore = std::stoi(s);
		//input >> highscore;
	}
	input.close();

	// Create texts
	InitText();
}

void Game::InitText()
{
	int size = 20;
	std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/cambriab.ttf", &size);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("Shoot Stuff", nc::Color::red));
	engine->Get<nc::ResourceSystem>()->Add("shootStuffTexture", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("Press Enter to Continue", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("startTexture", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("Highscore: " + std::to_string(highscore), nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("highscoreTexture", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("Score: ", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("scoreTexture", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("Lives: ", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("livesTexture", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("Health: ", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("healthTexture", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("Rocket In: ", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("rocketInTexture", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("Instructions", nc::Color::red));
	engine->Get<nc::ResourceSystem>()->Add("instructionsTexture", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("A: Turn Left", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("aTurnLeftTexture", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("D: Turn Right", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("dTurnRightTexture", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("W: Move Forward", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("wMoveForwardTexture", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("SPACE: Shoot Bullets", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("spaceShootTexture", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("Left Click: Shoot Rockets Toward mouse", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("lClickTexture", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("Game Over", nc::Color::red));
	engine->Get<nc::ResourceSystem>()->Add("gameOverTexture", textTexture);
}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update()
{
	engine->Update();

	float dt = engine->time.deltaTime;
	stateTimer += dt;
	screenTimer += dt;

	switch (state)
	{
	case Game::eState::Title:
		UpdateTitle(dt);
		break;
	case Game::eState::Instructions:
		if ((engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_RETURN) == nc::InputSystem::eKeyState::Pressed))
		{
			state = eState::StartGame;
		}
		break;
	case Game::eState::StartGame:
		state = eState::StartLevel;
		scene->RemoveAllActors();
		SpawnPlayer();
		lives = 3;
		score = 0;
		level = 0;
		break;
	case Game::eState::StartLevel:
		StartLevel();
		state = eState::Game;
		break;
	case Game::eState::Game:
		if (scene->GetActors<Enemy>().size() == 0)
		{
			level++;
			state = eState::StartLevel;
		}
		break;
	case Game::eState::GameOver:
		scene->RemoveAllActors();
		if ((engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_RETURN) == nc::InputSystem::eKeyState::Pressed))
		{
			state = eState::Title;
			screenTimer = 0;
		}
		break;
	default:
		break;
	}

	if (!quit) quit = (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed);
	// Update
	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<nc::Renderer>()->BeginFrame();
	
	int size = 12;
	std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/cambriab.ttf", &size);
	nc::Transform t{};
	std::shared_ptr<nc::Texture> texture;

	switch (state)
	{
	case Game::eState::Title:
	{
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 38, engine->Get<nc::Renderer>()->GetHeight() / 2 + static_cast<int>(std::sin(stateTimer * 6) * 10 - 50) };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("shootStuffTexture"), t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 40, engine->Get<nc::Renderer>()->GetHeight() / 2 + 20 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("highscoreTexture"), t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 70, engine->Get<nc::Renderer>()->GetHeight() / 2 + 50 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("startTexture"), t);
	}
		break;
	case Game::eState::Instructions:
	{
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 20, engine->Get<nc::Renderer>()->GetHeight() / 2 - 50 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("instructionsTexture"), t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 20, engine->Get<nc::Renderer>()->GetHeight() / 2 - 25 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("aTurnLeftTexture"), t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 20, engine->Get<nc::Renderer>()->GetHeight() / 2 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("dTurnRightTexture"), t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 20, engine->Get<nc::Renderer>()->GetHeight() / 2 + 25 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("wMoveForwardTexture"), t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 20, engine->Get<nc::Renderer>()->GetHeight() / 2 + 50 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("spaceShootTexture"), t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 20, engine->Get<nc::Renderer>()->GetHeight() / 2 + 75 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("lClickTexture"), t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 20, engine->Get<nc::Renderer>()->GetHeight() / 2 + 100 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("startTexture"), t);
	}
		break;
	case Game::eState::StartGame:
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::Game:
	{
		nc::Transform t;
		t.position = { 60, 20 };
		texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("highscoreTexture", engine->Get<nc::Renderer>());
		texture->Create(font->CreateSurface("Highscore: " + std::to_string(highscore), nc::Color::white));
		engine->Get<nc::Renderer>()->Draw(texture, t);
		t.position = { 60, 40 };
		texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("scoreTexture", engine->Get<nc::Renderer>());
		texture->Create(font->CreateSurface("Score: " + std::to_string(score), nc::Color::white));
		engine->Get<nc::Renderer>()->Draw(texture, t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() - 80, 20 };
		texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("livesTexture", engine->Get<nc::Renderer>());
		texture->Create(font->CreateSurface("Lives: " + std::to_string(lives), nc::Color::white));
		engine->Get<nc::Renderer>()->Draw(texture, t);

		if (scene->GetActor<Player>())
		{
			t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 30, 20 };
			texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("healthTexture", engine->Get<nc::Renderer>());
			texture->Create(font->CreateSurface("Health: " + std::to_string(scene->GetActor<Player>()->health), nc::Color::white));
			engine->Get<nc::Renderer>()->Draw(texture, t);
			if (scene->GetActor<Player>()->rocketFireTimer > 0)
			{
				t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 30, engine->Get<nc::Renderer>()->GetHeight() - 20 };
				texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("rocketInTexture", engine->Get<nc::Renderer>());
				texture->Create(font->CreateSurface("Rocket In: " + std::to_string(scene->GetActor<Player>()->rocketFireTimer), nc::Color::white));
				engine->Get<nc::Renderer>()->Draw(texture, t);
			}
		}
	}
		break;
	case Game::eState::GameOver:
	{
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 28, engine->Get<nc::Renderer>()->GetHeight() / 2 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("gameOverTexture"), t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 15, engine->Get<nc::Renderer>()->GetHeight() / 2 + 20 };
		texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("scoreTexture", engine->Get<nc::Renderer>());
		texture->Create(font->CreateSurface("Final Score: " + std::to_string(score), nc::Color::orange));
		engine->Get<nc::Renderer>()->Draw(texture, t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 15, engine->Get<nc::Renderer>()->GetHeight() / 2 + 40 };
		texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("highscoreTexture", engine->Get<nc::Renderer>());
		texture->Create(font->CreateSurface("Highscore: " + std::to_string(highscore), nc::Color::orange));
		engine->Get<nc::Renderer>()->Draw(texture, t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 20, engine->Get<nc::Renderer>()->GetHeight() / 2 + 60 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("startTexture"), t);
	}
		break;
	default:
		break;
	}

	//End Draw
	engine->Draw(engine->Get<nc::Renderer>());
	scene->Draw(engine->Get<nc::Renderer>());
	engine->Get<nc::Renderer>()->EndFrame();
}

void Game::UpdateTitle(float dt)
{
	
	if ((engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_RETURN) == nc::InputSystem::eKeyState::Pressed))
	{
		state = eState::Instructions;
		screenTimer = 0;
	}
}

void Game::StartLevel()
{
	scene->RemoveByTag("bullet");
	scene->RemoveByTag("asteroid");

	// add asteroids
	for (size_t i = 0; i < 1 + level; i++)
	{
		std::string asteroidNum = std::to_string(rand() % 5);
		std::shared_ptr<nc::Texture> texture =
			engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Images/asteroid_" + asteroidNum + ".png", engine->Get<nc::Renderer>());

		std::unique_ptr<Asteroid> asteroid = std::make_unique<Asteroid>(
			nc::Transform{ {}, nc::RandomRange(0.0f, nc::TwoPi), 1 },
			texture, 100.0f);
		nc::Vector2 location = scene->SafeLocation(asteroid->GetRadius(), 100);
		asteroid->transform.position = location;
		scene->AddActor(std::move(asteroid));
	}

	// add enemies
	for (size_t i = 0; i < 3 + level; i++)
	{
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(
			nc::Transform{ {}, nc::RandomRange(0.0f, nc::TwoPi), 0.5f },
			engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Images/flame_blade.png", engine->Get<nc::Renderer>()), 100.0f);
		nc::Vector2 location = scene->SafeLocation(enemy->GetRadius(), 500);
		enemy->transform.position = location;
		scene->AddActor(std::move(enemy));
	}
}

void Game::OnAddPoints(const nc::Event& event)
{
	score += std::get<int>(event.data);
}

void Game::OnPlayerDead(const nc::Event& event)
{
	lives--;
	if (lives > 0)
	{
		SpawnPlayer();
	}
	else
	{
		if (score > highscore)
		{
			highscore = score;
			std::ofstream output("high_score.txt");
			if (output.is_open())
			{
				output << highscore;
			}
		}
		state = eState::GameOver;
	}
}

void Game::SpawnPlayer()
{
	// Texture Stuff
	std::shared_ptr<nc::Texture> texture =
		engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("Images/green_arrow.png", engine->Get<nc::Renderer>());

	std::unique_ptr<Player> player = std::make_unique<Player>(
		nc::Transform{ nc::Vector2{0, 0}, 0.0f, 0.5f }, // position, rotation, scale
		texture);
	if (scene->ActorCount() > 0) player->transform.position = scene->SafeLocation(player->GetRadius(), 500.0f);
	else player->transform.position = { engine->Get<nc::Renderer>()->GetWidth() / 2, engine->Get<nc::Renderer>()->GetWidth() / 2 };
	scene->AddActor(std::move(player));
}


