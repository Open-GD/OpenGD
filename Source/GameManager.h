#pragma once
#include <string>

enum IconType;

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
