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
#include <vector>

#include "2d/Layer.h"
#include "2d/ActionTween.h"

class BoomScrollLayer : public ax::Layer, public ax::ActionTweenDelegate {
private:
	virtual void updateTweenAction(float value, std::string_view key) override {};
	ax::Layer* _internalLayer;

	float _dragMovement;

public:

	int _currentPage, _leftPage, _rightPage;
	int _totalPages;
	std::vector<ax::Layer*> _layers;
	
	bool init(std::vector<ax::Layer*>, int);
	static BoomScrollLayer* create(std::vector<ax::Layer*> layers, int currentPage);
	void selectPage(int current);
	void changePageRight();
	void changePageLeft();
	// bool onTouchBegan(ax::Touch *touch, ax::Event *event);
	// void onTouchEnded(ax::Touch *touch, ax::Event *event);
	// void onTouchMoved(ax::Touch *touch, ax::Event *event);
	void onExit() override;
};