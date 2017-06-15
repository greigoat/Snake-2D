#include "SnakeGame.h"
#include "AppGfx.h"
#include <iostream>
#include "SpriteRenderer.h"
#include <fstream>

void SnakeHead::AddPart()
{
	auto & game = SnakeGame::Instance();

	// If we dont have any part yet
	if (snakeParts.empty())
	{
		SnakeTailPart part(lastPos, lastHeadDir, "_afterHead");

		game.CreateTailPart(part);

		part.currentPos.column = (game.grid.size() - 1) / 2;
		part.currentPos.elem = (game.grid[0].size() - 1) / 2;
		part.part->gameEntity->transform->SetLocation(game.grid[part.currentPos.column][part.currentPos.elem].pos);

		snakeParts.push_back(part);
	}
	else
	{
		SnakeTailPart part(snakeParts.back().lastPos, lastHeadDir, std::to_string(snakeParts.size()));

		game.CreateTailPart(part);

		part.currentPos.column = (game.grid.size() - 1) / 2;
		part.currentPos.elem = (game.grid[0].size() - 1) / 2;
		part.part->gameEntity->transform->SetLocation(game.grid[part.currentPos.column][part.currentPos.elem].pos);

		snakeParts.push_back(part);
	}
}

void SnakeHead::Tick()
{
	auto& game = SnakeGame::Instance();

	SnakeHeadDirection dir = currentHeadDirection;

	if (App::Instance().input->GetKeyDown(VK_UP))
	{
		currentHeadDirection = SnakeHeadDirection::Up;
	}
	else if (App::Instance().input->GetKeyDown(VK_DOWN))
	{
		currentHeadDirection = SnakeHeadDirection::Down;
	}
	else if (App::Instance().input->GetKeyDown(VK_LEFT))
	{
		currentHeadDirection = SnakeHeadDirection::Left;
	}
	else if (App::Instance().input->GetKeyDown(VK_RIGHT))
	{
		currentHeadDirection = SnakeHeadDirection::Right;
	}

	// If direction is horizontal use columns, else rows
	// TODO: Handle scenario if position is grid bounds

	auto& grid = game.grid;

	if (game.frame + (size_t)game.speedModifier >= game.frameDelay)
	{
		lastPos = currentPos;

		switch (currentHeadDirection)
		{
		case SnakeHeadDirection::Up:
			if (currentPos.column <= 0)
			{
				currentPos.column = grid.size() - 1;
			}
			else
			{
				currentPos.column -= 1;
			}
			break;
		case SnakeHeadDirection::Left:
			if (currentPos.elem <= 0)
			{// TODO: Fix this bullshit bug
				currentPos.elem = grid[0].size();
			}
			currentPos.elem -= 1;
			break;
		case SnakeHeadDirection::Right:
			if (currentPos.elem == grid[0].size() - 1)
			{
				currentPos.elem = 0;
			}
			else currentPos.elem += 1;
			break;
		case SnakeHeadDirection::Down:
			if (currentPos.column == grid.size() - 1)
			{
				currentPos.column = 0;
			}
			else currentPos.column += 1;
			break;
		}

		if (snakeParts.size() > 0)
		{
			if (snakeParts[0].currentPos == currentPos)
			{
				currentPos = lastPos;

				switch (currentHeadDirection)
				{
				case SnakeHeadDirection::Up:
					currentHeadDirection = SnakeHeadDirection::Down;
					break;
				case SnakeHeadDirection::Left:
					currentHeadDirection = SnakeHeadDirection::Right;
					break;
				case SnakeHeadDirection::Right:
					currentHeadDirection = SnakeHeadDirection::Left;
					break;
				case SnakeHeadDirection::Down:
					currentHeadDirection = SnakeHeadDirection::Up;
					break;
				}

				return;
			}
		}

		dirCell = grid[currentPos.column][currentPos.elem];
		
		if (game.collisionEnabled)
		{
			head->gameEntity->transform->SetLocation(dirCell.pos);
		}

		game.frameFinished = true;
		game.frame = 0;
	}
	else
	{
		game.frame++;
	}

	if (currentPos == game.foodPos)
	{
		game.SetRandomFoodPosition();
		game.score += 1;

		if(game.score > game.bestScore)
			game.bestScore = game.score;

		game.speedModifier += 0.3F;
		AddPart();
	}

	for (auto& part : snakeParts)
	{
		if (currentPos == part.currentPos && game.collisionEnabled && part.spawned)
		{
			game.inGame = false;
			UI::cursorEnabled = true;
			game.SetActiveMap(SnakeGameMap::GameOverMap);
			game.gameOver = true;
			game.SaveScore();
		}
	}
}


