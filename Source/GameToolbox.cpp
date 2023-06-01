#include "GameToolbox.h"
#include <fmt/format.h>
#include <fstream>
#include "AltDirector.h"
#include "GameManager.h"
#include "external/fast_float.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"
#include "2d/CCTransition.h"
#include "2d/CCSprite.h"
#include "2d/CCLabel.h"
#include "2d/CCMenu.h"
#include "math/MathUtil.h"
#include "platform/CCFileUtils.h"

USING_NS_AX;

bool _showDebugImgui = true;

void GameToolbox::popSceneWithTransition(float duration, popTransition type) {
	auto dir = AltDirector::getInstance();

	if (dir->getRunningScene() != nullptr) {
		dir->popScene();

		unsigned int c = dir->getScenesStack().size();
		if (c == 0) dir->end();
		else {
			dir->setSendCleanupToScene(true);
			auto scene = dir->getScenesStack().back();

			switch (type) {
			case kTransitionShop:
				dir->replaceScene(TransitionMoveInB::create(duration, scene)); // i will change this transition later / att: Sai
				break;
			default:
				dir->replaceScene(TransitionFade::create(duration, scene));
				break;
			}
		}
	}
}

void GameToolbox::limitLabelWidth(ax::Label* label, float width, float normalScale, float minScale) {
	float val = width / label->getContentSize().width;
	label->setScale(val > normalScale ? normalScale : val < minScale ? minScale : val);
}

// no more "GameToolbox::getTextureString" in Label::createWithBMFont
Label* GameToolbox::createBMFont(std::string text, std::string font, int width, TextHAlignment alignment) {
	return Label::createWithBMFont(GameToolbox::getTextureString(font), text, alignment, width);
}
Label* GameToolbox::createBMFont(std::string text, std::string font, int width) {
	return createBMFont(text, font, width, TextHAlignment::LEFT);
}
Label* GameToolbox::createBMFont(std::string text, std::string font) {
	return createBMFont(text, font, 0, TextHAlignment::LEFT);
}

