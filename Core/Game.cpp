#include "Game.h"
#include "../Config/GameConfig.h"
#include "../CMUgraphicsLib/auxil.h"


Game::Game()
{
	timerValue = 60;
	lastRealTime = time(0);
	//1 - Create the main window
	pWind = CreateWind(config.windWidth, config.windHeight, config.wx, config.wy);

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
	p.x = foodAreaLeft;
	p.y = foodAreaTop;
	drawFoodAreaAt(p);
}

void Game::drawFoodAreaAt(point topLeft) const
{
	int foodAreaSize = 1 * config.toolBarHeight;

	pWind->SetPen(DARKGREEN, 2);
	pWind->SetBrush(DARKGREEN);
	pWind->DrawRectangle(topLeft.x, topLeft.y, topLeft.x + foodAreaSize, topLeft.y + foodAreaSize);
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
}

void Game::spawnWolf() const
{
	static int spwanedAtTime = -1;
	if ((int)timerValue % (21 - level) == 0 && timerValue != spwanedAtTime)
	{
		Wolf::wolfList[Wolf::count] = new Wolf(const_cast<Game*>(this), 50, 50, "images\\wolf.jpg");
		Wolf::count++;
		spwanedAtTime = timerValue;
	}
}


// feature 11
void Game::intialTimer() const
{
	if (timerValue <= 0)
	{
		level++;
		timerValue = 60 - (10 * (level - 1));
		lastRealTime = time(0);
	}
}

// feature 12
void Game::updateTime() const
{

	time_t currentRealTime = time(0);

	if (currentRealTime != lastRealTime) {
		double secondsPassed = difftime(currentRealTime, lastRealTime);
		timerValue -= (float)secondsPassed;
		lastRealTime = currentRealTime;
		intialTimer();
	}
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
	pWind->ChangeTitle("- - - - - - - - - - Farm Frenzy (CIE101-project) - - - - - - - - - -");

	do
	{
		updateTime();
		
		clearPlayingArea();
		clearBudget();
		clearStatusBar();
		

		drawStatusText();
		gameToolbar->draw();
		gameBudgetbar->draw();
		string budget_string = "BUDGET = $" + to_string(budget) + " | Chick: $100 | Cow : $200 | water : $50";

		printBudget(budget_string);


		clearPlayingArea();
		gameBudgetbar->updateAnimals();
		Pause(100);
		drawfieldboundary();
		warehouse();

		for (int i = 0; i < WaterIcon::count; i++)
		{
			drawFoodAreaAt(WaterIcon::waterList[i]);
		}

		for (int i = 0; i < ChickIcon::count; i++) 
		{
			ChickIcon::chickList[i]->moveStep();
			ChickIcon::chickList[i]->draw();
			ChickIcon::chickList[i]->drawProduct();
			ChickIcon::chickList[i]->drawCounter();
		}

		for (int i = 0; i < CowIcon::count; i++)
		{
			CowIcon::cowList[i]->moveStep();
			CowIcon::cowList[i]->draw();
			CowIcon::cowList[i]->drawProduct();
			CowIcon::cowList[i]->drawCounter();
		}

		spawnWolf();
		for (int i = 0; i < Wolf::count; i++)
		{
			Wolf::wolfList[i]->moveStep();
			Wolf::wolfList[i]->draw();
		}

		pWind->UpdateBuffer();
		if (pWind->GetMouseClick(x, y))
		{
			if (y >= 0 && y < config.toolBarHeight)
			{
				isExit = gameToolbar->handleClick(x, y);
			}
			// [2] If user clicks on the Budgetbar (Second row)
			else if (y >= config.toolBarHeight && y < 2 * config.toolBarHeight)
			{
				isExit = gameBudgetbar->handleClick(x, y);
			}
		}
		Sleep(30);
		// Note: Add logic here for clicking the actual play area if needed!

	} while (!isExit);
}


