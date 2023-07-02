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

class GroundLayer;
class BoomScrollLayer;
namespace ax 
{ 
	class Sprite;
	class Scene;
}


class LevelSelectLayer : public ax::Layer {
private:
	ax::Sprite* _background;
	GroundLayer* _ground;
	std::vector<Layer*> _levelPages;
public:
	BoomScrollLayer* _bsl;

public:

	static ax::Scene* scene(int page);
	bool init(int page);
	static LevelSelectLayer* create(int page);
	void onExit() override;
};