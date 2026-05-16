#include "Game.h"
#include "../UI/Toolbar.h"
#include "../Config/GameConfig.h"
#include "../CMUgraphicsLib/auxil.h"
#include <fstream> // feature 28
#include <iostream>
using namespace std;


Game::Game()
{
	timerValue = 60;
	lastRealTime = time(0);
	isPaused = false;
	isGameOver = false;
	lastWolfSpawnTimerValue = -1;
	//1 - Create the main window
	pWind = CreateWind(config.windWidth, config.windHeight, config.wx, config.wy);
	pWind->SetBuffering(true);
	//2 - create and draw the toolbar
	createToolbar();
	createBudgetbar();
	updatestatusbar();
	drawfieldboundary();
	warehouse();
	//3 - create and draw the backgroundPlayingArea


	//4- Create the Plane
	//TODO: Add code to create and draw the Plane

	//5- Create the Bullet
	//TODO: Add code to create and draw the Bullet

	//6- Create the enemies
	//TODO: Add code to create and draw enemies in random places

	//7- Create and clear the status bar
	clearStatusBar();
	if (Wolf::wolfList == nullptr) {
		Wolf::wolfList = new Wolf * [20];
		for (int i = 0; i < 20; i++)
		{
			Wolf::wolfList[i] = nullptr;
		}
	}



}

Game::~Game()
{
}

clicktype Game::getMouseClick(int& x, int& y) const
{
	return pWind->GetMouseClick(x, y);
}


string Game::getSrting() const
{
	string Label;
	char Key;
	keytype ktype;
	pWind->FlushKeyQueue();
	while (1)
	{
		ktype = pWind->WaitKeyPress(Key);
		if (ktype == ESCAPE)	//ESCAPE key is pressed
			return "";	//returns nothing as user has cancelled label
		if (Key == 13)	//ENTER key is pressed
			return Label;
		if (Key == 8)	//BackSpace is pressed
			if (Label.size() > 0)
				Label.resize(Label.size() - 1);
			else
				Key = '\0';
		else
			Label += Key;
		printMessage(Label);
	}
}

window* Game::CreateWind(int w, int h, int x, int y) const
{
	window* pW = new window(w, h, x, y);
	pW->SetBrush(config.bkGrndColor);
	pW->SetPen(config.bkGrndColor, 1);
	pW->DrawRectangle(0, 0, w, h);
	return pW;
}

void Game::createToolbar() 
{
	point toolbarUpperleft;
	toolbarUpperleft.x = 0;
	toolbarUpperleft.y = 0;

	gameToolbar = new Toolbar(this, toolbarUpperleft, 0, config.toolBarHeight);
	gameToolbar->draw();
}

void Game::createBudgetbar()
{
	point budgetbarUpperleft;
	budgetbarUpperleft.x = 0;
	budgetbarUpperleft.y = config.toolBarHeight;

	gameBudgetbar = new Budgetbar(this, budgetbarUpperleft, 0, config.toolBarHeight);
	gameBudgetbar->draw();
}
//Ahmed Ismail Features 5 & 6
void Game::drawWolf() const
{
	window* pWind = getWind();
	pWind->DrawImage("images\\wolf.jpg", 110, 110);
}

void Game::drawFoodArea() const
{
	int foodAreaSize = 1 * config.toolBarHeight;
	int foodAreaLeft = (config.windWidth - foodAreaSize) / 2;
	int foodAreaTop = (config.windHeight - foodAreaSize) / 2;

	point p;
	p.y = foodAreaTop;
	drawFoodAreaAt(p, 5);
}
void Game::drawFoodAreaAt(point topLeft, int counter) const
{
	int foodAreaSize = 1 * config.toolBarHeight;

	pWind->SetPen(DARKGREEN, 2);
	pWind->SetBrush(DARKGREEN);
	pWind->DrawRectangle(topLeft.x, topLeft.y, topLeft.x + foodAreaSize, topLeft.y + foodAreaSize);

	// Draw counter above food area 
	pWind->SetPen(BLACK, 1);
	pWind->SetFont(18, BOLD, BY_NAME, "Arial");
	pWind->DrawString(topLeft.x + 15, topLeft.y - 25, to_string(counter));
}

void Game::clearBudget() const
{
	//Clear Status bar by drawing a filled rectangle
	pWind->SetPen(config.bkGrndColor, 1);
	pWind->SetBrush(config.bkGrndColor);
	pWind->DrawRectangle(config.windWidth - 500, config.toolBarHeight, config.windWidth, 2*config.toolBarHeight);
}

