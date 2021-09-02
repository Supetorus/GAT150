#pragma once
#include "Engine.h"

class Game
{
public:
	enum class eState
	{
		Reset,
		Title,
		StartGame,
		StartLevel,
		Level,
		PlayerDead,
		GameOver
	};

public:
	void Initialize();
	void Shutdown();

	void Update();
	void Draw();

	bool isQuit() { return quit; }

private:
	void Reset();
	void Title();
	void StartGame();
	void StartLevel();
	void Level();
	void PlayerDead();
	void GameOver();

	void OnAddScore(const nc::Event& event);

public:
	std::unique_ptr<nc::Engine> engine;
	std::unique_ptr<nc::Scene> scene;

private:
	bool quit = false;

	eState state = eState::Reset;
	int score = 0;
	float stateTimer = 0;
	float spawnTimer = 0;

	std::vector<nc::Vector2> spawnLocations{
		{100, 100},
		{200, 420},
		{100, 540},
		{668, 50},
		{568, 420},
		{668, 540}
	};
};