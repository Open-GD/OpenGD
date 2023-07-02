/*************************************************************************
    OpenGD - Open source Geometry Dash.
    Copyright (C) 2023  OpenGD Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License    
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/

#pragma once
#include "2d/Layer.h"
#include "math/Vec2.h"
#include "PlatformMacros.h"

class PlayerObject;
class GroundLayer;

namespace ax 
{ 
	class Sprite; 
	class Menu;
	class Scene;
	class Touch;
	class Event;
}



class MenuGameLayer : public ax::Layer {
public:
	PlayerObject* player;
	ax::Vec2 startPos;
	GroundLayer* groundLayer;
	ax::Sprite* bgSpr;
	ax::Menu* bgSprites;
	float bgStartPos;
	float sep;
	float bsizeX;
	

public:
	void processPlayerMovement(float delta);
	void processBackground(float delta);
	void update(float delta);
	void updateVisibility(float delta);
	void resetPlayer(bool touched);
	bool onTouchBegan(ax::Touch *touch, ax::Event *event);
	void renderRect(ax::Rect rect, ax::Color4B col);


	static ax::Scene* scene();
	static MenuGameLayer* create();
	bool init();
};
