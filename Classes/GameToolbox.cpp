#include "GameToolbox.h"
#include <fmt/format.h>
#include <zlib.h>
#include <fstream>

USING_NS_AX;

bool _showDebugImgui = true;

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

#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32)
#	include <windows.h>
#endif
#include <ResourcesLoadingLayer.h>
std::string GameToolbox::getSteamGamePathByAppID(int appID, std::string exeName) {
#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32)

	// Find Steam Installation Path
	std::string steamInstallPath = "";

	std::string steamInstallPathSubKey = "SOFTWARE\\WOW6432Node\\Valve\\Steam\\";
	std::string steamInstallPathValue  = "InstallPath";

	size_t bufferSize = 0xFFF;
	std::string valueBuf;
	valueBuf.resize(bufferSize);
	auto cbData = static_cast<DWORD>(bufferSize * sizeof(char));
	auto rc	 = RegGetValueA(HKEY_LOCAL_MACHINE, steamInstallPathSubKey.c_str(), steamInstallPathValue.c_str(),
							   RRF_RT_REG_SZ, nullptr, static_cast<void*>(valueBuf.data()), &cbData);
	while (rc == ERROR_MORE_DATA) {
		// Get a buffer that is big enough.
		cbData /= sizeof(char);
		if (cbData > static_cast<DWORD>(bufferSize)) {
			bufferSize = static_cast<size_t>(cbData);
		} else {
			bufferSize *= 2;
			cbData = static_cast<DWORD>(bufferSize * sizeof(char));
		}
		valueBuf.resize(bufferSize);
		rc = RegGetValueA(HKEY_LOCAL_MACHINE, steamInstallPathSubKey.c_str(), steamInstallPathValue.c_str(),
						  RRF_RT_REG_SZ, nullptr, static_cast<void*>(valueBuf.data()), &cbData);
	}
	if (rc == ERROR_SUCCESS) {
		cbData /= sizeof(char);
		valueBuf.resize(static_cast<size_t>(cbData - 1));  // remove end null character

		steamInstallPath = valueBuf;
	} else
		return "";

	if (steamInstallPath.empty())
		return "";

	// Actual game path finding
	std::string steamappsPath		 = fmt::format("{}\\steamapps", steamInstallPath);
	std::string libraryFoldersVDFPath = fmt::format("{}\\libraryfolders.vdf", steamappsPath);

	std::vector<std::string> paths{steamappsPath};

	std::regex regex("\\s*\"(?:path)\"\\s+\"(.+)\"");

	std::ifstream libraryFoldersVDF(libraryFoldersVDFPath);
	std::string line;

	std::smatch match;

	while (std::getline(libraryFoldersVDF, line)) {
		if (std::regex_match(line, match, regex))
			paths.push_back(fmt::format("{}\\steamapps", match[1].str()));
	}

	line  = "";
	regex = ("\\s\"installdir\"\\s+\"(.+)\"");
	for (auto path : paths) {
		std::string appManifestPath = fmt::format("{}\\appmanifest_{}.acf", path, appID);
		if (std::filesystem::exists(appManifestPath)) {
			std::ifstream appManifest(appManifestPath);
			std::string commonPath;
			while (std::getline(appManifest, line))
				if (std::regex_match(line, match, regex)) {
					commonPath = fmt::format("{}\\common\\{}", path, match[1].str());
					if (std::filesystem::exists(fmt::format("{}\\{}", commonPath, exeName)))
						return std::regex_replace(commonPath, std::regex("\\\\\\\\"), "\\");
				}
		}
	}
#endif
	return "";
}

// if low or no "." found, return existing
// if high, add -uhd otherwise add -hd (medium default)
// take string by copy, reference or string_view is not possible because we actually modify the string

static inline std::string getTextureString_AppendResources(std::string texture) {
	std::string nTexture = "Resources/";
	nTexture += texture;
	bool medium = false;
	bool high   = true;

	auto pos = nTexture.find(".");
	if (pos != std::string::npos) {
		nTexture.insert(pos, high ? "-uhd" : "-hd");
	}
	// GameToolbox::log("texture: {}", nTexture);
	return nTexture;
}

