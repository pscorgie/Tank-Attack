#include "Pacman.h"

#include <sstream>
#include <iostream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPlayerSpeed(0.2f)
{
	srand(time(NULL));

	_player = new Player();
	_turret = new Enemy();
	_tank = new Enemy();
	_ammo = new Collectable();
	_health = new Collectable();
	_tempScore = new Score();
	
	_player->speedMultiplier = 1.0f;
	_player->direction = 0;
	_player->ammo = 3;
	_player->life = 3;
	_player->isAlive = true;


	_turret->direction = 0;
	_turret->currentTime = 1.0;
	_turret->fireRate = 1.0;
	_turret->life = 5;
	
	_tank->direction = 0;
	_tank->currentTime = 0.0;
	_tank->fireRate = 1.0;
	_tank->patrol = true;
	_tank->life = 3;

	_ammo->spawn = false;
	_ammo->spawnRate = 3.0;
	_ammo->spawnTimer = 0.0;
	_ammo->collected = true;
	_ammo->frameRate = 4.0;
	_ammo->currentFrameRate = 0.0;
	_ammo->currentFrame = 0;

	_health->spawn = false;
	_health->spawnRate = 3.0;
	_health->spawnTimer = 0.0;
	_health->collected = true;
	_health->frameRate = 2.0;
	_health->currentFrameRate = 0.0;
	_health->currentFrame = 0;

	for(int i = 0; i < BULLETS; i++)
	{
		_bullet[i] = new Bullet();
		_bullet[i]->shot = false;

	}
	for(int i = 0; i < BULLETS; i++)
	{
		_turretBullet[i] = new Bullet();
		_turretBullet[i]->shot = false;
		_turretBullet[i]->dir = new Vector2(0,(i*50));
		_tankBullet[i] = new Bullet();
		_tankBullet[i]->shot = false;
	}
	for (int i = 0; i < 5; i++)
	{
		_scores[i] = new Score();
		_scores[i]->playerName = "Player 1";
		_scores[i]->difficulty = "Normal";
		_scores[i]->position = new Vector2(280, 0);
		_scores[i]->time = 0.0;
		_scores[i]->played = false;
	}

	_tempScore->playerName = " ";
	_tempScore->difficulty = "Normal";
	_tempScore->position = new Vector2(0,0);
	_tempScore->time = 0.0;
	_tempScore->played = false;

	_playerShot = false;
	_AiMoveTimer = 0.0;
	_timer = 3.0;
	_gameTime = 0.0;
	_AiRate = 0.0;
	_paused = false;
	_pKeyDown = false;
	_isKeyDown = false;
	_enemiesRemaining = 2;
	_scoreUsed = 0;
	_pointerPos = 0;
	_sortScore = false;
	_usedScores = false;
	currentGameState = StartMenu;
	AiDirection = Left;
	currentGameDifficulty = Normal;

	_AISound = true;
	_AISoundStr = "AI SOUND ON";


	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();
	
	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _player->texture;
	delete _player->sourceRect;
	delete _player->posRect;
	delete _explode;
	delete _turret->texture;
	delete _turret->sourceRect;
	delete _turret->posRect;
	delete _tank->texture;
	delete _tank->posRect;
	delete _tank->sourceRect;
	delete _ammo->texture;
	delete _ammo->posRect;
	delete _ammo->sourceRect;
	delete _health->texture;
	delete _health->posRect;
	delete _health->sourceRect;

	for(int i = 0; i < BULLETS; i++)
	{
		delete _bullet[i]->sourceRect;
		delete _bullet[i]->posRect;
		delete _turretBullet[i]->sourceRect;
		delete _turretBullet[i]->dir;
		delete _turretBullet[i]->posRect;
		delete _tankBullet[i]->sourceRect;
		delete _tankBullet[i]->posRect;
	}
	delete [] _bullet;
	delete [] _turretBullet;
	delete [] _tankBullet;
	delete _background;
	delete _backRect;
	delete _gameLogo;
	delete _gameLogoRect;
	delete _gameModesLogo;
	delete _gameModesLogoRect;
	delete _pointer;
	delete _pointerRect;
	delete _gameOver;
	delete _gameOverRect;
	delete _shot;
	delete _reload;
	delete _viewportCollision;
	delete _bulletCollision;
	delete _enemyShot;
	delete _heal;
	delete _explosion;
	for (int i = 0; i < 5; i++)
	{
		delete _scores[i]->position;
	}
	delete [] _scores;
	delete _tempScore->position;
}

