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

namespace LevelTools {
	bool verifyLevelIntegrity(std::string_view levelData, int id);
	
	std::string getAudioFilename(int lid);
	std::string getAudioTitle(int lid);
	int getArtistForAudio(int lid);
	std::string getNameForArtist(int artistId);
	std::string getFbURLForArtist(int artistId);
	std::string getNgURLForArtist(int artistId);
	std::string getYtURLForArtist(int artistId);
	std::string getURLForAudio(int lid);
}
