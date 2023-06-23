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
#include "PopupLayer.h"

class MenuItemSpriteExtra;

class AlertLayer : public PopupLayer {
private:
	bool init(std::string_view title, std::string_view desc, std::string_view btn1, std::string_view btn2, float width, std::function<void(Node*)> btn1Callback, std::function<void(Node*)> btn2Callback);

	MenuItemSpriteExtra* _btn1;
	MenuItemSpriteExtra* _btn2;

public:
	static AlertLayer* create(std::string_view title, std::string_view desc, std::string_view btn1, std::string_view btn2, float width, std::function<void(Node*)> btn1Callback, std::function<void(Node*)> btn2Callback);
	static AlertLayer* create(std::string_view title, std::string_view desc, std::string_view btn1, std::string_view btn2, std::function<void(Node*)> btn1Callback, std::function<void(Node*)> btn2Callback);
	static AlertLayer* create(std::string_view title, std::string_view desc, std::string_view btn1, float width, std::function<void(Node*)> btn1Callback);
	static AlertLayer* create(std::string_view title, std::string_view desc, std::string_view btn1, std::function<void(Node*)> btn1Callback);
	static AlertLayer* create(std::string_view title, std::string_view desc, float width);
	static AlertLayer* create(std::string_view title, std::string_view desc);

	void setBtn1Callback(std::function<void(Node*)> btn1Callback);
	void setBtn2Callback(std::function<void(Node*)> btn2Callback);
};