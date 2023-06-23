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

#include "getTextureString.h"
#include "GameManager.h"

static inline std::string getTextureString_AppendResources(std::string texture)
{
	std::string nTexture = "Resources/";
	nTexture += texture;

	auto pos = nTexture.find('.');
	if (pos != std::string::npos) {
		nTexture.insert(pos, GameManager::getInstance()->isHigh() ? "-uhd" : "-hd");	
	}
	// GameToolbox::log("texture: {}", nTexture);
	return nTexture;
}

static inline std::string getTextureString_WithoutResources(std::string texture)
{
	auto pos = texture.find('.');
	if (pos != std::string::npos) {
		texture.insert(pos, GameManager::getInstance()->isHigh() ? "-uhd" : "-hd");	
	}
	return texture;
}


std::string GameToolbox::getTextureString(std::string texture) {
	return getTextureString_WithoutResources(texture);
}
