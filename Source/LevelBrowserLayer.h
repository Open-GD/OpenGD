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
#include <unordered_map>
#include <vector>

#include "2d/Layer.h"
#include "GameToolbox/conv.h"

class GJSearchObject;
class LoadingCircle;
class MenuItemSpriteExtra;
class GJGameLevel;

namespace ax 
{ 
	class Scene; 
	namespace ui
	{
		class ListView;
	}

	namespace network
	{
		class HttpClient;
		class HttpResponse;
	}
}



class LevelBrowserLayer : public ax::Layer {
public:
	static ax::Scene* scene(GJSearchObject* search);
	static LevelBrowserLayer* create(GJSearchObject* search);
	bool init(GJSearchObject* search);
	LoadingCircle* _loading;

	ax::ui::ListView* listView;
	GJSearchObject* _searchObj;
	MenuItemSpriteExtra* _leftBtn, *_rightBtn;

	std::unordered_map<int, std::vector<GJGameLevel*>> _cachedLevels;

	void fillList();
	void onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response);
};