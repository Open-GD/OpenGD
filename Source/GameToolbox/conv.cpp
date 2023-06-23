
#include "conv.h"
#include "math/MathUtil.h"
#include "external/fast_float.h"
#include <cmath>
#include <charconv>

bool _showDebugImgui = true;


USING_NS_AX;

ax::BlendFunc GameToolbox::getBlending() {
	return BlendFunc::ADDITIVE;
}

int GameToolbox::stoi(const std::string_view s) {
	int ret = 0;
	std::from_chars(s.data(),s.data() + s.size(), ret);
	return ret;
}

float GameToolbox::stof(const std::string_view s) {
	float ret = 0.0f;
	fast_float::from_chars(s.data(),s.data() + s.size(), ret);
	return ret;
}

std::vector<std::string> GameToolbox::splitByDelim(const std::string& str, char delim)
{
	std::vector<std::string> tokens;
	size_t pos = 0;
	size_t len = str.length();
	tokens.reserve(len / 2);  // allocate memory for expected number of tokens
	
	while (pos < len)
	{
		size_t end = str.find_first_of(delim, pos);
		if (end == std::string::npos)
		{
			tokens.emplace_back(str.substr(pos));
			break;
		}
		tokens.emplace_back(str.substr(pos, end - pos));
		pos = end + 1;
	}

	return tokens;
}


std::vector<std::string_view> GameToolbox::splitByDelimStringView(std::string_view str, char delim)
{
	std::vector<std::string_view> tokens;
	size_t pos = 0;
	size_t len = str.length();

	while (pos < len) {
		size_t end = str.find(delim, pos);
		if (end == std::string_view::npos) {
			tokens.emplace_back(str.substr(pos));
			break;
		}
		tokens.emplace_back(str.substr(pos, end - pos));
		pos = end + 1;
	}

	return tokens;
}


ax::Color3B GameToolbox::hsvToRgb(const ax::HSV& hsv) {
	float c = hsv.v * hsv.s;
	float h_dash = hsv.h / 60.0f;
	float x = c * (1.0f - std::fabs(std::fmod(h_dash, 2.0f) - 1.0f));
	float m = hsv.v - c;

	float r1, g1, b1;

	if (h_dash >= 0.0f && h_dash < 1.0f) {
		r1 = c;
		g1 = x;
		b1 = 0.0f;
	} else if (h_dash >= 1.0f && h_dash < 2.0f) {
		r1 = x;
		g1 = c;
		b1 = 0.0f;
	} else if (h_dash >= 2.0f && h_dash < 3.0f) {
		r1 = 0.0f;
		g1 = c;
		b1 = x;
	} else if (h_dash >= 3.0f && h_dash < 4.0f) {
		r1 = 0.0f;
		g1 = x;
		b1 = c;
	} else if (h_dash >= 4.0f && h_dash < 5.0f) {
		r1 = x;
		g1 = 0.0f;
		b1 = c;
	} else {
		r1 = c;
		g1 = 0.0f;
		b1 = x;
	}

	uint8_t r = static_cast<uint8_t>((r1 + m) * 255.0f);
	uint8_t g = static_cast<uint8_t>((g1 + m) * 255.0f);
	uint8_t b = static_cast<uint8_t>((b1 + m) * 255.0f);

	return Color3B(r, g, b);
}


const char* GameToolbox::levelLengthString(int len) {
	switch (len) {
	case 1: return "Short";
	case 2: return "Medium";
	case 3: return "Long";
	case 4: return "XL";
	case 5: return "Plat.";
	default: return "Tiny";
	}
}

std::string GameToolbox::xorCipher(const std::string& message, const std::string& key) {
    std::string encryptedMessage;
    for (size_t i = 0; i < message.size(); ++i) {
        encryptedMessage += message[i] ^ key[i % key.size()];
    }
    return encryptedMessage;
}

void GameToolbox::drawFromRect(ax::Rect const&rect, ax::Color4B color, ax::DrawNode* drawNode)
{
	drawNode->drawSolidRect({rect.getMinX(), rect.getMinY()}, {rect.getMaxX(), rect.getMaxX()}, color);
}

int GameToolbox::getValueForGamemode(IconType mode) {
	switch (mode) {
	case kIconTypeCube:
		return 142;
	case kIconTypeShip:
		return 51;
	case kIconTypeBall:
		return 43;
	case kIconTypeUfo:
	case kIconTypeWave:
		return 35;
	case kIconTypeRobot:
		return 26;
	case kIconTypeSpider:
		return 17;
	case kIconTypeSwing:
		return 0;
	case kIconTypeDeathEffect:
		return 17;
	case kIconTypeSpecial:
		return 7;
	default:
		return 0;
	}
}
const char* GameToolbox::getNameGamemode(IconType mode) {
		switch (mode) {
		case kIconTypeShip:
			return "ship";
		case kIconTypeBall:
			return "player_ball";
		case kIconTypeUfo:
			return "bird";
		case kIconTypeWave:
			return "dart";
		case kIconTypeRobot:
			return "robot";
		case kIconTypeSpider:
			return "spider";
		case kIconTypeSwing:
			return "swing";
		default:
			return "player";
		}
}