SnakeTailPart::SnakeTailPart(GridIndex& index, SnakeHeadDirection& headDir, const std::string& name) : otherPos(index), dir(headDir), name(name)
{
}

void SnakeTailPart::Tick()
{
	//std::cout << currentPos.column << " here " << std::endl;
	//std::cout << currentPos.elem << " here row " << std::endl;
	//std::cout << part->gameEntity->name << std::endl;

	//if (part->gameEntity->name != "SnakeTailPart_afterHead")
	//{
	//	MessageBox(0, std::to_string(currentPos.column).c_str(), 0, 0);
	//	MessageBox(0, std::to_string(currentPos.elem).c_str(), 0, 0);
	//}
	//std::cout << currentPos.column << "c " << currentPos.elem << std::endl;
	//std::cout << lastPos.column << " " << lastPos.elem << std::endl;
	auto& game = SnakeGame::Instance();
	
	if (game.frame + (size_t)game.speedModifier >= game.frameDelay)
	{
		lastPos = currentPos;
	}

	currentPos = otherPos;
		 
	part->gameEntity->transform->SetLocation(game.grid[currentPos.column][currentPos.elem].pos);
	spawned = true;
}


SnakeGame & SnakeGame::Instance()
{
	static SnakeGame game;
	return game;
}

// Should be called once
void SnakeGame::CreateGrid()
{
	auto area = App::Instance().gfx->GetRenderArea();
	size_t numCells = (size_t)((area.x / 64)*(area.y / 64));
	size_t numRowElements = size_t(area.x / 64);


	size_t numColumns = size_t(area.y / 64);

	grid.reserve(numColumns);
	// Add columns
	for (size_t i = 0; i < numColumns; i++)
	{
		grid.push_back(std::vector<GridCell>());
	}

	size_t j = 0;
	for (auto& elem : grid)
	{
		for (size_t i = 0; i < numRowElements; i++)
		{
			GridCell cell;
			cell.pos = FVector2((float)i * 64, (float)j * 64);
			elem.push_back(cell);
		}

		j++;
	}

	//gridCells.reserve(numCells);
	////                     192
	//for (size_t i = 0; i < numCells; i++)
	//{
	//	gridCells.push_back(GridCell());
	//}
	
	//for (size_t i = 0; i < area.x / 64; i++)
	//{
	//	for (size_t j = 0; j < area.y / 64; j++)
	//	{
	//		GridCell cell;
	//		cell.pos = FVector2((float)i * 64, (float)j * 64);
	//		grid.push_back(cell);
	//		//if (i == 0 && j == 0)
	//			//snakeHead.head->gameEntity->transform->SetLocation(gridCells[i * j].pos);
	//		//if(i == 0 && j <= 2)
	//		//std::cout << gridCells[i*j].pos.x << " " << gridCells[i*j].pos.y << std::endl;
	//	}
	//}
}

void SnakeGame::DrawGrid()
{
	auto& gfx = App::Instance().gfx;
	auto area = gfx->GetRenderArea();

	for (size_t x = 0; x < (size_t)area.x; x+=64)
	{
		gfx->d2dImmediateContext->DrawLine(D2D1::Point2F((float)x, 0.0F), D2D1::Point2F((float)x, area.y), gfx->brush, 0.5F);
	}

	for (size_t y = 0; y < (size_t)area.y; y+=64)
	{
		gfx->d2dImmediateContext->DrawLine(D2D1::Point2F(0, (float)y), D2D1::Point2F(area.x, (float)y), gfx->brush, 0.5F);
	}
}

