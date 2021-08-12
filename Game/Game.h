#pragma once
#include "Engine.h"
#include "Math/Transform.h"
#include "Math/Random.h"
#include <fstream>

class Game
{
public:
	enum class eState
	{
		Title,
		Instructions,
		StartGame,
		StartLevel,
		Game,
		GameOver
	};

public:
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();

	bool isQuit() { return quit; }

private:
	void UpdateTitle(float dt);
	void StartLevel();
	void OnAddPoints(const nc::Event& event);
	void OnPlayerDead(const nc::Event& event);
	void SpawnPlayer();

public:
	std::unique_ptr<nc::Engine> engine;
	std::unique_ptr<nc::Scene> scene;

private:
	bool quit = false;
	eState state = eState::Title;
	float stateTimer = 0.0f;
	float audioTimer = 0.0f;

	size_t score = 0;
	size_t lives = 3;
	size_t level = 0;
	size_t highscore = 0;

	//title toy
	float fireTimer{ 0 };
	float fireRate{ 0.1f }; // time between shots
	float screenTimer = 0;

	nc::AudioChannel musicChannel;
	std::shared_ptr<nc::Texture> particleTexture;
	std::shared_ptr<nc::Texture> textTexture;
};