Color3B GameToolbox::colorForIdx(int col) {
	switch (col) {
	case 0:
		return {125, 255, 0};
	case 1:
		return {0, 255, 0};
	case 2:
		return {0, 255, 125};
	case 3:
		return {0, 255, 255};
	case 4:
		return {0, 125, 255};
	case 5:
		return {0, 0, 255};
	case 6:
		return {125, 0, 255};
	case 7:
		return {255, 0, 255};
	case 8:
		return {255, 0, 125};
	case 9:
		return {255, 0, 0};
	case 10:
		return {255, 125, 0};
	case 11:
		return {255, 255, 0};
	case 12:
		return {255, 255, 255};
	case 13:
		return {185, 0, 255};
	case 14:
		return {255, 185, 0};
	case 15:
		return {0, 0, 0};
	case 16:
		return {0, 200, 255};
	case 17:
		return {175, 175, 175};
	case 18:
		return {90, 90, 90};
	case 19:
		return {255, 125, 125};
	case 20:
		return {0, 175, 75};
	case 21:
		return {0, 125, 125};
	case 22:
		return {0, 75, 175};
	case 23:
		return {75, 0, 175};
	case 24:
		return {125, 0, 125};
	case 25:
		return {175, 0, 75};
	case 26:
		return {175, 75, 0};
	case 27:
		return {125, 125, 0};
	case 28:
		return {75, 175, 0};
	case 29:
		return {255, 75, 0};
	case 30:
		return {150, 50, 0};
	case 31:
		return {150, 100, 0};
	case 32:
		return {100, 150, 0};
	case 33:
		return {0, 150, 100};
	case 34:
		return {0, 100, 150};
	case 35:
		return {100, 0, 150};
	case 36:
		return {150, 0, 100};
	case 37:
		return {150, 0, 0};
	case 38:
		return {0, 150, 0};
	case 39:
		return {0, 0, 150};
	case 40:
		return {125, 255, 175};
	case 41:
		return {125, 125, 255};
		// Custom colors from Sai Mod Pack (not the actual order for 2.2 colors)
	case 42:
		return {0xFA, 0xD2, 0xCD};
	case 43:
		return {0xFA, 0x3B, 0x3B};
	case 44:
		return {0x6C, 0x03, 0x02};
	case 45:
		return {0x4F, 0x04, 0x03};
	case 46:
		return {0x36, 0x01, 0x07};
	case 47:
		return {0x76, 0x4B, 0x4B};
	case 48:
		return {0x79, 0x36, 0x36};
	case 49:
		return {0x4F, 0x24, 0x24};
	case 50:
		return {0xF3, 0xB2, 0x70};
	case 51:
		return {0xFA, 0xA0, 0x42};
	case 52:
		return {0xA0, 0x62, 0x47};
	case 53:
		return {0x6E, 0x47, 0x37};
	case 54:
		return {0x54, 0x34, 0x27};
	case 55:
		return {0x66, 0x31, 0x1F};
	case 56:
		return {0x54, 0x24, 0x04};
	case 57:
		return {0x45, 0x21, 0x03};
	case 58:
		return {0xFD, 0xFD, 0xC1};
	case 59:
		return {0xF4, 0xF1, 0x82};
	case 60:
		return {0xF8, 0xDD, 0xA0};
	case 61:
		return {0x4F, 0x33, 0x10};
	case 62:
		return {0xC9, 0xAE, 0x78};
	case 63:
		return {0xA6, 0x7A, 0x50};
	case 64:
		return {0x6A, 0x51, 0x3A};
	case 65:
		return {0x45, 0x37, 0x25};
	case 66:
		return {0xB8, 0xF1, 0x98};
	case 67:
		return {0xAE, 0xF7, 0x6B};
	case 68:
		return {0xD3, 0xFD, 0x35};
	case 69:
		return {0x02, 0x5D, 0x03};
	case 70:
		return {0x03, 0x40, 0x04};
	case 71:
		return {0xBE, 0xF9, 0xDD};
	case 72:
		return {0x93, 0xFA, 0xE0};
	case 73:
		return {0x09, 0xFB, 0xBF};
	case 74:
		return {0x43, 0x98, 0x84};
	case 75:
		return {0x31, 0x69, 0x5C};
	case 76:
		return {0x26, 0x51, 0x47};
	case 77:
		return {0x04, 0x5C, 0x5C};
	case 78:
		return {0x03, 0x3E, 0x3E};
	case 79:
		return {0xA4, 0xFC, 0xFB};
	case 80:
		return {0x02, 0x07, 0x6D};
	case 81:
		return {0x02, 0x0B, 0x4B};
	case 82:
		return {0x05, 0x46, 0x68};
	case 83:
		return {0x03, 0x31, 0x4C};
	case 84:
		return {0x03, 0x25, 0x37};
	case 85:
		return {0x78, 0xBA, 0xF6};
	case 86:
		return {0x4F, 0x7B, 0xA5};
	case 87:
		return {0x32, 0x52, 0x72};
	case 88:
		return {0x35, 0x3A, 0x53};
	case 89:
		return {0xB4, 0xAB, 0xEA};
	case 90:
		return {0xAF, 0x7B, 0xF0};
	case 91:
		return {0x3C, 0x08, 0x8A};
	case 92:
		return {0x36, 0x09, 0x5E};
	case 93:
		return {0x4E, 0x4D, 0x8C};
	case 94:
		return {0x6E, 0x4B, 0xA1};
	case 95:
		return {0x53, 0x36, 0x7D};
	case 96:
		return {0x41, 0x2A, 0x62};
	case 97:
		return {0xF9, 0xB6, 0xFA};
	case 98:
		return {0x5D, 0x04, 0x38};
	case 99:
		return {0x45, 0x02, 0x33};
	case 100:
		return {0xF5, 0x81, 0xFA};
	case 101:
		return {0xAB, 0x58, 0xA9};
	case 102:
		return {0x7C, 0x42, 0x7D};
	case 103:
		return {0x5D, 0x32, 0x5D};
	case 104:
		return {0xE0, 0xE0, 0xE0};
	case 105:
		return {0x7F, 0x7F, 0x7F};
	case 106:
		return {0x3F, 0x3F, 0x3F};
	default:
		return { 255, 255, 255 }; // blanco
	}
}