void Game::printBudget(string msg) const
{
	clearBudget();	//First clear the status bar

	pWind->SetPen(config.penColor, 50);
	pWind->SetFont(24, BOLD, BY_NAME, "Arial");
	pWind->DrawString(config.windWidth - 575, config.toolBarHeight + 10, msg);

}

void Game::clearStatusBar() const
{
	//Clear Status bar by drawing a filled rectangle
	pWind->SetPen(config.statusBarColor, 1);
	pWind->SetBrush(config.statusBarColor);
	pWind->DrawRectangle(0, config.windHeight - config.statusBarHeight, config.windWidth, config.windHeight);
}
void Game::clearPlayingArea() const
{
	pWind->SetPen(config.bkGrndColor, 1);
	pWind->SetBrush(config.bkGrndColor);
	pWind->DrawRectangle(0, 2 * config.toolBarHeight, config.windWidth, config.windHeight - config.statusBarHeight);
}

void Game::updatestatusbar() const
{
	clearStatusBar();
	pWind->SetPen(config.penColor);
	pWind->SetFont(20, BOLD, BY_NAME, "Arial");

}
void Game::drawfieldboundary() const {
	pWind->SetPen(config.penColor, config.penWidth);
	// Draw from below the BudgetBar to above the StatusBar
	pWind->DrawRectangle(0, 2 * config.toolBarHeight, config.windWidth, config.windHeight - config.statusBarHeight, FRAME);
}
void Game::printMessage(string msg) const
{
	clearStatusBar();	//First clear the status bar

	pWind->SetPen(config.penColor, 50);
	pWind->SetFont(24, BOLD, BY_NAME, "Arial");
	pWind->DrawString(10, config.windHeight - (int)(0.85 * config.statusBarHeight), msg);

}
void Game::warehouse() const    // Draw the warehouse in the upper right corner of the playing area
{
	window* pWind = getWind();
	pWind->DrawImage("images\\warehouse.jpg", 550, 100, 110, 110);

}
void Game::drawWarehouseWindow() const
{
	if (!warehouseOpened) return;

	// 1. Draw Background Overlay (the popup box)
	pWind->SetBrush(WHITE);
	pWind->SetPen(BLACK, 2);
	pWind->DrawRectangle(300, 150, 900, 550);

	pWind->SetFont(30, BOLD, BY_NAME, "Arial");
	pWind->DrawString(450, 180, "WAREHOUSE");
	
	pWind->SetFont(22, BOLD, BY_NAME, "Arial");
	pWind->SetPen(BLACK);
	pWind->DrawString(380, 300, "Eggs: " + to_string(warehouseEggs));
	pWind->DrawString(380, 380, "Milk: " + to_string(warehouseMilk));

	pWind->SetBrush(LIGHTGRAY);
	pWind->DrawRectangle(650, 280, 750, 320); // Sell Egg Box
	pWind->DrawRectangle(650, 360, 750, 400); // Sell Milk Box

	pWind->SetPen(BLACK);
	pWind->DrawString(670, 290, "SELL");
	pWind->DrawString(670, 370, "SELL");

	pWind->SetBrush(RED);
	pWind->DrawRectangle(860, 160, 890, 190);
	pWind->SetPen(WHITE);
	pWind->DrawString(867, 162, "X");
}// y m
void Game::handleWarehouseClick(int x, int y)
{
	// Close Button
	if (x >= 860 && x <= 890 && y >= 160 && y <= 190)
	{
		warehouseOpened = false;
		return;
	}

	// Sell Eggs
	if (x >= 650 && x <= 750 && y >= 280 && y <= 320)
	{
		budget += warehouseEggs * 20;
		warehouseEggs = 0;

		drawWarehouseWindow();
		return;
	}

	// Sell Milk
	if (x >= 650 && x <= 750 && y >= 360 && y <= 400)
	{
		budget += warehouseMilk * 50;
		warehouseMilk = 0;

		drawWarehouseWindow();
		return;
	}
}

