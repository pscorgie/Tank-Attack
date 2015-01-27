#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"
#include <time.h>

#define BULLETS 25

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

struct Player
{
	Rect* posRect;
	Rect* sourceRect;
	Texture2D* texture;
	float speedMultiplier;
	int direction;
	int ammo;
	int life;
	bool isAlive;
};

struct Enemy
{
	Rect* posRect;
	Rect* sourceRect;
	Texture2D* texture;
	int direction;
	float fireRate;
	float currentTime;
	bool patrol;
	int life;
	
};

struct Bullet
{
	Rect* posRect;
	Rect* sourceRect;
	Texture2D* texture;
	bool shot;
	int direction;
	Vector2* dir;
};

struct Collectable
{
	Rect* posRect;
	Texture2D* texture;
	Rect* sourceRect;
	bool spawn;
	float spawnRate;
	float spawnTimer;
	bool collected;
	float frameRate;
	float currentFrameRate;
	int currentFrame;
};

struct Score
{
	string playerName;
	string difficulty;
	Vector2* position;
	float time;
	bool played;
};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	Player* _player;
	Enemy* _turret;
	Enemy* _tank;
	Bullet* _bullet[BULLETS];
	Bullet* _turretBullet[BULLETS];
	Bullet* _tankBullet[BULLETS];
	Collectable* _ammo;
	Collectable* _health;
	Texture2D* _explode;
	Score* _scores[5];
	Score* _tempScore;
	bool _playerShot;

	enum direction{Up, Down, Left, Right, None};
	direction currentDirection;
	direction AiDirection;
	direction currentAIDirection;
	direction previousAIDirection;
	enum gameState{StartMenu, SinglePlayer, GameOver, ScoreTable};
	gameState currentGameState;
	enum gameDifficulty{Normal, Hard};
	gameDifficulty currentGameDifficulty;

	Texture2D* _gameLogo;
	Rect* _gameLogoRect;
	Texture2D* _gameModesLogo;
	Rect* _gameModesLogoRect;
	Texture2D* _pointer;
	Rect* _pointerRect;
	Texture2D* _gameOver;
	Rect* _gameOverRect;
	int _enemiesRemaining;
	int _scoreUsed;
	int _pointerPos;
	float _AiMoveTimer;
	float _timer;
	float _gameTime;
	float _AiRate;
	bool _paused;
	bool _pKeyDown;
	bool _sortScore;
	bool _isKeyDown;
	bool _usedScores;
	bool _AISound;
	string _AISoundStr;
	
	
	Texture2D* _background;
	Rect* _backRect;

	SoundEffect* _shot;
	SoundEffect* _reload;
	SoundEffect* _explosion;
	SoundEffect* _viewportCollision;
	SoundEffect* _bulletCollision;
	SoundEffect* _enemyShot;
	SoundEffect* _heal;

	// Position for String
	Vector2* _stringPosition;
	Vector2* _pausedStringPos;
	Vector2* _healthStringPos;
	Vector2* _turretHealthStringPos;
	Vector2* _tankHealthStringPos;
	Vector2* _ammoStringPos;
	Vector2* _resetStringPos;
	Vector2* _AISoundPos;

	//Constant data for game variables
	const float _cPlayerSpeed;

	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);
	void MenuInput(Input::KeyboardState* state);
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void LoadGameVariables();
	void GameTime();
		
	// Player
	void UpdatePlayer(int elapsedTime);
	void PlayerShooting(bool _playerShot);
	void UpdateBullet(int elapsedTime);
	bool BulletCollision(int i, float X, float Y, float height, float width);
	void PlayerDeath();
	void PlayerViewPortCollision();

	// Turret
	void UpdateTurret(int elapsedTime);
	void UpdateTurretBullet(int elapsedTime);
	void TurretShooting(int elapsedTime);

	// Turret and AI Methods
	bool PlayerBulletCollision(int i, float X, float Y, float height, float width);
	bool CheckPlayerRange(float X, float Y, float height, float width);
	void AiDeath();

	// Ammo
	void SpawnAmmo(int elapsedTime);
	bool PlayerAmmoCollision();
	void AmmoAnimation();

	// Health
	void SpawnHealth();
	bool PlayerHealthCollision();
	void HealthAnimation();

	// AI tank
	void UpdateAI(int elapsedTime);
	void AiShooting(int elapsedTime);
	void MoveAI(int elapsedTime);
	void UpdateTankBullet(int elapsedTime, int i);
	bool CheckInLine(float aX, float pX, float aDim, float pDim);
	void AiViewPortCollision();
	void AiChangeDirection();
	void UpdateAIFireDirection();
	
	bool BulletViewPortCollision(float X, float Y, float Height, float Width);
	
	// ScoreSheet
	void ScoreSheet();
	void SortScore();
	int FindLargest(int last);

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};