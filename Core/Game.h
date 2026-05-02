#pragma once
#include "../CMUgraphicsLib/CMUgraphics.h"
#include "../UI/Toolbar.h"
#include "../UI/BudgetBar.h"
#include <ctime> // for time function

class Game
{
private:
	window* pWind;	//Pointer to the CMU graphics window
	Toolbar* gameToolbar;
	Budgetbar* gameBudgetbar;
	mutable time_t lastRealTime;
	// Features 26,27
	bool isPaused = false; 
	mutable int lastWolfSpawnTimerValue = -1; 
	void syncTimersAfterResume(); 
	void clearDynamicObjects(); 

public:
	int budget = 3000;
	mutable int timerValue = 60;
	int goal = 5000;
	mutable int level = 0;
	int animalCount = 0;

	Game();
	~Game();

	clicktype getMouseClick(int& x, int& y) const; //Get coordinate where user clicks and returns click type (left/right)
	string getSrting() const;	 //Returns a string entered by the user

	window* CreateWind(int, int, int, int) const; //creates the game window
	void createToolbar();
	void createBudgetbar();
	// Features 5, 6
	void drawWolf() const;
	void drawFoodArea() const;
	// End of features 5,6
	void drawFoodAreaAt(point topLeft) const;
	void clearBudget() const;
	void printBudget(string msg) const;
	void clearStatusBar() const;	//Clears the status bar
	void updatestatusbar() const;
	void printMessage(string msg) const;	//Print a message on Status bar

	void drawStatusText() const;
	void drawfieldboundary() const; //to set the field boundary	//Print a message on Status bar
	void warehouse() const;
	void clearPlayingArea() const;

	void intialTimer() const;
	void updateTime() const;
	void spawnWolf() const;
	// Features 26,27
	void pauseGame(); 
	void resumeGame();
	void restartGame();
	bool getPausedState() const;

	void go() const;

	window* getWind() const;		//returns a pointer to the graphics window
};