// Abdelaziz feature 1
void Game::drawStatusText() const
{
	clearStatusBar();

	pWind->SetPen(WHITE, 1);
	pWind->SetFont(20, BOLD, BY_NAME, "Arial");
	pWind->DrawString(20, config.windHeight - 35, "Timer: " + to_string(timerValue));
	pWind->DrawString(260, config.windHeight - 35, "Goal: $" + to_string(goal));
	pWind->DrawString(520, config.windHeight - 35, "Level: " + to_string(level));
	pWind->DrawString(760, config.windHeight - 35, "Animals: " + to_string(animalCount));
	//feature 26
	if (isPaused)
	{
		pWind->DrawString(980, config.windHeight - 35, "PAUSED");
	}
}

void Game::drawGameOverText() const
{
	if (!isGameOver)
	{
		return;
	}

	if ((time(0) % 2) == 0)
	{
		pWind->SetPen(RED, 1);
		pWind->SetFont(72, BOLD, BY_NAME, "Arial");
		pWind->DrawString(390, 260, "GAME OVER");
	}
}

void Game::spawnWolf() const
{
	int spawnInterval = 22 - (level);
	if (spawnInterval <= 0)
	{
		spawnInterval = 3;
	}

	if ((int)timerValue % spawnInterval == 0 && timerValue != lastWolfSpawnTimerValue && Wolf::count < 20)
	{
		Wolf::wolfList[Wolf::count] = new Wolf(const_cast<Game*>(this), 50, 50, "images\\wolf.jpg");
		Wolf::count++;
		lastWolfSpawnTimerValue = timerValue;
	}
}


// feature 11
void Game::intialTimer() const
{
	if (budget >= goal)
	{
		timerValue = 60 - (10 * (level - 1));
		if (timerValue < 20) // to limit intial timer to 20
		{
			timerValue = 20;
		}

		lastRealTime = time(0);
	}
}

// feature 12
void Game::updateTime() const
{
	if (isPaused)
	{
		return; // feature 26
	}

	time_t currentRealTime = time(0);

	if (currentRealTime != lastRealTime) {
		double secondsPassed = difftime(currentRealTime, lastRealTime);
		timerValue -= (int)secondsPassed;
		lastRealTime = currentRealTime;

		if (timerValue <= 0)
		{
			timerValue = 0;
			isGameOver = true;
			const_cast<Game*>(this)->pauseGame();
			return;
		}

		intialTimer();
	}
}

void Game::syncTimersAfterResume()
{
	time_t now = time(0);
	lastRealTime = now; 

	for (int i = 0; i < ChickIcon::count; i++)
	{
		if (ChickIcon::chickList[i] != nullptr)
		{
			ChickIcon::chickList[i]->lastTime = now; // feature 26
		}
	}
	for (int i = 0; i < ChickIcon::count; i++) //25
	{
		if (ChickIcon::chickList[i] != nullptr)
		{
			ChickIcon::chickList[i]->productReady = false;
		}
	}


	for (int i = 0; i < CowIcon::count; i++)
	{
		if (CowIcon::cowList[i] != nullptr)
		{
			CowIcon::cowList[i]->lastTime = now; // feature 26
		}
	} 
	for (int i = 0; i < CowIcon::count; i++)//25
	{
		if (CowIcon::cowList[i] != nullptr)
		{
			CowIcon::cowList[i]->productReady = false;
		}
	}


	for (int i = 0; i < Wolf::count; i++)
	{
		if (Wolf::wolfList[i] != nullptr)
		{
			Wolf::wolfList[i]->lastTime = now; // feature 26
		}
	}
}

void Game::clearDynamicObjects()//featuer 27
{
	for (int i = 0; i < ChickIcon::count; i++)
	{
		delete ChickIcon::chickList[i];
		ChickIcon::chickList[i] = nullptr;
	}
	ChickIcon::count = 0;

	for (int i = 0; i < CowIcon::count; i++)
	{
		delete CowIcon::cowList[i];
		CowIcon::cowList[i] = nullptr; 
	}
	CowIcon::count = 0;

	for (int i = 0; i < Wolf::count; i++)
	{

		delete Wolf::wolfList[i];
		Wolf::wolfList[i] = nullptr; 
	}
	Wolf::count = 0; 

	for (int i = 0; i < 15; i++)
	{
		WaterIcon::waterList[i].x = 0; 
		WaterIcon::waterList[i].y = 0; 
		WaterIcon::waterCounters[i] = 0; 
		WaterIcon::waterActive[i] = false; 
	}
	WaterIcon::count = 0; 
}

void Game::pauseGame()//feature 26
{
	if (isPaused)
	{
		return; 
	}
	isPaused = true; 
}

