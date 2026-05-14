#include "Drawable.h"

Drawable::Drawable(Game* r_pGame, point r_point, int r_width, int r_height)
{
	pGame = r_pGame;
	RefPoint = r_point;
	width = r_width;
	height = r_height;
}

bool Drawable::CollisionDetection(const Drawable& gObj)
{
	int top = RefPoint.y;
	int bottom = RefPoint.y + height;
	int left = RefPoint.x;
	int right = RefPoint.x + width;

	int otherTop = gObj.RefPoint.y;
	int otherBottom = gObj.RefPoint.y + gObj.height;
	int otherLeft = gObj.RefPoint.x;
	int otherRight = gObj.RefPoint.x + gObj.width;

	if (right < otherLeft || left > otherRight || top > otherBottom || bottom < otherTop)
	{
		return false;
	}

	return true;
}