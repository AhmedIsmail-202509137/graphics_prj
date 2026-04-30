#include "Animal.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"
#include <iostream>
using namespace std;

int Wolf::count = 0;
Wolf** Wolf::wolfList = nullptr;

Animal::Animal(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Drawable(r_pGame, r_point, r_width, r_height)
{
	image_path = img_path;
	curr_pos = r_point;
	curr_vel.x = 1;
	curr_vel.y = 1;

}

void Animal::draw() const
{
	//draw image of this object
	window* pWind = pGame->getWind();
	pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
}

Chick::Chick(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Animal(r_pGame, r_point, r_width, r_height, img_path)
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

Cow::Cow(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : Animal(r_pGame, r_point, r_width, r_height, img_path)
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

Wolf::Wolf(Game* r_pGame, int r_width, int r_height, string img_path) : Animal(r_pGame, { 0,0 }, r_width, r_height, img_path)
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