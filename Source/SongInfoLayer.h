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
#include <string_view>

#include "PopupLayer.h"


class SongInfoLayer : public PopupLayer
{
private:
	std::string _downloadLink;
	std::string _ngArtistLink;
	std::string _ytArtistLink;
	std::string _fbArtistLink;
public:
	static SongInfoLayer* create(int soundtrackID);
	static SongInfoLayer* create(std::string_view songName, std::string_view artistName, std::string_view urlLink,
		std::string_view ngArtistLink, std::string_view ytArtistLink, std::string_view fbArtistLink);
	bool init(std::string_view songName, std::string_view artistName, std::string_view urlLink,
		std::string_view ngArtistLink, std::string_view ytArtistLink, std::string_view fbArtistLink);
};
