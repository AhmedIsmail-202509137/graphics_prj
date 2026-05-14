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
	mutable bool isGameOver = false;
	mutable int lastWolfSpawnTimerValue = -1; 
	void syncTimersAfterResume(); 
	void clearDynamicObjects(); 
	bool warehouseOpened = false; 
	int warehouseEggs = 0;        
	int warehouseMilk = 0;
	void drawWarehouseWindow() const; 
	void drawGameOverText() const;
	void handleWarehouseClick(int x, int y);
	
public:
	int budget = 3000;
	mutable int timerValue = 60;
	int goal = 5000;
	mutable int level = 1;
	mutable int animalCount = 0;

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
	void drawFoodAreaAt(point topLeft, int counter = 0) const;
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
	void wolfKill() const;

	// Features 26,27
	void pauseGame(); 
	void resumeGame();
	void restartGame();
	void saveGame();
	bool getPausedState() const;


#pragma once
#include "../Core/Drawable.h"

	class FoodArea : public Drawable // 18   
	{
	private:
		int remainingSeconds;
		unsigned long lastTick;
		bool visible;

		void clearCounter() const;
		void drawCounter() const;

	public:
		FoodArea(Game* r_pGame, point r_point, int r_width, int r_height, int durationSeconds);
		void draw() const override;
		void erase() const;
		bool update();
		bool isVisible() const;
	};

	class FoodAreaManager
	{
	private:
		static const int MAX_FOOD_AREAS = 20;
		FoodArea* foodAreas[MAX_FOOD_AREAS];
		Game* pGame;

	public:
		FoodAreaManager(Game* r_pGame);
		~FoodAreaManager();
		void addFoodArea(int x, int y);
		void update();
	};
	void go() const;

	window* getWind() const;		//returns a pointer to the graphics window
};

