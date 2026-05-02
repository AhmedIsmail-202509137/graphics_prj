#include "Budgetbar.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include <iostream>
using namespace std;

const int MAX_BUDGETBAR_ITEMS = 15;

int ChickIcon::count = 0;
int CowIcon::count = 0;
int WaterIcon::count = 0;
Chick** ChickIcon::chickList = nullptr;
Cow** CowIcon::cowList = nullptr;
point WaterIcon::waterList[15]{};
int WaterIcon::waterCounters[15]{};
bool WaterIcon::waterActive[15]{};

BudgetbarIcon::BudgetbarIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Drawable(r_pGame, r_point, r_width, r_height)
{
	image_path = img_path;
}

void BudgetbarIcon::draw() const
{
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}

ChickIcon::ChickIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
	if (chickList == nullptr)
	{
		chickList = new Chick * [MAX_BUDGETBAR_ITEMS];
		for (int i = 0; i < MAX_BUDGETBAR_ITEMS; i++) {
			chickList[i] = nullptr;
		}
	}
}

CowIcon::CowIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
	if (cowList == nullptr)
	{
		cowList = new Cow * [MAX_BUDGETBAR_ITEMS];
		for (int i = 0; i < MAX_BUDGETBAR_ITEMS; i++) {
			cowList[i] = nullptr;
		}
	}
}

WaterIcon::WaterIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: BudgetbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}

void ChickIcon::onClick()
{
	cout << "Icon Chick Clicked" << endl;
	if (count < MAX_BUDGETBAR_ITEMS && pGame->budget >= 100) {
		pGame->budget = pGame->budget - 100;
		pGame->clearBudget();
		string budget_string = "BUDGET = $" + to_string(pGame->budget);
		pGame->printBudget(budget_string);

		point p;
		std::random_device rd1;
		std::mt19937 gen1(rd1());
		std::uniform_int_distribution<int> dist1(range_min_x, range_max_x);
		p.x = dist1(gen1);

		std::random_device rd2;
		std::mt19937 gen2(rd2());
		std::uniform_int_distribution<int> dist2(range_min_y, range_max_y);
		p.y = dist2(gen2);

		chickList[count] = new Chick(pGame, p, 50, 50, image_path);
		chickList[count]->draw();
		pGame->animalCount++;
		count++;
	}
}

void ChickIcon::updateChicks()
{
	for (int i = 0; i < count; i++)
	{
		if (chickList[i] != nullptr)
		{
			chickList[i]->updateCounter();
		}
	}
}

void ChickIcon::drawCount() const
{
	if (count <= 0)
		return;

	window* pWind = pGame->getWind();
	int counterLeft = RefPoint.x + width - 18;
	int counterTop = RefPoint.y + 4;

	pWind->SetPen(RED, 2);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(counterLeft, counterTop, counterLeft + 24, counterTop + 22);
	pWind->SetFont(16, BOLD, BY_NAME, "Arial");
	pWind->SetPen(BLACK, 1);
	pWind->DrawString(counterLeft + 6, counterTop + 3, to_string(count));
}

void CowIcon::onClick()
{
	cout << "Icon Cow Clicked" << endl;
	if (count < MAX_BUDGETBAR_ITEMS && pGame->budget >= 200) {
		pGame->budget = pGame->budget - 200;
		pGame->clearBudget();
		string budget_string = "BUDGET = $" + to_string(pGame->budget);
		pGame->printBudget(budget_string);

		point p;
		std::random_device rd1;
		std::mt19937 gen1(rd1());
		std::uniform_int_distribution<int> dist1(range_min_x, range_max_x);
		p.x = dist1(gen1);

		std::random_device rd2;
		std::mt19937 gen2(rd2());
		std::uniform_int_distribution<int> dist2(range_min_y, range_max_y);
		p.y = dist2(gen2);

		cowList[count] = new Cow(pGame, p, 50, 50, image_path);
		cowList[count]->draw();
		pGame->animalCount++;
		count++;
	}
}

void CowIcon::updateCows()
{
	for (int i = 0; i < count; i++)
	{
		if (cowList[i] != nullptr)
		{
			cowList[i]->updateCounter();
		}
	}
}

