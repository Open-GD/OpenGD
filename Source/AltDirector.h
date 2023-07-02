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
#include "base/Director.h"

	
class AltDirector : public ax::Director { 
public:
	static AltDirector* getInstance() {
		return (AltDirector*)ax::Director::getInstance();
	}

	// Director have this variables in "protected", so i did this class to use them
	ax::Vector<ax::Scene*> getScenesStack() {
		return Director::_scenesStack;
	}
	void setSendCleanupToScene(bool cleanupToScene) {
		ax::Director::_sendCleanupToScene = cleanupToScene;
	}
};