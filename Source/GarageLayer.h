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
#include <array>

#include "2d/Scene.h"

#include "GameToolbox/enums.h"
class SimplePlayer;

namespace ax 
{ 
	class Menu;
	class Sprite;

	namespace ui 
	{ 
		class TextField; 
	}
}


class GarageLayer : public ax::Scene {
public:
	static ax::Scene* scene(bool popSceneWithTransition = false);
	static GarageLayer* create();
	bool init();
	void setupIconSelect();
	const char* getSpriteName(int id, bool actived);
	void setupPage(IconType mode, int page);
	void createStat(const char* sprite, const char* statKey);
	int selectedGameModeInt();

private:
	bool _popSceneWithTransition;
	SimplePlayer* _iconPrev;
	ax::ui::TextField* _userNameField;
	ax::Menu* _menuIcons;
	ax::Sprite* _selectSprite;
	int _numPerRow = 12;
	int _numPerColumn = 3;
	int _stats = 0;
	IconType _selectedMode;
	std::array<int, 8> _modePages{0};
};