void Pacman::LoadContent()
{
	// Load Player
	_player->texture = new Texture2D();
	_player->texture->Load("Textures/PlayerTankSheet.PNG", false);
	_player->posRect = new Rect(0.0f, 0.0f, 30, 30);
	_player->sourceRect = new Rect(0.0f, 0.0f, 30, 30);

	// Load Turret
	_turret->texture = new Texture2D();
	_turret->texture->Load("Textures/TurretSheet.PNG", true);
	_turret->posRect = new Rect(492.0f, 364.0f, 40, 40);
	_turret->sourceRect = new Rect(0, 0, 40, 40);

	_tank->texture = new Texture2D();
	_tank->texture->Load("Textures/EnemyTankSheet.PNG", false);
	_tank->posRect = new Rect(1000.0f, 450.0f, 30, 30);
	_tank->sourceRect = new Rect(0.0f, 0.0f, 30, 30);

	_ammo->texture = new Texture2D();
	_ammo->texture->Load("Textures/BulletSpriteSheet.PNG", false);
	_ammo->posRect = new Rect(-10.0f, -10.0f, 25, 25);
	_ammo->sourceRect = new Rect(0.0f, 0.0f, 25, 25);

	_health->texture = new Texture2D();
	_health->texture->Load("Textures/Health.PNG", false);
	_health->posRect = new Rect(-10.0f, -10.0f, 25, 25);
	_health->sourceRect = new Rect(0.0f, 0.0f, 25, 25);

	_explode = new Texture2D();
	_explode->Load("Textures/explode.PNG", false);

	Texture2D* bulletTex = new Texture2D();
	bulletTex->Load("Textures/Munchie.TGA", true);
	Texture2D* turretBulletTex = new Texture2D();
	turretBulletTex->Load("Textures/MunchieInverted.TGA", true); 

	for(int i = 0; i < BULLETS; i++)
	{
		_bullet[i]->texture = bulletTex;
		_bullet[i]->posRect = new Rect(_player->posRect->X,_player->posRect->Y,12,12);
		_bullet[i]->sourceRect = new Rect(0,0,12,12);
		_turretBullet[i]->texture = turretBulletTex;
		_turretBullet[i]->posRect = new Rect(0.0f,0.0f,12,12);
		_turretBullet[i]->sourceRect = new Rect(0,0,12,12);
		_tankBullet[i]->texture = turretBulletTex;
		_tankBullet[i]->posRect = new Rect(0.0f,0.0f,12,12);

}

	// Menu Screen Graphics
	_gameLogo = new Texture2D();
	_gameLogo->Load("Textures/Logo.PNG", false);
	_gameLogoRect = new Rect(Graphics::GetViewportWidth()/2-268, 50.0f,536,102);
	_gameModesLogo = new Texture2D();
	_gameModesLogo->Load("Textures/gameModes.PNG", false);
	_gameModesLogoRect = new Rect(Graphics::GetViewportWidth()/2-80, 300.0f,200,200);
	_pointer = new Texture2D();
	_pointer->Load("Textures/Pointer.PNG", false);
	_pointerRect = new Rect(Graphics::GetViewportWidth()/2-180, 310.0f,100,60);
	_gameOver = new Texture2D();
	_gameOver->Load("Textures/GameOver.PNG", false);
	_gameOverRect = new Rect(Graphics::GetViewportWidth()/2-385, Graphics::GetViewportHeight()/2-400,800,400);

	_background = new Texture2D();
	_background->Load("Textures/Grass.PNG", false);
	_backRect = new Rect(0.0f, 0.0f,Graphics::GetViewportWidth(),Graphics::GetViewportHeight());

	_shot = new SoundEffect();
	_shot->Load("Sounds/TankFiring.wav");
	_reload = new SoundEffect();
	_reload->Load("Sounds/WeaponCocking.wav");
	_explosion = new SoundEffect();
	_explosion->Load("Sounds/Explosion.wav");
	_viewportCollision = new SoundEffect();
	_viewportCollision->Load("Sounds/WallCollision.wav");
	_bulletCollision = new SoundEffect();
	_bulletCollision->Load("Sounds/BulletCollision.wav");
	_enemyShot = new SoundEffect();
	_enemyShot->Load("Sounds/EnemyFiring.wav");
	_heal = new SoundEffect();
	_heal->Load("Sounds/Health.wav");
	

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
	_pausedStringPos = new Vector2(Graphics::GetViewportWidth()/2, Graphics::GetViewportHeight()/2);
	_healthStringPos = new Vector2(Graphics::GetViewportWidth()/2, 25.0f);
	_ammoStringPos = new Vector2(Graphics::GetViewportWidth()-100, 25.0f);
	_resetStringPos = new Vector2(Graphics::GetViewportWidth()/2 - 150, Graphics::GetViewportHeight()/2);
	_AISoundPos = new Vector2(Graphics::GetViewportWidth()/2 - 60, Graphics::GetViewportHeight()/2+ 200);
	_turretHealthStringPos = new Vector2(10, Graphics::GetViewportHeight() - 25);
	_tankHealthStringPos = new Vector2(Graphics::GetViewportWidth()-80, Graphics::GetViewportHeight() - 25);
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();
	switch(currentGameState)
	{
	case gameState::StartMenu:
		MenuInput(keyboardState);
		LoadGameVariables();
			return;
	case gameState::SinglePlayer:
			CheckPaused(keyboardState, Input::Keys::P);
			
			if(!_paused)
			{
				Input(elapsedTime, keyboardState, mouseState);
				if(_player->isAlive)
				{
					UpdatePlayer(elapsedTime);
					PlayerViewPortCollision();
					UpdateBullet(elapsedTime);
					if(_turret->life > 0)
					{
						if(CheckPlayerRange(_turret->posRect->X-250, _turret->posRect->Y-250, _turret->posRect->Height+530, _turret->posRect->Width+530))
						{
							_turret->currentTime += (1.0f/60);
							if (_turret->currentTime > _turret->fireRate)
							{
								TurretShooting(elapsedTime);
								_turret->currentTime = 0.0;
							}
						}
						else
						{
							_turret->currentTime = 1.0;
						}
					}
					if(_tank->life > 0)
					{
						UpdateAI(elapsedTime);
						MoveAI(elapsedTime);
						AiViewPortCollision();

						if(CheckPlayerRange(_tank->posRect->X-250, _tank->posRect->Y-250, _tank->posRect->Height+530, _tank->posRect->Width+530))
						{
							_tank->patrol = false;

							_tank->currentTime += (1.0f/60);
							if (_tank->currentTime > _tank->fireRate)
							{
								AiShooting(elapsedTime);
								_tank->currentTime = 0;
							}
						}
						else
						{
							_tank->patrol = true;
							_tank->currentTime = 0.0;
						}
						
					}
					UpdateTurretBullet(elapsedTime);
					UpdateTurret(elapsedTime);
					for (int i = 0; i < BULLETS; i++)
					{
						UpdateTankBullet(elapsedTime, i);
					}
					AiDeath();
				}
				PlayerDeath();
				if(_player->ammo < BULLETS)
				{
					if(PlayerAmmoCollision() && _ammo->spawn)
					{
						_ammo->posRect->X = -200;
						_ammo->posRect->Y = -200;
						_ammo->collected = true;
						_player->ammo += 3;
						if(_player->ammo > BULLETS)
						{
							_player->ammo = 25;
						}
						_ammo->spawn = false;
					}
				}
				if(_ammo->collected)
				{
					SpawnAmmo(elapsedTime);
				}
			}
			if(_player->life < 3)
			{
				if(PlayerHealthCollision() && _health->spawn)
				{
					_health->collected = true;
					_player->life += 1;
					if (currentGameDifficulty == Hard)
						_player->life = 3;
					_health->spawn = false;
					_health->posRect->X = -200;
					_health->posRect->Y = -200;
				}
			}
			if(_health->collected)
			{
				SpawnHealth();
			}
			HealthAnimation();
			AmmoAnimation();
			return;
		case gameState::GameOver:
			MenuInput(keyboardState);
			return;
		case gameState::ScoreTable:
			if(!_sortScore)
			{
				SortScore();
			}
			MenuInput(keyboardState);
			return;
	}
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	//stream << "Pacman X: " << _player->posRect->X << " Y: " << _player->posRect->Y;
	stream << "Game Duration: " << (int)_gameTime;
	std::stringstream health;
	health << "Health: " << _player->life;
	std::stringstream turretHealth;
	turretHealth << "Turret: " << _turret->life;
	std::stringstream tankHealth;
	tankHealth << "Tank: " << _tank->life;
	std::stringstream ammo;
	ammo << "Ammo: " << _player->ammo;
	std::stringstream menu;
	menu << "PAUSED!";
	std::stringstream Reset;
	Reset << "Press SPACE to return to start menu";
	std::stringstream AISound;
	AISound << _AISoundStr;
	
	switch(currentGameState)
	{
	case gameState::StartMenu:
			SpriteBatch::BeginDraw();
			SpriteBatch::Draw(_background, _backRect, nullptr);
			SpriteBatch::Draw(_gameLogo, _gameLogoRect, nullptr);
			SpriteBatch::Draw(_gameModesLogo, _gameModesLogoRect, nullptr);
			SpriteBatch::Draw(_pointer, _pointerRect, nullptr);
			SpriteBatch::DrawString(AISound.str().c_str(), _AISoundPos, Color::Red);
			SpriteBatch::EndDraw();
			return;
		case gameState::SinglePlayer:
			SpriteBatch::BeginDraw(); 
			SpriteBatch::Draw(_background, _backRect, nullptr);
			if(_paused)
			{
				SpriteBatch::DrawString(menu.str().c_str(), _pausedStringPos, Color::Red);
			}
			if(_turret->life <= 0)
			{
				SpriteBatch::Draw(_explode, _turret->posRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
			}
			else
			{
				SpriteBatch::Draw(_turret->texture, _turret->posRect, _turret->sourceRect);
			}
			if(_tank->life <= 0)
			{
				SpriteBatch::Draw(_explode, _tank->posRect, _tank->sourceRect);
			}
			else
			{
				SpriteBatch::Draw(_tank->texture, _tank->posRect, _tank->sourceRect);
			}
			
			for(int i = 0; i < BULLETS; i++)
			{
				if(_bullet[i]->shot)
				{
					SpriteBatch::Draw(_bullet[i]->texture,_bullet[i]->posRect, _bullet[i]->sourceRect); 
				}
				if(_turretBullet[i]->shot)
				{
					SpriteBatch::Draw(_turretBullet[i]->texture,_turretBullet[i]->posRect, _turretBullet[i]->sourceRect); 
				}
				if(_tankBullet[i]->shot)
				{
					SpriteBatch::Draw(_tankBullet[i]->texture,_tankBullet[i]->posRect, _tankBullet[i]->sourceRect); 
				}
			}
			if(_player->life <= 0)
			{
				if(_gameTime <= 0.6)
				{
					SpriteBatch::Draw(_explode, _player->posRect, _player->sourceRect);
				}
			}
			else
			{
				SpriteBatch::Draw(_player->texture, _player->posRect, _player->sourceRect);
			}
			if(_ammo->spawn)
			{
				SpriteBatch::Draw(_ammo->texture, _ammo->posRect, _ammo->sourceRect);
			}
			if(_health->spawn)
			{
				SpriteBatch::Draw(_health->texture, _health->posRect, _health->sourceRect);
			}
			// Draws String
			SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
			SpriteBatch::DrawString(health.str().c_str(), _healthStringPos, Color::Blue);
			SpriteBatch::DrawString(turretHealth.str().c_str(), _turretHealthStringPos, Color::Red);
			SpriteBatch::DrawString(tankHealth.str().c_str(), _tankHealthStringPos, Color::Red);
			SpriteBatch::DrawString(ammo.str().c_str(), _ammoStringPos, Color::Blue);
			SpriteBatch::EndDraw();
			return;
		case gameState::GameOver:
			SpriteBatch::BeginDraw();
			SpriteBatch::Draw(_gameOver, _gameOverRect, nullptr);
			SpriteBatch::DrawString(Reset.str().c_str(), _resetStringPos, Color::Red);
			SpriteBatch::EndDraw();
			return;
		case gameState::ScoreTable:
			SpriteBatch::BeginDraw();
			for (int i = 0; i < 5; i++)
			{
				std::stringstream Score;
				Score << _scores[i]->playerName << "............................" << _scores[i]->difficulty << "............................" << _scores[i]->time;
				_scores[i]->position->Y = (i*50)+50;
				SpriteBatch::DrawString(Score.str().c_str(), _scores[i]->position, Color::Red);
			}
			SpriteBatch::DrawString(Reset.str().c_str(), _resetStringPos, Color::Red);
			SpriteBatch::EndDraw();
			return;
	}
}
void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{

	if(state->IsKeyDown(Input::Keys::D))
	{
		currentDirection = Right;
	}
	else if(state->IsKeyDown(Input::Keys::A))
	{
		currentDirection = Left;
	}
	else if(state->IsKeyDown(Input::Keys::W))
	{
		currentDirection = Up;
	}
	else if(state->IsKeyDown(Input::Keys::S))
	{
		currentDirection = Down;
	}
	else
	{
		currentDirection = None;
	}

	// speed multiplier
	if(state->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		_player->speedMultiplier = 3.0f;
	}
	else
	{
		_player->speedMultiplier = 1.5f;
	}

	// player shooting
	if(state->IsKeyDown(Input::Keys::B))
	{
		while(_player->ammo > 0 && (!_playerShot))
		{
			_playerShot = true;
			PlayerShooting(_playerShot);
			Audio::Play(_shot);
			
		}
	}
	if(state->IsKeyUp(Input::Keys::B))
	{
		_playerShot = false;
	}

}

void Pacman::UpdatePlayer(int elapsedTime)
{
	// Changes sprite sheet to face player direction
	_player->sourceRect->Y = _player->sourceRect->Width * _player->direction;

	float _playerSpeed = _cPlayerSpeed * elapsedTime * _player->speedMultiplier;

	switch (currentDirection)
	{
		case direction::Up:
			_player->posRect->Y -= _playerSpeed;
			_player->direction = 1;
			break;
		case direction::Down:
			_player->posRect->Y += _playerSpeed;
			_player->direction = 3;
			break;
		case direction::Left:
			_player->posRect->X -= _playerSpeed;
			_player->direction = 2;
			break;
		case direction::Right:
			_player->posRect->X += _playerSpeed;
			_player->direction = 0;
			break;
		case direction::None:
			_player->posRect->X = _player->posRect->X;
			_player->posRect->Y = _player->posRect->Y;
	}

}
void Pacman::PlayerViewPortCollision()
{
	if(_player->posRect->X + _player->sourceRect->Width > Graphics::GetViewportWidth())
	{
		_player->posRect->X = Graphics::GetViewportWidth() - _player->sourceRect->Width;
		Audio::Play(_viewportCollision);
	}
	if(_player->posRect->X < 0)
	{
		_player->posRect->X = 0;
		Audio::Play(_viewportCollision);
	}
	if(_player->posRect->Y + _player->sourceRect->Height > Graphics::GetViewportHeight())
	{
		_player->posRect->Y = Graphics::GetViewportHeight() - _player->sourceRect->Height;
		Audio::Play(_viewportCollision);
	}
	if(_player->posRect->Y < 0)
	{
		_player->posRect->Y = 0;
		Audio::Play(_viewportCollision);
	}
}
void Pacman::PlayerShooting(bool _playerShot)
{
	while(_player->ammo > 0 && _playerShot == true)
		{
			_playerShot = false;
			for(int i = 0; i<BULLETS; i++)
			{
				if(_bullet[i]->shot != true)
				{
					_bullet[i]->direction = _player->direction;
					if(_bullet[i]->direction == 0)
					{
						_bullet[i]->posRect->X = _player->posRect->X + _player->posRect->Width/3;
						_bullet[i]->posRect->Y = _player->posRect->Y + _player->posRect->Height/3.5;
					}
					if(_bullet[i]->direction == 1)
					{
						_bullet[i]->posRect->X = _player->posRect->X + _player->posRect->Width/3.5;
						_bullet[i]->posRect->Y = _player->posRect->Y;
					}
					if(_bullet[i]->direction == 2)
					{
						_bullet[i]->posRect->X = _player->posRect->X;
						_bullet[i]->posRect->Y = _player->posRect->Y + _player->posRect->Height /3.5;
					}
					if(_bullet[i]->direction == 3)
					{
						_bullet[i]->posRect->X = _player->posRect->X + _player->posRect->Width / 3.5;
						_bullet[i]->posRect->Y = _player->posRect->Y + _player->posRect->Height/3;
					}
					_bullet[i]->shot = true;
					_player->ammo--;
					break;
				}
			}
		}
	
}
void Pacman::UpdateBullet(int elapsedTime)
{
	float _bulletSpeed = _cPlayerSpeed * elapsedTime * 2.0;
	if(currentGameDifficulty == Hard)
		_bulletSpeed *= 2.0;

	for(int i = 0; i<BULLETS; i++)
	{
		//bullet movement
		if(_bullet[i]->direction == 0) // right
		{
			_bullet[i]->posRect->X += _bulletSpeed;
		}
		else if(_bullet[i]->direction == 1) // up
		{
			_bullet[i]->posRect->Y -= _bulletSpeed;
		}
		else if(_bullet[i]->direction == 2) // left
		{
			_bullet[i]->posRect->X -= _bulletSpeed;
		}
		else if(_bullet[i]->direction == 3) // down
		{
			_bullet[i]->posRect->Y += _bulletSpeed;
		}

		// bullet off screen
		if(BulletViewPortCollision(_bullet[i]->posRect->X, _bullet[i]->posRect->Y, _bullet[i]->posRect->Height, _bullet[i]->posRect->Width))
		{
			_bullet[i]->shot = false;
		}
		
		//bullet collision with tank
		if(PlayerBulletCollision(i, _tank->posRect->X, _tank->posRect->Y, _tank->posRect->Height, _tank->posRect->Width) && _bullet[i]->shot)
		{
			_bullet[i]->shot = false;
			_tank->life --;
		}
		
		// bullet collision with turret
		
		if(PlayerBulletCollision(i, _turret->posRect->X, _turret->posRect->Y, _turret->posRect->Height, _turret->posRect->Width) && _bullet[i]->shot)
		{
			_bullet[i]->shot = false;
			if(CheckPlayerRange(_turret->posRect->X-250, _turret->posRect->Y-250, _turret->posRect->Height+530, _turret->posRect->Width+530))
			{
				_turret->life --;
			}
		}
	}
	
}
bool Pacman::PlayerBulletCollision(int i, float X, float Y, float height, float width)
{
	if(_bullet[i]->posRect->X > X + width)
	{
		return false;
	}
	if(_bullet[i]->posRect->X + _bullet[i]->posRect->Width < X)
	{
		return false;
	}
	if(_bullet[i]->posRect->Y > Y + height)
	{
		return false;
	}
	if(_bullet[i]->posRect->Y + _bullet[i]->posRect->Height < Y)
	{
		return false;
	}
	else
	{
		Audio::Play(_bulletCollision);
		return true;
	}
}

void Pacman::UpdateAI(int elapsedTime)
{
	_tank->sourceRect->Y = _tank->sourceRect->Width * _tank->direction;

	_AiMoveTimer +=(1.0f/60);

	if(_tank->patrol)
	{
		if(_AiMoveTimer > 3.0)
		{
			_tank->direction = rand() %4;
			AiChangeDirection();
			_AiMoveTimer = 0;
		}
	}
	

	if(!_tank->patrol)
	{
			
		if(CheckInLine(_tank->posRect->X+15, _player->posRect->X, 0, _player->posRect->Width) && CheckInLine(_tank->posRect->Y+15, _player->posRect->Y, 0, _player->posRect->Height))
		{
			_tank->currentTime += (1.0f/60);			
			AiDirection = None;
		}
		else if(!CheckInLine(_tank->posRect->X+15, _player->posRect->X, 0, _player->posRect->Width) && _AiMoveTimer < 2.5)
			{
				if(_tank->posRect->X  > _player->posRect->X)
					{
						AiDirection = Left;
					}
					else
					{
						AiDirection = Right;
					}
				if(CheckInLine(_tank->posRect->X+15, _player->posRect->X, 0, _player->posRect->Width))
				{
					_AiMoveTimer =2.5;
				}
			}
			else if(!CheckInLine(_tank->posRect->Y+15, _player->posRect->Y, 0, _player->posRect->Height))
			{
				
				if(_tank->posRect->Y > _player->posRect->Y)
					{
						AiDirection = Up;
					}
					else
					{
						AiDirection = Down;
					}
				if(CheckInLine(_tank->posRect->Y+15, _player->posRect->Y, 0, _player->posRect->Height))
				{
					_AiMoveTimer = 0;
				}
			}
		if(_AiMoveTimer > 3.5)
		{
			_AiMoveTimer = 0;	
		}

		UpdateAIFireDirection();
	}
}
void Pacman::MoveAI(int elapsedTime)
{
	float _enemySpeed = _cPlayerSpeed * elapsedTime;
	if(currentGameDifficulty == Hard)
		_enemySpeed *= 1.5;

		switch (AiDirection)
	{
		case direction::Up:
			_tank->posRect->Y -= _enemySpeed;
			_tank->direction = 1;
			break;
		case direction::Down:
			_tank->posRect->Y += _enemySpeed;
			_tank->direction = 3;
			break;
		case direction::Left:
			_tank->posRect->X -= _enemySpeed;
			_tank->direction = 2;
			break;
		case direction::Right:
			_tank->posRect->X += _enemySpeed;
			_tank->direction = 0;
			break;
		case direction::None:
			_tank->posRect->X = _tank->posRect->X;
			_tank->posRect->Y = _tank->posRect->Y;
	}
}
void Pacman::AiViewPortCollision()
{
	if(_tank->posRect->X + _tank->sourceRect->Width > Graphics::GetViewportWidth())
	{
		while(_tank->direction == 0)
		{
			_tank->direction = rand() % 4;
			AiChangeDirection();
		}
		_tank->posRect->X = Graphics::GetViewportWidth() - _tank->sourceRect->Width;
	}
	if(_tank->posRect->X < 0)
	{
		while(_tank->direction == 2)
		{
			_tank->direction = rand() % 4;
			AiChangeDirection();
		}
		_tank->posRect->X = 0;
	}
	if(_tank->posRect->Y + _tank->sourceRect->Height > Graphics::GetViewportHeight())
	{
		while(_tank->direction == 3)
		{
			_tank->direction = rand() % 4;
			AiChangeDirection();
		}
		_tank->posRect->Y = Graphics::GetViewportHeight() - _tank->sourceRect->Height;
	}
	if(_tank->posRect->Y < 0)
	{
		while(_tank->direction == 1)
		{
			_tank->direction = rand() % 4;
			AiChangeDirection();
		}
		_tank->posRect->Y = 0;
	}
}
void Pacman::AiChangeDirection()
{
	if(_tank->direction == 0)
	{
		AiDirection = Right;
	}
	if(_tank->direction == 1)
	{
		AiDirection = Up;
	}
	if(_tank->direction == 2)
	{
		AiDirection = Left;
	}
	if(_tank->direction == 3)
	{
		AiDirection = Down;
	}
}
void Pacman::AiShooting(int elapsedTime)
{
	if(CheckInLine(_tank->posRect->X+15, _player->posRect->X+15, _tank->posRect->Width-15, _player->posRect->Width-15) || CheckInLine(_tank->posRect->Y+15, _player->posRect->Y+15, _tank->posRect->Height-15, _player->posRect->Height-15))
	{
		for (int i = 0; i < BULLETS; i++)
		{
			if(!_tankBullet[i]->shot)
			{
				_tankBullet[i]->direction = _tank->direction;
				if(AiDirection == None)
				{
					AiDirection = previousAIDirection;
				}
				if(AiDirection == Up)
				{
					_tankBullet[i]->posRect->X = _tank->posRect->X + _tank->posRect->Width/3.5;
					_tankBullet[i]->posRect->Y = _tank->posRect->Y;
				}
				if(AiDirection == Down)
				{
					_tankBullet[i]->posRect->X = _tank->posRect->X + _tank->posRect->Width/3.5;
					_tankBullet[i]->posRect->Y = _tank->posRect->Y + _tank->posRect->Height/3.5;

				}
				if(AiDirection == Left)
				{
					_tankBullet[i]->posRect->X = _tank->posRect->X;
					_tankBullet[i]->posRect->Y = _tank->posRect->Y + _tank->posRect->Height/3.5;

				}
				if(AiDirection == Right)
				{
					_tankBullet[i]->posRect->X = _tank->posRect->X + _tank->posRect->Width;
					_tankBullet[i]->posRect->Y = _tank->posRect->Y + _tank->posRect->Height/3.5;

				}
				if(AiDirection == previousAIDirection)
				{
					AiDirection = currentAIDirection;
				}

				_tankBullet[i]->shot = true;
				if(_AISound)
					Audio::Play(_enemyShot);
				break;	
			}
					
		}
	}

}
void Pacman::UpdateAIFireDirection()
{
	if(currentAIDirection != AiDirection)
	{
		previousAIDirection = currentAIDirection;
		currentAIDirection = AiDirection;

	}
	
}
void Pacman::UpdateTankBullet(int elapsedTime, int i)
{
	float _bulletSpeed = _cPlayerSpeed * elapsedTime * 2.0;
	if(currentGameDifficulty == Hard)
		_bulletSpeed *= 1.5;

		if(_tankBullet[i]->direction == 0) // right
		{
			_tankBullet[i]->posRect->X += _bulletSpeed;
		}
		else if(_tankBullet[i]->direction == 1) // up
		{
			_tankBullet[i]->posRect->Y -= _bulletSpeed;
		}
		else if(_tankBullet[i]->direction == 2) // left
		{
			_tankBullet[i]->posRect->X -= _bulletSpeed;
		}
		else if(_tankBullet[i]->direction == 3) // down
		{
			_tankBullet[i]->posRect->Y += _bulletSpeed;
		}

		// tank bullet collision

		if(_tankBullet[i]->shot == true)
		{
			// Bullet goes off screen
			if(BulletViewPortCollision(_tankBullet[i]->posRect->X, _tankBullet[i]->posRect->Y, _tankBullet[i]->posRect->Height, _tankBullet[i]->posRect->Width))
			{
				_tankBullet[i]->shot = false;
			}

			// tank bullet collision with player
			if(BulletCollision(i,_tankBullet[i]->posRect->X, _tankBullet[i]->posRect->Y, _tankBullet[i]->posRect->Height, _tankBullet[i]->posRect->Width))
			{
				_tankBullet[i]->shot = false;
				_player->life--;
			}
		}
}
bool Pacman::CheckInLine(float aX, float pX, float aDim, float pDim)
{
	// Check if AI (co-ord aX and dimenion aDim) in line with player (pX and pDim)
	if (aX + aDim < pX)
		return false;
	if (aX > pX + pDim)
		return false;
	else
		return true;
}

void Pacman::UpdateTurret(int elapsedTime)
{
	int x = _turret->posRect->X - _player->posRect->X;
	int y = _turret->posRect->Y - _player->posRect->Y;
	_turret->sourceRect->Y = _turret->sourceRect->Width * _turret->direction;

	if (x > 0)
	{
		if (y > 0)
		{
			_turret->direction = 0;
		}
		else
		{
			_turret->direction = 3;
		}

	}
	else
	{
		if (y > 0)
		{
			_turret->direction = 1;
		}
		else
		{
			_turret->direction = 2;
		}
	}

}
void Pacman::UpdateTurretBullet(int elapsedTime)
{
	float _bulletSpeed = _cPlayerSpeed * elapsedTime * 2.0;
	if(currentGameDifficulty == Hard)
		_bulletSpeed *= 1.5;
	
	// move turret bullet
	for(int i = 0; i<BULLETS; i++)
	{
			_turretBullet[i]->posRect->X += _turretBullet[i]->dir->X*_bulletSpeed;
			_turretBullet[i]->posRect->Y += _turretBullet[i]->dir->Y * _bulletSpeed;
	}
		
	// turret bullet collision
	for(int i = 0; i<BULLETS; i++)
	{
		if(_turretBullet[i]->shot == true)
		{
			// Bullet off screen
			if(BulletViewPortCollision(_turretBullet[i]->posRect->X, _turretBullet[i]->posRect->Y, _turretBullet[i]->posRect->Height, _turretBullet[i]->posRect->Width))
			{
				_turretBullet[i]->shot = false;
			}

			// turret bullet collision with player
			if(BulletCollision(i, _turretBullet[i]->posRect->X, _turretBullet[i]->posRect->Y, _turretBullet[i]->posRect->Height, _turretBullet[i]->posRect->Width))
			{
				_turretBullet[i]->shot = false;
				_player->life--;
			}
		}
	}
}
void Pacman::TurretShooting(int elapsedTime)
{
	for (int i = 0; i < BULLETS; i++)
	{
		if(!_turretBullet[i]->shot)
		{
			if(_turret->direction == 0)
			{
				_turretBullet[i]->posRect->X = _turret->posRect->X;
				_turretBullet[i]->posRect->Y = _turret->posRect->Y;
			}
			if(_turret->direction == 1)
			{
				_turretBullet[i]->posRect->X = _turret->posRect->X + _turret->posRect->Width/1.4;
				_turretBullet[i]->posRect->Y = _turret->posRect->Y;
			}
			if(_turret->direction == 2)
			{
				_turretBullet[i]->posRect->X = _turret->posRect->X + _turret->posRect->Width/1.4;
				_turretBullet[i]->posRect->Y = _turret->posRect->Y + _turret->posRect->Height/1.4;
			}
			if(_turret->direction == 3)
			{
				_turretBullet[i]->posRect->X = _turret->posRect->X;
				_turretBullet[i]->posRect->Y = _turret->posRect->Y + _turret->posRect->Height/1.4;
			}
			_turretBullet[i]->dir->X = _player->posRect->X - _turret->posRect->X; 
			_turretBullet[i]->dir->Y = _player->posRect->Y - _turret->posRect->Y;
			_turretBullet[i]->dir->Normalize();
			
			_turretBullet[i]->shot = true;
			if(_AISound)
				Audio::Play(_enemyShot);
			break;
		}
						
	}
}

bool Pacman::BulletCollision(int i, float X, float Y, float height, float width)
{
	if(X > _player->posRect->X + _player->posRect->Width)
	{
		return false;
	}
	if(X + width < _player->posRect->X)
	{
		return false;
	}
	if(Y > _player->posRect->Y + _player->posRect->Height)
	{
		return false;
	}
	if(Y + height < _player->posRect->Y)
	{
		return false;
	}
	else
	{
		Audio::Play(_bulletCollision);
		return true;
	}
}
bool Pacman::CheckPlayerRange(float X, float Y, float height, float width)
{
	if(X > _player->posRect->X + _player->posRect->Width)
	{
		return false;
	}
	if(X + width < _player->posRect->X)
	{
		return false;
	}
	if(Y > _player->posRect->Y + _player->posRect->Height)
	{
		return false;
	}
	if(Y + height < _player->posRect->Y)
	{
		
		return false;
	}
	else
		return true;
}
bool Pacman::BulletViewPortCollision(float X, float Y, float Height, float Width)
{
	if(X + Width > Graphics::GetViewportWidth())
	{
		return true;
	}
	if(X < 0)
	{
		return true;
	}
	if(Y + Height > Graphics::GetViewportHeight())
	{
		return true;
	}
	if(Y < 0)
	{
		return true;
	}
	else
		return false;
}
void Pacman::PlayerDeath()
{
	if (_player->life <= 0)
	{
		
		if(_player->isAlive)
		{
			Audio::Play(_explosion);
			_gameTime = 0;
		}
		_player->isAlive = false;
		_gameTime += (1.0f/60);
			
		if(_gameTime >= _timer)
		{
			_scores[_scoreUsed]->played = false;
			currentGameState = GameOver;
		}
	}
	else
	{
		GameTime();
	}
}
void Pacman::AiDeath()
{
	if(_turret->life <= 0 && _turret->posRect->X != -400)
	{
		if(_AiRate == 0)
			Audio::Play(_explosion);
		_AiRate += (1.0f/60);
		if (_AiRate >= 0.5)
		{
			_turret->posRect->X = -400;
			_turret->posRect->Y = -400;
			_AiRate = 0;
			_enemiesRemaining--;
		}
	}
	if(_tank->life <= 0 && _tank->posRect->X != -400)
	{
		if(_AiRate == 0)
			Audio::Play(_explosion);

		_AiRate += (1.0f/60);
		if (_AiRate >= 0.5)
		{
			_tank->posRect->X = -400;
			_tank->posRect->Y = -400;
			_AiRate = 0;
			_enemiesRemaining--;	
		}
	}
	if(_enemiesRemaining == 0)
	{
		if(_scores[_scoreUsed]->played)
		{
			if(_scores[_scoreUsed]->time > _gameTime || _scores[_scoreUsed]->time == 0)
				_scores[_scoreUsed]->time = _gameTime;
			if (currentGameDifficulty == Hard)
				_scores[_scoreUsed]->time /= 4;
			if(_scoreUsed == 4)
				_usedScores = true;
			_scores[_scoreUsed]->played = false;
		}
		GameTime();
					
		if(_gameTime >= _timer)
		{
			currentGameState = GameOver;
		}
	}
}

void Pacman::SpawnAmmo(int elapsedTime)
{
	if(!_ammo->spawn)
	{
		_ammo->spawnTimer += (1.0f/60);

		if(_ammo->spawnTimer > _ammo->spawnRate)
		{
			_ammo->spawn = true;
			_ammo->collected = false;
			_ammo->posRect->X = rand() % Graphics::GetViewportWidth();
			_ammo->posRect->Y = rand() % Graphics::GetViewportHeight();
			// prevent ammo spawning partly on the screen
			if(_ammo->posRect->X + _ammo->posRect->Width > Graphics::GetViewportWidth())
				_ammo->posRect->X -= _ammo->posRect->Width;
			if(_ammo->posRect->Y + _ammo->posRect->Height > Graphics::GetViewportHeight())
				_ammo->posRect->Y -= _ammo->posRect->Height;
			_ammo->spawnTimer = 0;
		}
	}
}
bool Pacman::PlayerAmmoCollision()
{
	if(_player->posRect->X + _player->sourceRect->Width < _ammo->posRect->X)
	{
		return false;
	}
	if(_player->posRect->X > _ammo->posRect->X + _ammo->posRect->Width)
	{
		return false;
	}
	if(_player->posRect->Y + _player->sourceRect->Height < _ammo->posRect->Y)
	{
		return false;
	}
	if(_player->posRect->Y > _ammo->posRect->Y + _ammo->posRect->Height)
	{
		return false;
	}
	else
	{
		Audio::Play(_reload);
		return true;
	}
		
}
void Pacman::AmmoAnimation()
{
	_ammo->sourceRect->Y = _ammo->sourceRect->Height * _ammo->currentFrame;

	_ammo->currentFrameRate += (1.0f/60);

	if(_ammo->currentFrameRate > 1.0 && _ammo->currentFrameRate < 2.0)
		_ammo->currentFrame = 1;
	if(_ammo->currentFrameRate > 2.0 && _ammo->currentFrameRate < 3.0)
		_ammo->currentFrame = 2;
	if(_ammo->currentFrameRate > 3.0 && _ammo->currentFrameRate < 4.0)
		_ammo->currentFrame = 3;

	if (_ammo->currentFrameRate > _ammo->frameRate)
	{
		_ammo->currentFrameRate = 0;
		_ammo->currentFrame = 0;
	}
}

void Pacman::SpawnHealth()
{
	if(!_health->spawn)
	{
		_health->spawnTimer += (1.0f/60);

		if(_health->spawnTimer > _health->spawnRate)
		{
			_health->spawn = true;
			_health->collected = false;
			_health->posRect->X = rand() % Graphics::GetViewportWidth();
			_health->posRect->Y = rand() % Graphics::GetViewportHeight();
			// prevent ammo spawning partly on the screen
			if(_health->posRect->X + _health->posRect->Width > Graphics::GetViewportWidth())
				_health->posRect->X -= _health->posRect->Width;
			if(_health->posRect->Y + _health->posRect->Height > Graphics::GetViewportHeight())
				_health->posRect->Y -= _health->posRect->Height;
			_health->spawnTimer = 0;
		}
	}
}
bool Pacman::PlayerHealthCollision()
{
	if(_player->posRect->X + _player->sourceRect->Width < _health->posRect->X)
	{
		return false;
	}
	if(_player->posRect->X > _health->posRect->X + _health->posRect->Width)
	{
		return false;
	}
	if(_player->posRect->Y + _player->sourceRect->Height < _health->posRect->Y)
	{
		return false;
	}
	if(_player->posRect->Y > _health->posRect->Y + _health->posRect->Height)
	{
		return false;
	}
	else
	{
		Audio::Play(_heal);
		return true;
	}
}
void Pacman::HealthAnimation()
{
		_health->sourceRect->Y = _health->sourceRect->Height * _health->currentFrame;

	_health->currentFrameRate += (1.0f/60);

	
	if (_health->currentFrameRate > _health->frameRate)
	{
		_health->currentFrame++;
		if(_health->currentFrame >= 2)
		{
			_health->currentFrame = 0;
		}
		_health->currentFrameRate = 0;
		
	}
}

void Pacman::MenuInput(Input::KeyboardState* state)
{
	if(currentGameState == StartMenu)
	{
		if(state->IsKeyDown(Input::Keys::DOWN) && !_isKeyDown)
		{
			_pointerPos++;
			_isKeyDown = true;
		}
		if(state->IsKeyDown(Input::Keys::UP) && !_isKeyDown)
		{
			_pointerPos--;
			_isKeyDown = true;
		}
		if (state->IsKeyUp(Input::Keys::DOWN) || state->IsKeyUp(Input::Keys::UP) && !state->IsKeyDown(Input::Keys::DOWN) && !state->IsKeyDown(Input::Keys::UP))
			_isKeyDown = false;
		if(state->IsKeyDown(Input::Keys::RETURN))
		{
			if(_pointerPos == 0)
			{
				currentGameState = SinglePlayer;
				ScoreSheet();
				currentGameDifficulty = Normal;
			}
			if(_pointerPos == 1)
			{
				currentGameState = SinglePlayer;
				ScoreSheet();
				currentGameDifficulty = Hard;
			}
			if(_pointerPos == 2)
				currentGameState = ScoreTable;
		}
	}
	if(currentGameState == GameOver || currentGameState == ScoreTable)
	{
		if(state->IsKeyDown(Input::Keys::SPACE))
		{
			currentGameState = StartMenu;
		}
	}

	if(_pointerPos == 0)
		_pointerRect->Y = 300;
	if(_pointerPos == 1)
		_pointerRect->Y = 360;
	if(_pointerPos == 2)
		_pointerRect->Y = 440;
	if(_pointerPos < 0)
		_pointerPos = 0;
	if(_pointerPos > 2)
		_pointerPos = 2;

	if(state->IsKeyDown(Input::Keys::LEFT))
		{
			_AISoundStr = "AI SOUND ON";
			_AISound = true;
		}
		if(state->IsKeyDown(Input::Keys::RIGHT))
		{
			_AISoundStr = "AI SOUND OFF";
			_AISound = false;
		}
}
void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	if (state->IsKeyDown(Input::Keys::P) && !_pKeyDown)
		{
			_pKeyDown = true;
			_paused = !_paused;
			
		}

		if (state->IsKeyUp(Input::Keys::P))
			_pKeyDown=false;
			
}

