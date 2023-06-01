#pragma once
#include <string>
#include <string_view>

class getGJRewards;

class getGJRewards { 
public:
	int orbs = 0;
	int diamonds = 0;
	bool key = 0;

	static getGJRewards *create();
};
