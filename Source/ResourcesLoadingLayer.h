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

#include <string>

#include "2d/Layer.h"
#include "math/Vec2.h"

class GameManager;
namespace ax 
{ 
	class RepeatForever; 
	class Director;
	class FileUtils;
	class Scene;
}

class ResourcesLoadingLayer : public ax::Layer
{
private:
	ax::RepeatForever* _gdProcessAction;
	ax::Director* _dir;
	ax::FileUtils* _fu;
	GameManager* _gm;
	std::string _writablePath;
	ax::Vec2 _posMiddle;

	void handleWindows();
	bool isWindowsGDPathValid(std::string exepath);
	static std::string getRunningGDPathWindows();

	void handleMac();
	void handleLinux();
	
public:
	void loadLoadingLayer();
	bool init();
	static ResourcesLoadingLayer* create();
	static ax::Scene* scene();
};