#include "Game.h"

void Game::Initialize()
{
	// Create Engine
	engine = std::make_unique<nc::Engine>();
	engine->Startup();
	engine->Get<nc::Renderer>()->Create("GAT150", 800, 600);
	engine->time.timeScale = 1.0f;

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

	//Get the font
	int size = 20;
	std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("Fonts/cambriab.ttf", &size);
	// create font texture
	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	// set font texture with font surface
	textTexture->Create(font->CreateSurface("Shoot Stuff", nc::Color::red));
	// add font texture to resource system
	engine->Get<nc::ResourceSystem>()->Add("shootStuff", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("Press Enter to Start", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("start", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("Highscore:", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("highscore", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface(std::to_string(highscore).c_str(), nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("score", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("Instructions", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("instructions", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("A: Turn Left", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("aTurnLeft", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("D: Turn Right", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("dTurnRight", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("W: Move Forward", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("wMoveForward", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("SPACE: Shoot Bullets", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("space", textTexture);

	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	textTexture->Create(font->CreateSurface("Left Click: Shoot Rockets Toward mouse", nc::Color::orange));
	engine->Get<nc::ResourceSystem>()->Add("lClick", textTexture);
	
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
	
	nc::Transform t;

	switch (state)
	{
	case Game::eState::Title:
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 38, engine->Get<nc::Renderer>()->GetHeight() / 2 + static_cast<int>(std::sin(stateTimer * 6) * 10) };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("shootStuff"), t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 70, engine->Get<nc::Renderer>()->GetHeight() / 2 + 50 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("start"), t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 40, engine->Get<nc::Renderer>()->GetHeight() / 2 + 20 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("highscore"), t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 20, engine->Get<nc::Renderer>()->GetHeight() / 2 + 30 };
		break;
	case Game::eState::Instructions:
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 20, engine->Get<nc::Renderer>()->GetHeight() / 2 - 45 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("instructions"), t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 20, engine->Get<nc::Renderer>()->GetHeight() / 2 - 25 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("aTurnLeft"), t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 20, engine->Get<nc::Renderer>()->GetHeight() / 2 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("dTurnRight"), t);
		t.position = { engine->Get<nc::Renderer>()->GetWidth() / 2 - 20, engine->Get<nc::Renderer>()->GetHeight() / 2 + 25 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("wMoveForward"), t);
		//graphics.DrawString(nc::WindowInfo::maxWidth / 2 - 20, nc::WindowInfo::maxHeight / 2 +15, "W: Move Forward");
		//graphics.DrawString(nc::WindowInfo::maxWidth / 2 - 20, nc::WindowInfo::maxHeight / 2 +30, "SPACE: Shoot Bullets");
		//graphics.DrawString(nc::WindowInfo::maxWidth / 2 - 20, nc::WindowInfo::maxHeight / 2 +45, "Left Click: Shoot Rockets Towards Cursor");
		//graphics.DrawString(nc::WindowInfo::maxWidth / 2 - 20, nc::WindowInfo::maxHeight / 2 +75, "Press Enter to Continue");
		break;
	case Game::eState::StartGame:
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::Game:
		/*graphics.SetColor(nc::Color::white);
		graphics.DrawString(nc::WindowInfo::minWidth + 30, nc::WindowInfo::minHeight + 20, ("High Score: " + std::to_string(highscore)).c_str());
		graphics.DrawString(nc::WindowInfo::minWidth + 30, nc::WindowInfo::minHeight + 35, ("Score: " + std::to_string(score)).c_str());
		graphics.DrawString(nc::WindowInfo::maxWidth - 80, nc::WindowInfo::minHeight + 20, ("Lives: " + std::to_string(lives)).c_str());*/
		//if (scene->GetActor<Player>())
		//{
		//	graphics.SetColor(nc::Color::orange);
		//	//graphics.DrawString(nc::WindowInfo::maxWidth / 2 - 30, nc::WindowInfo::minHeight + 20, ("Health: " + std::to_string(scene->GetActor<Player>()->health)).c_str());
		//	if (scene->GetActor<Player>()->rocketFireTimer > 0)
		//	{
		//		std::string t = std::to_string(scene->GetActor<Player>()->rocketFireTimer);
		//		//graphics.DrawString(nc::WindowInfo::maxWidth / 2 - 30, nc::WindowInfo::maxHeight - 20, t.c_str());
		//	}
		//}
		break;
	case Game::eState::GameOver:
		/*graphics.SetColor(nc::Color::red);
		graphics.DrawString(nc::WindowInfo::maxWidth / 2 - 28, nc::WindowInfo::maxHeight / 2, "Game Over");
		graphics.SetColor(nc::Color::orange);
		graphics.DrawString(nc::WindowInfo::maxWidth / 2 - 15, nc::WindowInfo::maxHeight / 2 + 20, "Score:");
		graphics.DrawString(nc::WindowInfo::maxWidth / 2 - 10, nc::WindowInfo::maxHeight / 2 + 35, std::to_string(score).c_str());
		graphics.DrawString(nc::WindowInfo::maxWidth / 2 - 30, nc::WindowInfo::maxHeight / 2 + 50, "High Score:");
		graphics.DrawString(nc::WindowInfo::maxWidth / 2 - 10, nc::WindowInfo::maxHeight / 2 + 60, std::to_string(highscore).c_str());
		graphics.SetColor(nc::Color::white);
		graphics.DrawString(nc::WindowInfo::maxWidth / 2 - 70, nc::WindowInfo::maxHeight / 2 + 100, "Press Enter to Continue");*/
		break;
	default:
		break;
	}

	engine->Draw(engine->Get<nc::Renderer>());
	scene->Draw(engine->Get<nc::Renderer>());

	// Draw
	scene->Draw(engine->Get<nc::Renderer>());

	nc::Vector2 position = engine->Get<nc::InputSystem>()->GetMousePosition();
	//if (engine.Get<nc::InputSystem>()->GetButtonState((int)nc::InputSystem::eMouseButton::Left) == nc::InputSystem::eKeyState::Pressed)
	//{
	//	std::cout << position.x << " " << position.y << std::endl;
	//	engine.Get<nc::AudioSystem>()->PlayAudio("explosion", 1, nc::RandomRange(0.2f, 2.0f));
	//	//Create Particles
	//	channel.SetPitch(nc::RandomRange(0.2f, 2.0f));
	//}



	/*nc::Transform t;
	t.position = { 30, 30 };
	engine.Get<nc::Renderer>()->Draw(textTexture, t);*/



	//End Draw
	engine->Draw(engine->Get<nc::Renderer>());
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