void SnakeGame::SetRandomFoodPosition()
{
	bool occupied = true;
	GridIndex pos;

	while (occupied)
	{
		std::mt19937 rng(rd());
		std::uniform_int_distribution<int> columnRange(0, grid.size() - 1);
		std::uniform_int_distribution<int> rowRange(0, grid[0].size() - 1);
		pos.column = columnRange(rng);
		pos.elem = rowRange(rng);

		bool found = false;
		for (size_t i = 0; i < snakeHead.snakeParts.size(); i++)
		{
			if (snakeHead.snakeParts[i].currentPos == pos || pos.column == 0 && pos.elem == 0)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			occupied = false;
			foodPos = pos;
			foodSprite->gameEntity->transform->SetLocation(grid[foodPos.column][foodPos.elem].pos);
			break;
		}
	}
}

void SnakeGame::Initialize()
{
	frame = 0;
	frameDelay = 25;
	UI::cursor = L"Gamedata/Textures/UI/cursor.png";
}

void SnakeGame::CreateMaps()
{
	CreateMainMap();
	CreateMainMenuMap();
	CreatePauseMenuMap();
	CreateGameOverMap();
}


void SnakeGame::CreateMainMenuMap()
{
	// There will be four buttons in main menu which are:
	// Play Game Button
	// Credits Button
	// Quit button

	mainMenuMap = App::Instance().mapManager->CreateMap("MainMenu");
	auto ebrImage = mainMenuMap->AddEntity("brImage");
	backgroundImg = ebrImage->AttachComponent<UIImage>();
	backgroundImg->SetSprite(L"Gamedata/Textures/UI/bg_MainMenu.png");


	auto ePlayGameButton = mainMenuMap->AddEntity("PlayGameButton");
	auto playBtImg = ePlayGameButton->AttachComponent<UIImage>();
	playBtImg->SetSprite(L"Gamedata/Textures/UI/playGameButton.png");
	playGameButton = ePlayGameButton->AttachComponent<UIButton>();
	ePlayGameButton->transform->SetLocation(FVector2(App::Instance().gfx->d2dImmediateContext->GetSize().width / 2 - 100, 
		App::Instance().gfx->d2dImmediateContext->GetSize().height / 2 + 50));
	playGameButton->onClick.AddListener([&] ()
	{
		if (!creditsImageDrawn)
		{
			UI::cursorEnabled = false;
			inGame = true;
			App::Instance().mapManager->SetActiveMap(mainMap);
		}
	});
	playGameButton->onMouseOver.AddListener([&] ()
	{
		if (!creditsImageDrawn)
			playGameButton->highlightedColor = { 1.5f, 1.5f, 1.5f, 1.0f };
	});
	playGameButton->onMouseExit.AddListener([&] ()
	{
		if (!creditsImageDrawn)
			playGameButton->highlightedColor = { 1, 1, 1, 1 };
	});


	auto eCreditsBt = mainMenuMap->AddEntity("CreditsButton");
	creditsImg = eCreditsBt->AttachComponent<UIImage>();
	creditsImg->SetSprite(L"Gamedata/Textures/UI/CreditsButton.png");
	creditsButton = eCreditsBt->AttachComponent<UIButton>();
	eCreditsBt->transform->SetLocation(FVector2(App::Instance().gfx->d2dImmediateContext->GetSize().width / 2 - 100,
		App::Instance().gfx->d2dImmediateContext->GetSize().height / 2 + 150));

	creditsButton->onClick.AddListener([&] ()
	{
		creditsImg->gameEntity->active = true;
	});
	creditsButton->onMouseOver.AddListener([&] ()
	{
		if (!creditsImageDrawn)
			creditsButton->highlightedColor = { 1.5f, 1.5f, 1.5f, 1.0f };
	});
	creditsButton->onMouseExit.AddListener([&] ()
	{
		if (!creditsImageDrawn)
			creditsButton->highlightedColor = { 1, 1, 1, 1 };
	});


	auto eQuitButton = mainMenuMap->AddEntity("QuitButton");
	auto quitImg = eQuitButton->AttachComponent<UIImage>();
	quitImg->SetSprite(L"Gamedata/Textures/UI/QuitButton.png");
	quitButton = eQuitButton->AttachComponent<UIButton>();
	eQuitButton->transform->SetLocation(FVector2(App::Instance().gfx->d2dImmediateContext->GetSize().width / 2 - 100,
		App::Instance().gfx->d2dImmediateContext->GetSize().height / 2 + 250));
	quitButton->onClick.AddListener([&] ()
	{
		if (!creditsImageDrawn)
			App::Instance().running = false;
	});
	quitButton->onMouseOver.AddListener([&] ()
	{
		if (!creditsImageDrawn)
			quitButton->highlightedColor = { 1.5f, 1.5f, 1.5f, 1.0f };
	});
	quitButton->onMouseExit.AddListener([&] ()
	{
		if (!creditsImageDrawn)
			quitButton->highlightedColor = { 1, 1, 1, 1 };
	});

	auto eCredits = mainMenuMap->AddEntity("CreditsImg");
	creditsImg = eCredits->AttachComponent<UIImage>();
	creditsImg->SetSprite(Sprite(L"Gamedata/Textures/UI/Credits.png"));
	creditsImgButton = eCredits->AttachComponent<UIButton>();
	creditsImgButton->onClick.AddListener([&] ()
	{
		if (creditsImageDrawn)
		{
			creditsImg->gameEntity->active = false;
			creditsButton->highlightedColor = { 1, 1, 1, 1 };
		}

	});
	eCredits->transform->SetLocation(FVector2(App::Instance().gfx->d2dImmediateContext->GetSize().width / 2 - creditsImg->GetSprite().GetSize().x / 2, 35));
	eCredits->active = false;

	App::Instance().mapManager->SetActiveMap(mainMenuMap);
}

