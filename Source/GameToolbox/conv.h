#pragma once
#include "ccTypes.h"
#include "enums.h"
#include <string_view>
#include <string>
#include <vector>

namespace GameToolbox
{
	ax::Color3B colorForIdx(int col);
	const char* getNameGamemode(IconType mode);
	int getValueForGamemode(IconType mode);
	ax::Color3B hsvToRgb(const ax::HSV& hsv);
	float stof(const std::string_view str);
	int stoi(const std::string_view str);
	const char* levelLengthString(int len);
	std::vector<std::string> splitByDelim(const std::string& s, char delim);
	std::vector<std::string_view> splitByDelimStringView(std::string_view str, char delim);
	ax::BlendFunc getBlending();
	std::string xorCipher(const std::string& message, const std::string& key);

}