void CowIcon::drawCount() const
{
	if (count <= 0)
		return;

	window* pWind = pGame->getWind();
	int counterLeft = RefPoint.x + width - 18;
	int counterTop = RefPoint.y + 4;

	pWind->SetPen(RED, 2);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(counterLeft, counterTop, counterLeft + 24, counterTop + 22);
	pWind->SetFont(16, BOLD, BY_NAME, "Arial");
	pWind->SetPen(BLACK, 1);
	pWind->DrawString(counterLeft + 6, counterTop + 3, to_string(count));
}

void WaterIcon::onClick()
{
	cout << "Icon Water Clicked" << endl;
	if (pGame->budget >= 50 && count < MAX_BUDGETBAR_ITEMS) {
		pGame->budget = pGame->budget - 50;
		pGame->clearBudget();
		string budget_string = "BUDGET = $" + to_string(pGame->budget);
		pGame->printBudget(budget_string);

		point p;
		int foodAreaSize = 5 * config.toolBarHeight;

		std::random_device rd1;
		std::mt19937 gen1(rd1());
		std::uniform_int_distribution<int> dist1(0, config.windWidth - foodAreaSize);
		p.x = dist1(gen1);

		std::random_device rd2;
		std::mt19937 gen2(rd2());
		std::uniform_int_distribution<int> dist2(2 * config.toolBarHeight, config.windHeight - config.statusBarHeight - foodAreaSize);
		p.y = dist2(gen2);

		waterList[count] = p;
		waterCounters[count] = 5;
		waterActive[count] = true;
		pGame->drawFoodAreaAt(p);
		count++;
	}
}

void WaterIcon::updateWater()
{
	static time_t lastTime = time(0);
	time_t now = time(0);
	bool tick = difftime(now, lastTime) >= 1.0;
	if (tick) lastTime = now;

	for (int i = 0; i < count; i++)
	{
		if (!waterActive[i]) continue;
		if (tick)
		{
			waterCounters[i]--;
			if (waterCounters[i] <= 0) { waterActive[i] = false; continue; }
		}
		pGame->drawFoodAreaAt(waterList[i]);
		window* pWind = pGame->getWind();
		pWind->SetPen(BLACK, 1);
		pWind->SetBrush(WHITE);
		pWind->DrawRectangle(waterList[i].x + 5, waterList[i].y - 25,
			waterList[i].x + 35, waterList[i].y - 5);
		pWind->SetFont(16, BOLD, BY_NAME, "Arial");
		pWind->DrawString(waterList[i].x + 10, waterList[i].y - 23,
			to_string(waterCounters[i]));
	}
}

Budgetbar::Budgetbar(Game* r_pGame, point r_point, int r_width, int r_height) : Drawable(r_pGame, r_point, r_width, r_height)
{
	iconsImages[ICON_CHICK] = "images\\chick.jpg";
	iconsImages[ICON_COW] = "images\\cow.jpg";
	iconsImages[ICON_WATER] = "images\\water.jpg";

	point p;
	p.x = 0;
	p.y = config.toolBarHeight;

	iconsList = new BudgetbarIcon * [ANIMAL_COUNT];

	iconsList[ICON_CHICK] = new ChickIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_CHICK]);
	p.x += config.iconWidth;
	iconsList[ICON_COW] = new CowIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_COW]);
	p.x += config.iconWidth;
	iconsList[ICON_WATER] = new WaterIcon(pGame, p, config.iconWidth, config.toolBarHeight, iconsImages[ICON_WATER]);
}

Budgetbar::~Budgetbar()
{
	for (int i = 0; i < ANIMAL_COUNT; i++)
		delete iconsList[i];
	delete[] iconsList;
}

void Budgetbar::draw() const
{
	for (int i = 0; i < ANIMAL_COUNT; i++)
		iconsList[i]->draw();
	((ChickIcon*)iconsList[ICON_CHICK])->drawCount();
	((CowIcon*)iconsList[ICON_COW])->drawCount();
	window* pWind = pGame->getWind();
	pWind->SetPen(BLACK, 3);
	pWind->DrawLine(0, 2 * config.toolBarHeight, pWind->GetWidth(), 2 * config.toolBarHeight);
}

void Budgetbar::updateAnimals()
{
	((ChickIcon*)iconsList[ICON_CHICK])->updateChicks();
	((CowIcon*)iconsList[ICON_COW])->updateCows();
}

bool Budgetbar::handleClick(int x, int y)
{
	if (x > ANIMAL_COUNT * config.iconWidth)
		return false;

	int clickedIconIndex = (x / config.iconWidth);
	iconsList[clickedIconIndex]->onClick();

	return false;
}