void SnakeGame::CreateMainMap()
{
	mainMap = App::Instance().mapManager->CreateMap("MainMap");
	CreateGrid();
	CreateFood();
	SetRandomFoodPosition();

	// Create snake head
	auto eHead = mainMap->AddEntity("SnakeHead");
	snakeHead.head = eHead->AttachComponent<SpriteRenderer>();
	snakeHead.head->sprite = L"Gamedata/Textures/Player/SnakeHead.png";
	snakeHead.map = mainMap;

	snakeHead.currentPos.column = (grid.size() - 1) / 2;
	snakeHead.currentPos.elem = (grid[0].size() - 1) / 2;
	// Set snake head location to the center
	snakeHead.head->gameEntity->transform->SetLocation(grid[snakeHead.currentPos.column][snakeHead.currentPos.elem].pos);
	snakeHead.currentHeadDirection = SnakeHeadDirection::None;
	snakeHead.lastHeadDir = SnakeHeadDirection::None;

	//std::cout << gridCells[1].pos.x << " " << gridCells[1].pos.y;
	snakeHead.snakeParts.reserve(192); // this... THIIIIS!!! argh..
	score = 0;
	speedModifier = 0;
	helpMessageDrawn = true;
	lastKey = LastKey::None;
	collisionEnabled = true;

	std::ifstream ifs("Gamedata/highscore");
	ifs >> bestScore;
}

void SnakeGame::CreatePauseMenuMap()
{
	pauseMenuMap = App::Instance().mapManager->CreateMap("PasueMenuMap");
	auto ebrImage = pauseMenuMap->AddEntity("brImage_PAUSE");
	backgroundImgPause = ebrImage->AttachComponent<UIImage>();
	backgroundImgPause->SetSprite(L"Gamedata/Textures/UI/bg_MainMenu.png");

	auto eContinueButton = pauseMenuMap->AddEntity("ContinueButton_PAUSE");
	auto contImg = eContinueButton->AttachComponent<UIImage>();
	contImg->SetSprite(L"Gamedata/Textures/UI/ContinueButton.png");
	continueButton = eContinueButton->AttachComponent<UIButton>();
	eContinueButton->transform->SetLocation(FVector2(App::Instance().gfx->d2dImmediateContext->GetSize().width / 2 - 100,
		App::Instance().gfx->d2dImmediateContext->GetSize().height / 2 + 150));

	continueButton->onClick.AddListener([&] ()
	{
		SetActiveMap(SnakeGameMap::MainMap);
		inGame = true;
		UI::cursorEnabled = false;
		isPaused = false;
	});
	continueButton->onMouseOver.AddListener([&] ()
	{
		continueButton->highlightedColor = { 1.5f, 1.5f, 1.5f, 1.0f };
	});
	continueButton->onMouseExit.AddListener([&] ()
	{
		continueButton->highlightedColor = { 1, 1, 1, 1 };
	});

	auto eQuitButton = pauseMenuMap->AddEntity("QuitButton_PAUSE");
	auto quitImg = eQuitButton->AttachComponent<UIImage>();
	quitImg->SetSprite(L"Gamedata/Textures/UI/QuitButton.png");
	quitButtonPause = eQuitButton->AttachComponent<UIButton>();
	eQuitButton->transform->SetLocation(FVector2(App::Instance().gfx->d2dImmediateContext->GetSize().width / 2 - 100,
		App::Instance().gfx->d2dImmediateContext->GetSize().height / 2 + 250));
	quitButtonPause->onClick.AddListener([&] ()
	{
		App::Instance().running = false;
	});

	quitButtonPause->onMouseOver.AddListener([&] ()
	{
		quitButtonPause->highlightedColor = { 1.5f, 1.5f, 1.5f, 1.0f };
	});

	quitButtonPause->onMouseExit.AddListener([&] ()
	{
		quitButtonPause->highlightedColor = { 1, 1, 1, 1 };
	});
}

