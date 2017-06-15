#pragma once

#include <random>

#include "SnakeGame.h"
#include "App.h"
#include "AppInput.h"
#include "MapManager.h"
#include "Map.h"
#include "weakPtr.h"
#include "UI.h"
#include "GameEntity.h"

/// <summary> 
/// Enum of availible maps
/// </summary>
enum class SnakeGameMap
{
	MainMenu,
	PauseMenu,
	MainMap,
	GameOverMap
};

/// <summary> 
/// Represents an cell on the grid
/// </summary>
struct GridCell
{
	FVector2 pos;
};

enum LastKey
{
	Up, Down, Left, Right, None
};

/// <summary> 
/// Enum of availible snake directions
/// </summary>
enum class SnakeHeadDirection
{
	Up, Left, Right, Down, None
};


/// <summary> 
/// Reperesents index of an grid cell
/// </summary>
struct GridIndex
{
	bool operator==(const GridIndex& other)
	{
		return elem == other.elem && column == other.column;
	}

	bool operator!=(const GridIndex& other)
	{
		return elem != other.elem && column != other.column;
	}

	size_t elem = 0;
	size_t column = 0;
};

struct SnakeHead;

/// <summary> 
/// Reperesents child part of snake
/// </summary>
struct SnakeTailPart
{
	SnakeTailPart(GridIndex& index, SnakeHeadDirection& headDir, const std::string& name);
	//SnakeTailPart(SnakeTailPart* parta, const std::string& name);

	weakPtr<SpriteRenderer> part;

	GridIndex& otherPos;

	// Current position of this snake part
	GridIndex currentPos;

	// Last position of this snake part
	GridIndex lastPos;

	/// <summary> 
	/// name of this snake part
	/// </summary>
	std::string name;

	SnakeHeadDirection& dir;

	// is part spawned?
	bool spawned = false;

	// Update part per frame
	void Tick();
};


struct SnakeHead
{
	weakPtr<Map> map;
	weakPtr<SpriteRenderer> head;
	std::vector<SnakeTailPart> snakeParts;
	// Current head direction
	SnakeHeadDirection currentHeadDirection;
	// Last head direction
	SnakeHeadDirection lastHeadDir;
	// Current head position
	GridIndex currentPos;
	// Last head position
	GridIndex lastPos;
	GridCell dirCell;

	public:

	// Add new snake part to head
	void AddPart();

	// Update head per frame
	void Tick();
};

class SnakeGame
{
	public:

	// Singleton instance of this game
	static SnakeGame& Instance();

	// used by frame delay
	size_t frame;
	size_t frameDelay;
	std::random_device rd;

	// Map references
	weakPtr<Map> mainMenuMap;
	weakPtr<Map> mainMap;
	weakPtr<Map> pauseMenuMap;
	weakPtr<Map> gameOverMap;

	// UI
	weakPtr<UIImage> backgroundImg;
	weakPtr<UIImage> backgroundImgPause;
	weakPtr<UIImage> backgroundImgGameOver;
	weakPtr<UIButton> quitButtonPause;
	weakPtr<UIButton> quitButtonGameOver;
	weakPtr<UIButton> continueButton;
	weakPtr<UIButton> playAgainButton;
	weakPtr<UIButton> playGameButton;
	weakPtr<UIButton> creditsButton;
	weakPtr<UIButton> quitButton;
	weakPtr<UIImage> creditsImg;
	weakPtr<UIButton> creditsImgButton;
	weakPtr<SpriteRenderer> foodSprite;

	// Current food position
	GridIndex foodPos;

	// is credits image drawn?
	bool creditsImageDrawn;
	// are collisions enabled?
	bool collisionEnabled;
	/////////

	// Create grid
	void CreateGrid();
	// Draw grid on the screeen
	void DrawGrid();
	void SetRandomFoodPosition();

	// Grid blob
	std::vector<std::vector<GridCell>> grid;
	// head of snake
	SnakeHead snakeHead;
	// is game paused
	bool isPaused;
	// is game currently playign
	bool inGame;
	// is game began?
	bool gameBegan;
	// is game over?
	bool gameOver;
	// is help message drawn?
	bool helpMessageDrawn;
	// current score
	size_t score;
	// best score
	size_t bestScore = 0;
	// speed factor
	float speedModifier;
	// last key down
	LastKey lastKey;
	// is frame finished?
	bool frameFinished;

	public:

	// Initialize game
	void Initialize();
	// Create game maps
	void CreateMaps();
	// Set active map
	void SetActiveMap(SnakeGameMap map);
	// Update game per frame
	void Tick();
	// Render game perframe
	void Draw();
	// Render above all
	void DrawTopMost();
	void CreateMainMenuMap();
	void CreateMainMap();
	void CreatePauseMenuMap();
	void CreateGameOverMap();
	// create food
	void CreateFood();
	// create new tail part
	void CreateTailPart(SnakeTailPart& fromPart);
	// save score on disk
	void SaveScore();
};