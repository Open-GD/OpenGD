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
#include "BaseGameLayer.h"
#include "math/Vec2.h"
#include "EventKeyboard.h"

class GJGameLevel;

namespace ax 
{ 
	class Sprite; 
	class Scene;
	class Event;
}



//follow structure
//1. private members
//2. public members
//3. private functions
//4. public functions

class LevelDebugLayer : public BaseGameLayer {
private:
	ax::Vec2 _camInput;
	float _camSpeed = 420.f;

	ax::Sprite* _BG;

	bool _showDebugMenu = false;

public:
	static LevelDebugLayer* create(GJGameLevel*);
	static ax::Scene* scene(GJGameLevel*);
	void onEnter() override;
	void onExit() override;
	bool init(GJGameLevel*);
	void exit();
	void update(float delta) override;
	void onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
	void onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event);
    void updateVisibility();
    void updateTriggers(float dt);
    void onDrawImgui();
    void playMusic(float dt);
    void reorderLayering(GameObject* parentObj, ax::Sprite* child);
};