void Game::resumeGame()//feature 26
{
	if (!isPaused)
	{
		return; 
	}
	syncTimersAfterResume(); 
	isPaused = false; 
}

void Game::restartGame()// feature 27
{
	clearDynamicObjects(); 
	budget = 3000; 
	timerValue = 60; 
	goal = 5000; 
	level = 0; 
	animalCount = 0; 
	lastRealTime = time(0); 
	lastWolfSpawnTimerValue = -1; 
	isPaused = false; 
	isGameOver = false;
	clearPlayingArea(); 
	clearBudget(); 
	clearStatusBar(); 
}


void Game::saveGame() // feature 28
{ 
	string saveFilePath = "D:\\Study\\ZC\\Spring 2026\\CIE 101 (C++ and OOP)\\CIE101_ProjectStartupCode\\savegame.txt"; 
	std::ofstream saveFile(saveFilePath); 
	saveFile << "LEVEL " << level << '\n';
	saveFile << "BUDGET " << budget << '\n';
	saveFile << "TIMER " << timerValue << '\n';
	saveFile << "GOAL " << goal << '\n';
	saveFile << "ANIMALCOUNT " << animalCount << '\n';
	saveFile << "PAUSED " << isPaused << '\n';
	saveFile << "GAMEOVER " << isGameOver << '\n';
	saveFile << "LASTWOLFSPAWN " << lastWolfSpawnTimerValue << '\n';
	saveFile << '\n';


	//Animals
	int savedAnimalsCount = 0;
	for (int i = 0; i < ChickIcon::count; i++)
	{
		if (ChickIcon::chickList[i] != nullptr)
		{savedAnimalsCount++;} 
	}

	for (int i = 0; i < CowIcon::count; i++)
	{
		if (CowIcon::cowList[i] != nullptr)
		{savedAnimalsCount++;}
	}

	saveFile << "ANIMALS " << savedAnimalsCount << '\n';

	for (int i = 0; i < ChickIcon::count; i++)
	{if (ChickIcon::chickList[i] == nullptr)
		{
		continue;
		} 
		saveFile << "CHICK " << ChickIcon::chickList[i]->curr_pos.x << ' ' << ChickIcon::chickList[i]->curr_pos.y << ' ' << ChickIcon::chickList[i]->counter << ' ' << ChickIcon::chickList[i]->productReady << ' ' << ChickIcon::chickList[i]->curr_vel.x << ' ' << ChickIcon::chickList[i]->curr_vel.y << ' ' << ChickIcon::chickList[i]->productPoint.x << ' ' << ChickIcon::chickList[i]->productPoint.y << '\n'; 
	}

	for (int i = 0; i < CowIcon::count; i++) 
	{ 
		if (CowIcon::cowList[i] == nullptr) 
		{ 
			continue; 
		} 
		saveFile << "COW " << CowIcon::cowList[i]->curr_pos.x << ' ' << CowIcon::cowList[i]->curr_pos.y << ' ' << CowIcon::cowList[i]->counter << ' ' << CowIcon::cowList[i]->productReady << ' ' << CowIcon::cowList[i]->curr_vel.x << ' ' << CowIcon::cowList[i]->curr_vel.y << ' ' << CowIcon::cowList[i]->productPoint.x << ' ' << CowIcon::cowList[i]->productPoint.y << '\n'; 
	}
	saveFile << '\n'; 


	//wolves
	int savedWolvesCount = 0;
	for (int i = 0; i < Wolf::count; i++)
	{
		if (Wolf::wolfList[i] != nullptr) 
		{
			savedWolvesCount++;
		}
	}

	saveFile << "WOLVES " << savedWolvesCount << '\n';
	for (int i = 0; i < Wolf::count; i++) 
	{ 
		if (Wolf::wolfList[i] == nullptr)
		{ 
			continue; 
		} 
		saveFile << "WOLF " << Wolf::wolfList[i]->curr_pos.x << ' ' << Wolf::wolfList[i]->curr_pos.y << '\n'; 
	}
	saveFile << '\n';
	


	//food areas
	saveFile << "FOODAREAS " << WaterIcon::count << '\n';
	static time_t lastFoodTime = time(0); // feature 26: to ensure food area timers are updated correctly even if the game is paused when saving

	if (!isPaused && time(0) != lastFoodTime)
	{
		for (int i = 0; i < WaterIcon::count; i++)
		{
			if (WaterIcon::waterActive[i])
			{
				WaterIcon::waterCounters[i]--;

				if (WaterIcon::waterCounters[i] <= 0)
				{
					WaterIcon::waterActive[i] = false;
				}
			}
		}

		lastFoodTime = time(0);
	}

	for (int i = 0; i < WaterIcon::count; i++)
	{
		if (WaterIcon::waterActive[i])
		{
			drawFoodAreaAt(WaterIcon::waterList[i], WaterIcon::waterCounters[i]);
		}
	}
	for (int i = 0; i < WaterIcon::count; i++)
	{
		saveFile << "FOOD " << WaterIcon::waterList[i].x << ' ' << WaterIcon::waterList[i].y << ' ' << WaterIcon::waterCounters[i] << ' ' << WaterIcon::waterActive[i] << '\n';
	}

	saveFile.close();
	printMessage("Game saved to savegame.txt");
} //

