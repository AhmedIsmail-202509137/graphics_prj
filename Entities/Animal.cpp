#include "Animal.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include <iostream>
using namespace std;

int Wolf::count = 0;
Wolf** Wolf::wolfList = nullptr;

Animal::Animal(Game* r_pGame, point r_point, int r_width, int r_height, string img_path, int r_maxCounter, string r_productImage) : Drawable(r_pGame, r_point, r_width, r_height)
{
	image_path = img_path;
	curr_pos = r_point;
	curr_vel.x = 1;
	curr_vel.y = 1;
	counter = 0;
	maxCounter = r_maxCounter;
	lastTime = time(0);
	productReady = false;
	productImage = r_productImage;
}


void Animal::draw() const
{
	//draw image of this object
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}

// Abdelaziz feature 19
void Animal::updateCounter()
{
	time_t now = time(0);
	int seconds = (int)difftime(now, lastTime);

	if (seconds > 0)
	{
		counter = counter + seconds;
		lastTime = now;
	}

	if (counter >= maxCounter)
	{
		counter = 0;
		productReady = true;
		lastTime = now;
	}
}

void Animal::drawProduct() const
{
	if (productReady == true)
	{
		window* pWind = pGame->getWind();
		pWind->DrawImage(productImage, RefPoint.x + 10, RefPoint.y + height + 5, 30, 30);
	}
}


Chick::Chick(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Animal(r_pGame, r_point, r_width, r_height, img_path, 10, "images\\egg.jpg")
{}

void Chick::moveStep()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(-3, 3);

	bool hitWall = false;

	// Left/Right boundaries
	if (curr_pos.x + curr_vel.x <= range_min_x || curr_pos.x + curr_vel.x >= range_max_x) {
		curr_pos.x -= curr_vel.x;
		curr_vel.x = -curr_vel.x;
		hitWall = true;
	}

	// Top/Bottom boundaries
	if (curr_pos.y + curr_vel.y <= range_min_y || curr_pos.y + curr_vel.y >= range_max_y) {
		curr_pos.y -= curr_vel.y;
		curr_vel.y = -curr_vel.y; 
		hitWall = true;
	}

	// Percentage of randomly changing a direction is currently 1% (also changes direction if hitting a wall
	std::uniform_int_distribution<int> chance(1, 100);
	if (chance(gen) <= 1 || hitWall) {
		curr_vel.x = dist(gen);
		curr_vel.y = dist(gen);
	}

	// Update Position
	curr_pos.x += curr_vel.x;
	curr_pos.y += curr_vel.y;
	RefPoint = curr_pos;
}

Cow::Cow(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Animal(r_pGame, r_point, r_width, r_height, img_path, 20, "images\\milk.jpg")
{}

void Cow::moveStep()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(-3, 3);

	bool hitWall = false;

	// Left/Right boundaries
	if (curr_pos.x + curr_vel.x <= range_min_x || curr_pos.x + curr_vel.x >= range_max_x) {
		curr_pos.x -= curr_vel.x;
		curr_vel.x = -curr_vel.x;
		hitWall = true;
	}

	// Top/Bottom boundaries
	if (curr_pos.y + curr_vel.y <= range_min_y || curr_pos.y + curr_vel.y >= range_max_y) {
		curr_pos.y -= curr_vel.y;
		curr_vel.y = -curr_vel.y;
		hitWall = true;
	}

	// Percentage of randomly changing a direction is currently 1% (also changes direction if hitting a wall
	std::uniform_int_distribution<int> chance(1, 100);
	if (chance(gen) <= 1 || hitWall) {
		curr_vel.x = dist(gen);
		curr_vel.y = dist(gen);
	}

	// Update Position
	curr_pos.x += curr_vel.x;
	curr_pos.y += curr_vel.y;
	RefPoint = curr_pos;
}

// Abdelaziz feature 7
Egg::Egg(Game* r_pGame, point r_point, int r_width, int r_height) : Drawable(r_pGame, r_point, r_width, r_height)
{
}

void Egg::draw() const
{
	window* pWind = pGame->getWind();
	pWind->DrawImage("images\\egg.jpg", RefPoint.x, RefPoint.y, width, height);
}

Milk::Milk(Game* r_pGame, point r_point, int r_width, int r_height) : Drawable(r_pGame, r_point, r_width, r_height)
{
}

void Milk::draw() const
{
	window* pWind = pGame->getWind();
	pWind->DrawImage("images\\milk.jpg", RefPoint.x, RefPoint.y, width, height);
}

Wolf::Wolf(Game* r_pGame, int r_width, int r_height, string img_path) : Animal(r_pGame, { 0,0 }, r_width, r_height, img_path, 0, "")
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> dist1(range_min_x, range_max_x);
	std::uniform_int_distribution<int> dist2(range_min_y, range_max_y);

	curr_pos.x = dist1(gen);
	curr_pos.y = dist2(gen);
	RefPoint = curr_pos;
}

void Wolf::moveStep()
{
	// add movement code for wolf here
}

// Feature 21 Abdelaziz 

void Animal::drawCounter() const
{
	if (maxCounter <= 0)
		return;

	window* pWind = pGame->getWind();
	int remainingTime = maxCounter - counter;

	if (remainingTime < 0)
		remainingTime = 0;

	pWind->SetPen(BLACK, 1);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(RefPoint.x + 10, RefPoint.y + height + 2,
		RefPoint.x + 38, RefPoint.y + height + 22);
	pWind->SetFont(16, BOLD, BY_NAME, "Arial");
	pWind->DrawString(RefPoint.x + 15, RefPoint.y + height + 4,
		to_string(remainingTime));
}

void Chick::drawCounter() const
{
	if (maxCounter <= 0)
		return;

	window* pWind = pGame->getWind();
	int remainingTime = maxCounter - counter;

	if (remainingTime < 0)
		remainingTime = 0;

	int counterLeft = RefPoint.x + 11;
	int counterTop = RefPoint.y + height + 3;

	pWind->SetPen(BLACK, 1);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(counterLeft, counterTop,
		counterLeft + 28, counterTop + 20);
	pWind->SetFont(16, BOLD, BY_NAME, "Arial");
	pWind->DrawString(counterLeft + 5, counterTop + 2,
		to_string(remainingTime));
}

void Cow::drawCounter() const
{
	if (maxCounter <= 0)
		return;

	window* pWind = pGame->getWind();
	int remainingTime = maxCounter - counter;

	if (remainingTime < 0)
		remainingTime = 0;

	int counterLeft = RefPoint.x + (width / 2) - 14;
	int counterTop = RefPoint.y + height - 20;

	pWind->SetPen(BLACK, 1);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(counterLeft, counterTop,
		counterLeft + 28, counterTop + 18);
	pWind->SetFont(15, BOLD, BY_NAME, "Arial");
	pWind->DrawString(counterLeft + 5, counterTop + 1,
		to_string(remainingTime));
}
