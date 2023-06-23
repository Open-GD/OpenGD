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

#include "GameToolbox/enums.h"

class GameManager
{
public:
	bool _mediumQuality;
	bool _openedGarage;
	bool _openedCreator;
	bool _openedPracticeMode;

	int _selectedCube;
	int _selectedShip;
	int _selectedBall;
	int _selectedUfo;
	int _selectedWave;
	int _selectedRobot;
	int _selectedSpider;
	int _selectedSpecial;
	int _selectedDeathEffect;
	int _accountID;
	IconType _mainSelectedMode;
	std::string _filepath;

private:
	bool init();
	void load();
	
public:
	static GameManager* getInstance();
	
	template<typename T>
	T get(const std::string& key);

	template<typename T>
	void set(const std::string& key, const T& val);
	
	void save();
	void setMembersToMap();
	void loadMembersFromMap();
	bool isMedium();
	bool isHigh();
	void setQuality(bool medium);
	void setQualityMedium();
	void setQualityHigh();
	void print();
	int getSelectedIcon(IconType);
	void setSelectedIcon(IconType, int);
	bool isFollowingUser(int accountID);
};