void Game::wolfKill() const
{
	for (int i = 0; i < Wolf::count; i++)
	{
		if (Wolf::wolfList[i] == nullptr)
		{
			continue;
		}
		if (!isPaused)
		{
			for (int j = 0; j < ChickIcon::count; j++)
			{
				if (Wolf::wolfList[i]->CollisionDetection(*ChickIcon::chickList[j]))
				{
					delete ChickIcon::chickList[j];
					ChickIcon::chickList[j] = ChickIcon::chickList[ChickIcon::count - 1];
					ChickIcon::count--;
					Game::animalCount--;
					cout << "wolf ate a chick" << endl;
				}
			}
			for (int j = 0; j < CowIcon::count; j++)
			{
				if (Wolf::wolfList[i]->CollisionDetection(*CowIcon::cowList[j]))
				{
					delete CowIcon::cowList[j];
					CowIcon::cowList[j] = CowIcon::cowList[CowIcon::count - 1];
					CowIcon::count--;
					Game::animalCount--;
					cout << "wolf ate a cow" << endl;
				}
			}
		}
	}
}

bool Game::getPausedState() const
{
	return isPaused; // feature 26
}

window* Game::getWind() const
{
	return pWind;
}
void Game::go() const
{
	// This function reads the position where the user clicks to determine the desired operation
	int x, y;
	bool isExit = false;
	pWind->SetBuffering(true);
	// Change the title
	pWind->ChangeTitle("- - - - - - Farm Frenzy (CIE101-project/ Team 16) - - - - - -");

	
	do
	{
		clearPlayingArea();
		clearBudget();
		updateTime();
		
		clearStatusBar();
		drawStatusText();
		gameToolbar->draw();
		gameBudgetbar->draw();

		string budget_string = "BUDGET = $" + to_string(budget) + " | Chick: $100 | Cow : $200 | water : $50";
		printBudget(budget_string);
		if (!isPaused)
		{
			gameBudgetbar->updateAnimals(); // feature 26
		}
		Pause(100);
		drawfieldboundary();

		warehouse();

		for (int i = 0; i < WaterIcon::count; i++)
		{
			if (WaterIcon::waterActive[i])
			{
				drawFoodAreaAt(WaterIcon::waterList[i], WaterIcon::waterCounters[i]);
			}
		}

		for (int i = 0; i < ChickIcon::count; i++) 
		{
			if (ChickIcon::chickList[i] == nullptr)
			{
				continue;
			}
			if (!isPaused)
			{
				ChickIcon::chickList[i]->moveStep(); 	// feature 26
				for (int j = 0; j < WaterIcon::count; j++)
				{
					if (WaterIcon::waterActive[j] &&
						ChickIcon::chickList[i]->curr_pos.x < WaterIcon::waterList[j].x + 50 &&
						ChickIcon::chickList[i]->curr_pos.x + 50 > WaterIcon::waterList[j].x &&
						ChickIcon::chickList[i]->curr_pos.y < WaterIcon::waterList[j].y + 50 &&
						ChickIcon::chickList[i]->curr_pos.y + 50 > WaterIcon::waterList[j].y)
					{
						WaterIcon::waterCounters[j]--;

						if (WaterIcon::waterCounters[j] <= 0)
							WaterIcon::waterActive[j] = false;
					}
				}
			
			}
			ChickIcon::chickList[i]->draw();
			ChickIcon::chickList[i]->drawProduct();
			ChickIcon::chickList[i]->drawCounter();
		}

		for (int i = 0; i < CowIcon::count; i++)
		{
			if (CowIcon::cowList[i] == nullptr)
			{
				continue;
			}
			if (!isPaused)
			{
				CowIcon::cowList[i]->moveStep(); // feature 26
				for (int j = 0; j < WaterIcon::count; j++)
				{
					if (WaterIcon::waterActive[j] &&
						CowIcon::cowList[i]->curr_pos.x < WaterIcon::waterList[j].x + 50 &&
						CowIcon::cowList[i]->curr_pos.x + 50 > WaterIcon::waterList[j].x &&
						CowIcon::cowList[i]->curr_pos.y < WaterIcon::waterList[j].y + 50 &&
						CowIcon::cowList[i]->curr_pos.y + 50 > WaterIcon::waterList[j].y)
					{
						WaterIcon::waterCounters[j]--;

						if (WaterIcon::waterCounters[j] <= 0)
							WaterIcon::waterActive[j] = false;
					}
				}


			}
			CowIcon::cowList[i]->draw();
			CowIcon::cowList[i]->drawProduct();
			CowIcon::cowList[i]->drawCounter();
		}
		
		if (!isPaused)
		{
			spawnWolf(); // feature 26
		}

		wolfKill();

		for (int i = 0; i < Wolf::count; i++)
		{
			if (Wolf::wolfList[i] == nullptr)
			{
				continue;
			}
			if (!isPaused)
			{
				Wolf::wolfList[i]->moveStep(); // feature 26
			}
			Wolf::wolfList[i]->draw();
		}

		if (pWind->GetMouseClick(x, y))
		{
			if (warehouseOpened)
			{
				const_cast<Game*>(this)->handleWarehouseClick(x, y);
			}
			else if (y >= 0 && y < config.toolBarHeight)
			{
				isExit = gameToolbar->handleClick(x, y);
			}
			else if (y >= config.toolBarHeight && y < 2 * config.toolBarHeight)
			{
				if (!isPaused)
					isExit = gameBudgetbar->handleClick(x, y);
			}
			else //20 
			{
				bool productClicked = false;

				for (int i = 0; i < ChickIcon::count; i++)
				{
					if (ChickIcon::chickList[i] != nullptr &&
						ChickIcon::chickList[i]->productReady &&
						x >= ChickIcon::chickList[i]->productPoint.x &&
						x <= ChickIcon::chickList[i]->productPoint.x + 30 &&
						y >= ChickIcon::chickList[i]->productPoint.y &&
						y <= ChickIcon::chickList[i]->productPoint.y + 30)
					{
						const_cast<Game*>(this)->warehouseEggs++;
						ChickIcon::chickList[i]->productReady = false;
						productClicked = true;
						break;
					}
				}

				if (!productClicked)
				{
					for (int i = 0; i < CowIcon::count; i++)
					{
						if (CowIcon::cowList[i] != nullptr &&
							CowIcon::cowList[i]->productReady &&
							x >= CowIcon::cowList[i]->productPoint.x &&
							x <= CowIcon::cowList[i]->productPoint.x + 30 &&
							y >= CowIcon::cowList[i]->productPoint.y &&
							y <= CowIcon::cowList[i]->productPoint.y + 30)
						{
							const_cast<Game*>(this)->warehouseMilk++;
							CowIcon::cowList[i]->productReady = false;
							productClicked = true;
							break;
						}
					}
				}

				if (!productClicked)
				{
					for (int i = 0; i < Wolf::count; i++)
					{
						if (Wolf::wolfList[i] != nullptr &&
							x >= Wolf::wolfList[i]->curr_pos.x && x <= Wolf::wolfList[i]->curr_pos.x + 50 &&
							y >= Wolf::wolfList[i]->curr_pos.y && y <= Wolf::wolfList[i]->curr_pos.y + 50)
						{
							Wolf::wolfList[i]->clickCount++;

							if (Wolf::wolfList[i]->clickCount >= 5)
							{
								delete Wolf::wolfList[i];

								for (int j = i; j < Wolf::count - 1; j++)
								{
									Wolf::wolfList[j] = Wolf::wolfList[j + 1];
								}

								Wolf::count--;
								Wolf::wolfList[Wolf::count] = nullptr;
							}

							break;
											}
										}
									}
								}
			if (x >= 550 && x <= 660 && y >= 100 && y <= 210)
			{
				const_cast<Game*>(this)->warehouseOpened = true;
			}

								}
		drawWarehouseWindow();
		drawGameOverText();
		pWind->UpdateBuffer();
		Sleep(30);

								} while (!isExit);
						}