void Pacman::LoadGameVariables()
{
	_player->direction = 0;
	_player->ammo = 3;
	_player->life = 3;
	_player->isAlive = true;
	_player->posRect->X = 25;
	_player->posRect->Y = 25;

	_turret->direction = 0;
	_turret->currentTime = 1.0;
	_turret->posRect->X = 492;
	_turret->posRect->Y = 364;
	
	_tank->direction = 0;
	_tank->currentTime = 0.0;
	_tank->patrol = true;
	_tank->posRect->X = 800;
	_tank->posRect->Y = 450;

	_ammo->spawn = false;
	_ammo->spawnTimer = 0.0;
	_ammo->collected = true;
	_ammo->frameRate = 4.0;
	_ammo->currentFrameRate = 0.0;
	_ammo->currentFrame = 0;

	_health->spawn = false;
	_health->spawnTimer = 0.0;
	_health->collected = true;
	_health->frameRate = 0.5;
	_health->currentFrameRate = 0.0;
	_health->currentFrame = 0;

	for(int i = 0; i < BULLETS; i++)
	{
		_bullet[i]->shot = false;

	}
	for(int i = 0; i < BULLETS; i++)
	{
		_turretBullet[i]->shot = false;
		_tankBullet[i]->shot = false;
	}

	_playerShot = false;
	_enemiesRemaining = 2;
	_AiMoveTimer = 0.0;
	_timer = 3.0;
	_gameTime = 0.0;
	_AiRate = 0.0;
	_paused = false;
	_pKeyDown = false;
	_sortScore = false;
	AiDirection = Left;

	switch (currentGameDifficulty)
	{
		case gameDifficulty::Normal:
			_turret->fireRate = 1.0;
			_turret->life = 5;
			_tank->fireRate = 1.0;
			_tank->life = 3;
			_ammo->spawnRate = 3.0;
			_health->spawnRate = 5.0;
			return;
		case gameDifficulty::Hard:
			_turret->fireRate = 0.5;
			_turret->life = 7;
			_tank->fireRate = 0.5;
			_tank->life = 5;
			_ammo->spawnRate = 4.0;
			_health->spawnRate = 6.0;
				return;
	}
	
}
void Pacman::GameTime()
{
	_gameTime += (1.0f/60);
}
void Pacman::ScoreSheet()
{
	if(!_usedScores)
	{
		for (int i = 0; i < 5; i++)
		{
			if(!_scores[i]->played && _scores[i]->time == 0.0)
			{
				_scores[i]->played = true;
				_scoreUsed = i;
				if(_pointerPos == 0)
					_scores[i]->difficulty = "Normal";
				if(_pointerPos == 1)
					_scores[i]->difficulty = ".Hard.";
					break;
			}

		}
	}
	else
	{
		_scoreUsed = FindLargest(4);
		_scores[_scoreUsed]->played = true;
		if(_pointerPos == 1)
			_scores[_scoreUsed]->difficulty = ".Hard.";
	}
}
void Pacman::SortScore()
{
	for (int i = 1; i < 5; i++)
	{
		int lastElement = 5 - i;
		int largePos = FindLargest(lastElement);
		if (largePos != lastElement)
		{
			if(_scores[lastElement]->time != 0)
			{
				_tempScore = _scores[largePos];
				_scores[largePos] = _scores[lastElement];
				_scores[lastElement] = _tempScore;
			}
		}
	}
	_sortScore = true;
}
int Pacman::FindLargest(int last)
{
	int largestPos = 0;
	for (int i = 1; i <= last; i++)
	{
		if(_scores[i]->time > _scores[largestPos]->time && _scores[i]->time != 0)
		{
			largestPos = i;
		}
	}
	return largestPos;
}