void SnakeGame::CreateGameOverMap()
{
	gameOverMap = App::Instance().mapManager->CreateMap("GameOverMenuMap");
	auto ebrImage = gameOverMap->AddEntity("brImage_GAYMOVER");
	backgroundImgGameOver = ebrImage->AttachComponent<UIImage>();
	backgroundImgGameOver->SetSprite(L"Gamedata/Textures/UI/bg_GameOverMenu.png");

	auto ePlayAgainButton = gameOverMap->AddEntity("PlayAgainButton");
	auto playAgainImg = ePlayAgainButton->AttachComponent<UIImage>();
	playAgainImg->SetSprite(L"Gamedata/Textures/UI/PlayAgainButton.png");
	playAgainButton = ePlayAgainButton->AttachComponent<UIButton>();
	ePlayAgainButton->transform->SetLocation(FVector2(App::Instance().gfx->d2dImmediateContext->GetSize().width / 2 - 100,
		App::Instance().gfx->d2dImmediateContext->GetSize().height / 2 + 150));

	playAgainButton->onClick.AddListener([&] ()
	{
		App::Instance().mapManager->DestroyMap(mainMap);
		snakeHead.snakeParts.clear();
		grid.clear();
		CreateMainMap();
		gameBegan = false;
		UI::cursorEnabled = false;
		gameOver = false;
		inGame = true;
		SetActiveMap(SnakeGameMap::MainMap);
	});
	playAgainButton->onMouseOver.AddListener([&] ()
	{
		playAgainButton->highlightedColor = { 1.5f, 1.5f, 1.5f, 1.0f };
	});
	playAgainButton->onMouseExit.AddListener([&] ()
	{
		playAgainButton->highlightedColor = { 1, 1, 1, 1 };
	});


	auto eQuitButton = gameOverMap->AddEntity("QuitButton_GAMEOVER");
	auto quitImg = eQuitButton->AttachComponent<UIImage>();
	quitImg->SetSprite(L"Gamedata/Textures/UI/QuitButton.png");
	quitButtonGameOver = eQuitButton->AttachComponent<UIButton>();
	eQuitButton->transform->SetLocation(FVector2(App::Instance().gfx->d2dImmediateContext->GetSize().width / 2 - 100,
		App::Instance().gfx->d2dImmediateContext->GetSize().height / 2 + 250));
	quitButtonGameOver->onClick.AddListener([&] ()
	{
		App::Instance().running = false;
	});

	quitButtonGameOver->onMouseOver.AddListener([&] ()
	{
		quitButtonGameOver->highlightedColor = { 1.5f, 1.5f, 1.5f, 1.0f };
	});

	quitButtonGameOver->onMouseExit.AddListener([&] ()
	{
		quitButtonGameOver->highlightedColor = { 1, 1, 1, 1 };
	});
}