static inline std::string getTextureString_WithoutResources(std::string texture) {
	bool medium = false;
	bool high   = true;

	auto pos = texture.find(".");
	if (pos != std::string::npos) {
		texture.insert(pos, high ? "-uhd" : "-hd");
	}
	// GameToolbox::log("texture: {}", texture);
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

	int code = response->getResponseCode();
	GameToolbox::log("response code: {}", code);

	auto buffer = response->getResponseData();
	std::string ret{buffer->begin(), buffer->end()};

	if (code != 200) {
		if (!ret.empty())
			GameToolbox::log("recieved error: {}", ret);
		return std::nullopt;
	}

	return std::optional<std::string>{ret};
}

std::vector<std::string> GameToolbox::splitByDelim(const std::string& s, char delim) {
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim)) {
		elems.push_back(std::move(item));
	}
	return elems;
};

#define BUFFER_INC_FACTOR (2)

int GameToolbox::ccInflateMemoryWithHint(unsigned char* in,
										 unsigned int inLength,
										 unsigned char** out,
										 unsigned int* outLength,
										 unsigned int outLenghtHint) {
	/* ret value */
	int err = Z_OK;

	int bufferSize = outLenghtHint;
	*out		   = new unsigned char[bufferSize];

	z_stream d_stream; /* decompression stream */
	d_stream.zalloc = (alloc_func)0;
	d_stream.zfree  = (free_func)0;
	d_stream.opaque = (voidpf)0;

	d_stream.next_in   = in;
	d_stream.avail_in  = inLength;
	d_stream.next_out  = *out;
	d_stream.avail_out = bufferSize;

	/* window size to hold 256k */
	if ((err = inflateInit2(&d_stream, 15 + 32)) != Z_OK)
		return err;

	for (;;) {
		err = inflate(&d_stream, Z_NO_FLUSH);

		if (err == Z_STREAM_END) {
			break;
		}

		switch (err) {
		case Z_NEED_DICT:
			err = Z_DATA_ERROR;
		case Z_DATA_ERROR:
		case Z_MEM_ERROR:
			inflateEnd(&d_stream);
			return err;
		}

		// not enough memory ?
		if (err != Z_STREAM_END) {
			*out = (unsigned char*)realloc(*out, bufferSize * BUFFER_INC_FACTOR);

			/* not enough memory, ouch */
			if (!*out) {
				GameToolbox::log("cocos2d: ZipUtils: realloc failed");
				inflateEnd(&d_stream);
				return Z_MEM_ERROR;
			}

			d_stream.next_out  = *out + bufferSize;
			d_stream.avail_out = bufferSize;
			bufferSize *= BUFFER_INC_FACTOR;
		}
	}

	*outLength = bufferSize - d_stream.avail_out;
	err		= inflateEnd(&d_stream);
	return err;
}

int GameToolbox::ccInflateMemoryWithHint(unsigned char* in,
										 unsigned int inLength,
										 unsigned char** out,
										 unsigned int outLengthHint) {
	unsigned int outLength = 0;
	int err				= ccInflateMemoryWithHint(in, inLength, out, &outLength, outLengthHint);

	if (err != Z_OK || *out == NULL) {
		if (err == Z_MEM_ERROR) {
			GameToolbox::log("cocos2d: ZipUtils: Out of memory while decompressing map data!");
		} else if (err == Z_VERSION_ERROR) {
			GameToolbox::log("cocos2d: ZipUtils: Incompatible zlib version!");
		} else if (err == Z_DATA_ERROR) {
			GameToolbox::log("cocos2d: ZipUtils: Incorrect zlib compressed data!");
		} else {
			GameToolbox::log("cocos2d: ZipUtils: Unknown error while decompressing map data!");
		}

		delete[] * out;
		*out	  = NULL;
		outLength = 0;
	}

	return outLength;
}

int GameToolbox::ccInflateMemory(unsigned char* in, unsigned int inLength, unsigned char** out) {
	// 256k for hint
	return ccInflateMemoryWithHint(in, inLength, out, 256 * 1024);
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