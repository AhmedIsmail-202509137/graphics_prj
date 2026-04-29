#include "Toolbar.h"
#include "../Config/GameConfig.h"
#include "../Core/Game.h"

ToolbarIcon::ToolbarIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path, string text, color fillColor) : Drawable(r_pGame, r_point, r_width, r_height)
{
	image_path = img_path;
	buttonText = text;
	buttonColor = fillColor;
}

void ToolbarIcon::draw() const
{
	window* pWind = pGame->getWind();
	if (image_path != "")
	{
		pWind->DrawImage(image_path, RefPoint.x, RefPoint.y, width, height);
	}
	else
	{
		pWind->SetPen(BLACK, 2);
		pWind->SetBrush(buttonColor);
		pWind->DrawRectangle(RefPoint.x, RefPoint.y, RefPoint.x + width, RefPoint.y + height, FILLED);
		pWind->SetPen(BLACK, 1);
		pWind->SetFont(18, BOLD, BY_NAME, "Arial");
		pWind->DrawString(RefPoint.x + 12, RefPoint.y + 15, buttonText);
	}
}

RestartIcon::RestartIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}

void RestartIcon::onClick()
{

}

ExitIcon::ExitIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path) : ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path)
{
}

void ExitIcon::onClick()
{

}

PauseIcon::PauseIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path, "Pause", LIGHTYELLOW)
{
}

void PauseIcon::onClick()
{
	//TO DO: add code for restarting the game here
}

ResumeIcon::ResumeIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path, "Resume", LIGHTGREEN)
{
}

void ResumeIcon::onClick()
{
	//TO DO: add code for restarting the game here
}

SaveIcon::SaveIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path, "Save", LIGHTBLUE)
{
}

void SaveIcon::onClick()
{

}

LoadGameIcon::LoadGameIcon(Game* r_pGame, point r_point, int r_width, int r_height, string img_path)
	: ToolbarIcon(r_pGame, r_point, r_width, r_height, img_path, "Load", LIGHTCORAL)
{
}

void LoadGameIcon::onClick()
{

}

Toolbar::Toolbar(Game* r_pGame, point r_point, int r_width, int r_height) : Drawable(r_pGame, r_point, r_width, r_height)
{
	//First prepare List of images for each icon
	//To control the order of these images in the menu, reoder them in enum ICONS above	
	iconsImages[ICON_RESTART] = "images\\RESTART.jpg";
	iconsImages[ICON_PAUSE] = "";
	iconsImages[ICON_RESUME] = "";
	iconsImages[ICON_SAVE] = "";
	iconsImages[ICON_LOAD] = "";
	iconsImages[ICON_EXIT] = "images\\EXIT.JPG";
	point p;
	p.x = 0;
	p.y = 0;

	iconsList = new ToolbarIcon * [ICON_COUNT];

	//For each icon in the tool bar create an object 
	iconsList[ICON_RESTART] = new RestartIcon(pGame, p, config.toolbarButtonWidth, config.toolBarHeight, iconsImages[ICON_RESTART]);
	p.x += config.toolbarButtonWidth;
	iconsList[ICON_PAUSE] = new PauseIcon(pGame, p, config.toolbarButtonWidth, config.toolBarHeight, iconsImages[ICON_PAUSE]);
	p.x += config.toolbarButtonWidth;
	iconsList[ICON_RESUME] = new ResumeIcon(pGame, p, config.toolbarButtonWidth, config.toolBarHeight, iconsImages[ICON_RESUME]);
	p.x += config.toolbarButtonWidth;
	iconsList[ICON_SAVE] = new SaveIcon(pGame, p, config.toolbarButtonWidth, config.toolBarHeight, iconsImages[ICON_SAVE]);
	p.x += config.toolbarButtonWidth;
	iconsList[ICON_LOAD] = new LoadGameIcon(pGame, p, config.toolbarButtonWidth, config.toolBarHeight, iconsImages[ICON_LOAD]);
	p.x += config.toolbarButtonWidth;
	iconsList[ICON_EXIT] = new ExitIcon(pGame, p, config.toolbarButtonWidth, config.toolBarHeight, iconsImages[ICON_EXIT]);
}

Toolbar::~Toolbar()
{
	for (int i = 0; i < ICON_COUNT; i++)
		delete iconsList[i];
	delete[] iconsList;
}

void Toolbar::draw() const
{
	for (int i = 0; i < ICON_COUNT; i++)
		iconsList[i]->draw();
	window* pWind = pGame->getWind();
	pWind->SetPen(BLACK, 3);
	pWind->DrawLine(0, config.toolBarHeight, pWind->GetWidth(), config.toolBarHeight);
}

bool Toolbar::handleClick(int x, int y)
{
	if (x > ICON_COUNT * config.toolbarButtonWidth)	//click outside toolbar boundaries
		return false;


	//Check whick icon was clicked
	//==> This assumes that menu icons are lined up horizontally <==
	//Divide x co-ord of the point clicked by the icon width (int division)
	//if division result is 0 ==> first icon is clicked, if 1 ==> 2nd icon and so on

	int clickedIconIndex = (x / config.toolbarButtonWidth);
	iconsList[clickedIconIndex]->onClick();	//execute onClick action of clicled icon

	if (clickedIconIndex == ICON_EXIT) return true;

	return false;

}