void SnakeGame::CreateFood()
{	
	// Spawn initial apple
	auto eApple = mainMap->AddEntity("Food");
	foodSprite = eApple->AttachComponent<SpriteRenderer>();
	foodSprite->sprite = L"Gamedata/Textures/Gameplay/Apple.png";
}

void SnakeGame::CreateTailPart(SnakeTailPart & fromPart)
{
	collisionEnabled = false;
	auto ePart = mainMap->AddEntity("SnakeTailPart"+fromPart.name);
	fromPart.part = ePart->AttachComponent<SpriteRenderer>();
	fromPart.part->sprite = L"Gamedata/Textures/Player/SnakeBodyPart.png";
	collisionEnabled = true;
}

void SnakeGame::SaveScore()
{
	std::ofstream ofs("Gamedata/highscore");
	ofs << bestScore;
}

void SnakeGame::SetActiveMap(SnakeGameMap map)
{
	switch (map)
	{
		case SnakeGameMap::MainMenu:
			if (mainMenuMap != nullptr)
			{
				App::Instance().mapManager->SetActiveMap(mainMenuMap);
			}
			break;
		case SnakeGameMap::PauseMenu:
			if (pauseMenuMap != nullptr)
				App::Instance().mapManager->SetActiveMap(pauseMenuMap);
			break;
		case SnakeGameMap::MainMap:
			if (mainMap != nullptr)
				App::Instance().mapManager->SetActiveMap(mainMap);
			break;
		case SnakeGameMap::GameOverMap:
			if (gameOverMap != nullptr)
				App::Instance().mapManager->SetActiveMap(gameOverMap);
			break;
	}
}

void SnakeGame::Tick()
{
	if(!inGame)
		creditsImageDrawn = creditsImg != nullptr && creditsImg->gameEntity->active;

	if (!gameOver)
	{
		if (App::Instance().input->GetKeyDown(VK_ESCAPE))
		{
			if (!isPaused)
			{
				SetActiveMap(SnakeGameMap::PauseMenu);
				inGame = false;
				UI::cursorEnabled = true;
				isPaused = true;
			}
			else
			{
				SetActiveMap(SnakeGameMap::MainMap);
				inGame = true;
				UI::cursorEnabled = false;
				isPaused = false;
			}
		}
	}


	if (inGame)
	{
		if (App::Instance().input->GetKeyDown(0x0D))
		{
			helpMessageDrawn = false;
			gameBegan = true;
		}

		if (gameBegan)
		{
			snakeHead.Tick();

			for (size_t i = 0; i < snakeHead.snakeParts.size(); i++)
			{
				snakeHead.snakeParts[i].Tick();
			}
		}
	}
}

void SnakeGame::Draw()
{
	DrawGrid();
}

void SnakeGame::DrawTopMost()
{
	auto str = std::wstring(L"score: ") + std::to_wstring(score);
	App::Instance().gfx->d2dImmediateContext->DrawTextA(str.c_str(), str.length(),
		App::Instance().gfx->format, D2D1::RectF(20, -50, App::Instance().gfx->d2dImmediateContext->GetSize().width, 100), App::Instance().gfx->m_d2dDefaultBrush);
	
	auto highscoreText = std::wstring(L"highscore: ") + std::to_wstring(bestScore);
	App::Instance().gfx->d2dImmediateContext->DrawTextA(
		highscoreText.c_str(), highscoreText.length(),
		App::Instance().gfx->format, 
		D2D1::RectF(App::Instance().gfx->d2dImmediateContext->GetSize().width - 350, 
			-50, 
			App::Instance().gfx->d2dImmediateContext->GetSize().width,
			100), 
		App::Instance().gfx->m_d2dDefaultBrush);

	if (helpMessageDrawn)
	{
		std::wstring enterMsg(L"Press 'Enter' to start");
		App::Instance().gfx->d2dImmediateContext->DrawTextA(enterMsg.c_str(), enterMsg.length(),
			App::Instance().gfx->format,
			D2D1::RectF(
			(App::Instance().gfx->d2dImmediateContext->GetSize().width / 2 - 250),
				(App::Instance().gfx->d2dImmediateContext->GetSize().height / 2),
				App::Instance().gfx->d2dImmediateContext->GetSize().width, 100), App::Instance().gfx->m_d2dDefaultBrush);
	}
}