const char* GameToolbox::lengthString(int len) {
	switch (len) {
	case 1: return "Short";
	case 2: return "Medium";
	case 3: return "Long";
	case 4: return "XL";
	case 5: return "Plat.";
	default: return "Tiny";
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

int GameToolbox::randomInt(int min, int max) {
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<int> distribution(min, max);

	return distribution(generator);
}

void GameToolbox::createBG(ax::Node* self, const ax::Color3B color) {
	auto winSize = Director::getInstance()->getWinSize();
	auto bg = Sprite::create("GJ_gradientBG.png");
	bg->setStretchEnabled(false);
	bg->setScaleX(winSize.width / bg->getContentSize().width);
	bg->setScaleY(winSize.height / bg->getContentSize().height);
	bg->setAnchorPoint({ 0, 0 });
	bg->setColor(color);
	self->addChild(bg);
}

void GameToolbox::createBG(ax::Node* self) {
	return createBG(self, { 0, 102, 255 });
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
int GameToolbox::randomInt(int max) {
	return GameToolbox::randomInt(0, max);
}
float GameToolbox::randomFloat(int min, int max) {
	return static_cast<float>(GameToolbox::randomInt(min, max));
}
float GameToolbox::randomFloat(int max) {
	return static_cast<float>(GameToolbox::randomInt(max));
}

ax::Color3B GameToolbox::randomColor3B() {
	uint8_t r = GameToolbox::randomInt(255);
	uint8_t g = GameToolbox::randomInt(255);
	uint8_t b = GameToolbox::randomInt(255);

	return {r, g, b};
}

ax::BlendFunc GameToolbox::getBlending() {
	return BlendFunc::ADDITIVE;
}

void GameToolbox::createCorners(ax::Node* self, bool topLeft, bool topRight, bool botLeft, bool botRight) {
	Sprite* corner = nullptr;
	auto winSize = Director::getInstance()->getWinSize();
	if (botLeft) {
		corner = Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
		corner->setStretchEnabled(false);
		corner->setPosition({ 0, 0 });
		corner->setAnchorPoint({ 0.0, 0.0 });
		self->addChild(corner);
	}

	if (topLeft) {
		corner = Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
		corner->setStretchEnabled(false);
		corner->setPosition({ 0, winSize.height });
		corner->setAnchorPoint({ 0, 1 });
		corner->setFlippedY(true);
		self->addChild(corner);
	}

	if (topRight) {
		corner = Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
		corner->setStretchEnabled(false);
		corner->setPosition(winSize);
		corner->setAnchorPoint({ 1, 1 });
		corner->setFlippedX(true);
		corner->setFlippedY(true);
		self->addChild(corner);
	}

	if (botRight) {
		corner = Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
		corner->setStretchEnabled(false);
		corner->setPosition({ winSize.width, 0 });
		corner->setAnchorPoint({ 1, 0 });
		corner->setFlippedX(true);
		self->addChild(corner);
	}
}

// if low or no "." found, return existing
// if high, add -uhd otherwise add -hd (medium default)
// take string by copy, reference or string_view is not possible because we actually modify the string

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

void GameToolbox::alignItemsInColumnsWithPadding(ax::Menu* menu,
												 const int rows,
												 const int x_padding,
												 const int y_padding) {
	// Get the menu items and calculate the total number of items
	const auto items	 = menu->getChildren();
	const int totalItems = static_cast<int>(items.size());

	// Calculate the number of rows and the maximum number of items per row
	const int columns	 = (totalItems + rows - 1) / rows;
	const int itemsPerRow = (totalItems + rows - 1) / rows;

	// Calculate the starting position for the menu items
	const float startX = -(columns - 1) * (x_padding / 2.0f);
	const float startY = (rows - 1) * (y_padding / 2.0f);

	// Loop through each row and position the items in it
	for (int row = 0; row < rows; row++) {
		// Calculate the y position for the items in this row
		const float yPos = startY - (row * y_padding);

		// Loop through each item in this row and position it
		for (int column = 0; column < columns; column++) {
			// Calculate the index of the item in the menu items array
			const int index = (row * columns) + column;
			GameToolbox::log("index: {}", index);

			// Check if this index is valid for the menu items array
			if (index < totalItems) {
				// Get the item and set its position
				auto item		= dynamic_cast<ax::MenuItem*>(items.at(index));
				const float xPos = startX + (column * x_padding);
				item->setPosition(ax::Vec2(xPos, yPos));
			} else {
				// We've reached the end of the items, so we break out of the loop
				break;
			}
		}
	}
}

void GameToolbox::alignItemsHorizontally(ax::Vector<Node*> children, float padding, Point location)
{
	auto totalWidth = -padding;

	if (children.size())
	{
		for (Node* child : children)
		{
			totalWidth += padding + (child->getContentSize().width * child->getScaleX());
		}

		float xPos = -(float)(totalWidth * 0.5);
		for (Node* child : children)
		{
			auto width = child->getContentSize().width;
			auto scale = child->getScaleX();

			child->setPosition({ xPos + (width * scale) * 0.5f, 0 });
			child->setPosition(child->getPosition() + location);

			xPos = xPos + (float)(padding + (float)(width * child->getScaleX()));
		}
	}
}

void GameToolbox::alignItemsHorizontallyWithPadding(ax::Vector<ax::Node*> _children, float padding) {
	float width = -padding;
	for (const auto& child : _children)
		width += child->getContentSize().width * child->getScaleX() + padding;

	float x = -width / 2.0f;

	for (const auto& child : _children) {
		child->setPosition(x + child->getContentSize().width * child->getScaleX() / 2.0f, 0);
		x += child->getContentSize().width * child->getScaleX() + padding;
	}
}

void GameToolbox::alignItemsVerticallyWithPadding(ax::Vector<ax::Node*> _children, float padding) {
	float height = -padding;

	for (const auto& child : _children)
		height += child->getContentSize().height * child->getScaleY() + padding;

	float y = height / 2.0f;

	for (const auto& child : _children) {
		child->setPosition(0, y - child->getContentSize().height * child->getScaleY() / 2.0f);
		y -= child->getContentSize().height * child->getScaleY() + padding;
	}
}
float GameToolbox::repeat(float a, float length) {
	return std::clamp(a - floorf(a / length) * length, 0.0f, length);
}
float GameToolbox::SquareDistance(float xa, float ya, float xb, float yb) {
	return ((xb - xa) * (xb - xa)) + ((yb - ya) * (yb - ya));
}
float GameToolbox::SquareDistance(ax::Vec2 a, ax::Vec2 b) {
	return SquareDistance(a.x, a.y, b.x, b.y);
}
float GameToolbox::slerp(float a, float b, float ratio) {
	float delta = repeat((b - a), 360.f);
	if (delta > 180.f)
		delta -= 360.f;
	return a + delta * std::clamp(ratio, 0.f, 1.f);
}
float GameToolbox::iLerp(float a, float b, float ratio, float dt) {
	const float rDelta = dt / (1.f / 60.f);
	const float s	  = 1.f - ratio;

	float iRatio = 1.f - powf(s, rDelta);

	return MathUtil::lerp(a, b, iRatio);
}
float GameToolbox::iSlerp(float a, float b, float ratio, float dt) {
	const float rDelta = dt / (1.f / 60.f);
	const float s	  = 1.f - ratio;

	float iRatio = 1.f - powf(s, rDelta);

	return slerp(a, b, iRatio);
}

std::optional<std::string> GameToolbox::getResponse(ax::network::HttpResponse* response) {
	if (!response)
		return std::nullopt;

	auto buffer = response->getResponseData();
	std::string ret{buffer->begin(), buffer->end()};

	int code = response->getResponseCode();
	GameToolbox::log("response code: {}", code);

	bool robtopError = ( ret.size() > 1 && ret[0] == '-' && std::isdigit(ret[1]) );
	if (code != 200 || robtopError)
	{
		if (!ret.empty()) {
			GameToolbox::log("recieved error: {}", ret);
		}
		return std::nullopt;
	}

	return ret == "-1" ? std::nullopt : std::optional<std::string>{ret};
}

/*
std::vector<std::string_view> _splitByDelimView(const std::string_view str, char delim)
{
	std::vector<std::string_view> tokens;
	size_t pos = 0;
	size_t len = str.length();

	while (pos < len) {
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
};
*/

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

void GameToolbox::executeHttpRequest(const std::string& url, const std::string& postData, ax::network::HttpRequest::Type type, const ax::network::ccHttpRequestCallback& callback)
{
	ax::network::HttpRequest* request = new ax::network::HttpRequest();
	request->setUrl(url);
	request->setRequestType(type);
	request->setHeaders(std::vector<std::string>{"User-Agent: "});
	request->setRequestData(postData.c_str(), postData.length());
	request->setResponseCallback(callback);
	ax::network::HttpClient::getInstance()->send(request);
	request->release();
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

//auto arg works kind of like a template, will compile if .data() and .size() is available
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

std::string GameToolbox::getFileContentsResources(std::string_view file) {
	return ax::FileUtils::getInstance()->getStringFromFile(file);
}

std::string GameToolbox::xorCipher(const std::string& message, const std::string& key) {
    std::string encryptedMessage;
    for (size_t i = 0; i < message.size(); ++i) {
        encryptedMessage += message[i] ^ key[i % key.size()];
    }
    return encryptedMessage;
}