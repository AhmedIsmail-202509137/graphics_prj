#pragma once
#include "../CMUgraphicsLib/CMUgraphics.h"
#include "../UI/Toolbar.h"
#include "../UI/BudgetBar.h"

class Game
{
private:
	window* pWind;	//Pointer to the CMU graphics window
	Toolbar* gameToolbar;
	Budgetbar* gameBudgetbar;

public:
	int budget = 2000;
	int timerValue = 60;
	int goal = 5000;
	int level = 1;
	int animalCount = 0;

	Game();
	~Game();

	clicktype getMouseClick(int& x, int& y) const; //Get coordinate where user clicks and returns click type (left/right)
	string getSrting() const;	 //Returns a string entered by the user


	window* CreateWind(int, int, int, int) const; //creates the game window
	void createToolbar();
	void createBudgetbar();
	void clearBudget() const;
	void printBudget(string msg) const;
	void clearStatusBar() const;	//Clears the status bar
	void updatestatusbar() const;
	void printMessage(string msg) const;	//Print a message on Status bar

	void drawStatusText() const;
	void drawfieldboundary() const; //to set the field boundary	//Print a message on Status bar
	void warehouse() const;


	void go() const;

	window* getWind() const;		//returns a pointer to the graphics window
};

