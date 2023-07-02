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

class LoadingCircle;
class MenuItemSpriteExtra;
class GJGameLevel;

namespace ax 
{ 
	class Scene;
	namespace network 
	{ 
		class HttpRequest; 
		class HttpClient;
		class HttpResponse;
	} 
}


class LevelInfoLayer : public ax::Layer
{
private:
	LoadingCircle* loading;
	MenuItemSpriteExtra* playBtn;
	GJGameLevel* _level;
	ax::network::HttpRequest* _request;
public:
	static LevelInfoLayer* create(GJGameLevel* level);

	static ax::Scene* scene(GJGameLevel* level);
	bool init(GJGameLevel* level);

	void onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response);

	void onDownloadFailed();
};
