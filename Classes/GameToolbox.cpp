#include "GameToolbox.h"
#include <fmt/format.h>
#include <fstream>
#include <zlib.h>
#include <zlib.h>

USING_NS_AX;

bool _showDebugImgui = true;

//general purpose class for helper functions that can be useful anywhere

int GameToolbox::randomInt(int min, int max)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<int> distribution(min, max);
	
	return distribution(generator);
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

ax::Color3B GameToolbox::randomColor3B()
{
	uint8_t r = GameToolbox::randomInt(255);
	uint8_t g = GameToolbox::randomInt(255);
	uint8_t b = GameToolbox::randomInt(255);
	
	return {r, g, b};
}

ax::BlendFunc GameToolbox::getBlending()
{
	return BlendFunc::ADDITIVE;
}

void GameToolbox::createCorners(ax::Node* self, bool topLeft, bool topRight, bool botLeft, bool botRight)
{
	Sprite* corner = nullptr;
	auto winSize = Director::getInstance()->getWinSize();
	if(botLeft)
	{
		corner = Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
		corner->setStretchEnabled(false);
		corner->setPosition({0, 0});
		corner->setAnchorPoint({0.0, 0.0});
		self->addChild(corner);
	}
	
	if(topLeft)
	{
		corner = Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
		corner->setStretchEnabled(false);
		corner->setPosition({0, winSize.height});
		corner->setAnchorPoint({0, 1});
		corner->setFlippedY(true);
		self->addChild(corner);
	}
	
	if(topRight)
	{
		corner = Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
		corner->setStretchEnabled(false);
		corner->setPosition(winSize);
		corner->setAnchorPoint({1, 1});
		corner->setFlippedX(true);
		corner->setFlippedY(true);
		self->addChild(corner);
	}
	
	if(botRight)
	{
		corner = Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
		corner->setStretchEnabled(false);
		corner->setPosition({winSize.width, 0});
		corner->setAnchorPoint({1, 0});
		corner->setFlippedX(true);
		self->addChild(corner);
	}
}
//if low or no "." found, return existing
//if high, add -uhd otherwise add -hd (medium default)
//take string by copy, reference or string_view is not possible because we actually modify the string

static inline std::string getTextureString_AppendResources(std::string texture) {
	std::string nTexture = "Resources/";
	nTexture += texture;
	bool low = false;
	bool medium = true;
	bool high = false;

	size_t pos = nTexture.find(".");
	if(!low && pos != std::string::npos) {
		nTexture.insert(pos, high ? "-uhd" : "-hd");
	}
	//GameToolbox::log("texture: {}", nTexture);
	return nTexture;
}

static inline std::string getTextureString_WithoutResources(std::string texture)
{
	bool low = false;
	bool medium = true;
	bool high = false;

	size_t pos = texture.find(".");
	if(!low && pos != std::string::npos) {
		texture.insert(pos, high ? "-uhd" : "-hd");
	}
	//GameToolbox::log("texture: {}", texture);
	return texture;
}
std::string GameToolbox::getTextureString(std::string texture)
{ 
	// // this code would try to access texture using six different ways. if last way fails, this function returns '-' for error handling

	// // attempt to create sprite without resources path (sprite frame name and without it)
	// ax::Sprite *spr = ax::Sprite::create(getTextureString_WithoutResources(texture));
	// if(!spr) {
	//	 // not found, try with sprite frame name
	//	 spr = ax::Sprite::createWithSpriteFrameName(getTextureString_WithoutResources(texture));
	//	 if(!spr) {
	//		 // not found, switch to resources path attempt
	//		 goto withResourcesPath;
	//	 } else {
	//		 spr->release();
	//		 return getTextureString_WithoutResources(texture);
	//	 }
	// } else {
	//	 return getTextureString_WithoutResources(texture);
	// }

	// withResourcesPath:
	// // attempt to create sprite with resources path (sprite frame name and without it)
	// spr = ax::Sprite::create(getTextureString_AppendResources(texture));
	// if(!spr) {
	//	 // not found, try with sprite frame name
	//	 spr = ax::Sprite::createWithSpriteFrameName(getTextureString_AppendResources(texture));
	//	 if(!spr) {
	//		 goto tryDifferentFormats;
	//	 } else {
	//		 spr->release();
	//		 return getTextureString_AppendResources(texture);
	//	 }
	// } else {
	//	 spr->release();
	//	 return getTextureString_AppendResources(texture);
	// }

	// tryDifferentFormats:
	// // every way was checked with no result, check if it plist file
	// if(texture.ends_with(".plist") || texture.ends_with(".fnt")) {
	//	 #if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32) || (AX_TARGET_PLATFORM == AX_PLATFORM_LINUX)
	//		 // try to find requested file
	//		 std::string s1 = getTextureString_WithoutResources(texture);
	//		 std::string s2 = getTextureString_AppendResources(texture);

	//		 std::ifstream f1(s1);
	//		 std::ifstream f2(s2);

	//		 if(f1.good()) return s1;
	//		 if(f2.good()) return s2;

	//		 GameToolbox::log("File not found: -");
	//		 return "-";
			
	//	 #else
	//		 return getTextureString_WithoutResources(texture);
	//	 #endif
	// } else {
	//	 // return '-' for error handling
	//	 GameToolbox::log("Worst case: -");
	//	 return "-";
	// }
	return getTextureString_WithoutResources(texture);
}


void GameToolbox::alignItemsInColumnsWithPadding(ax::Menu* menu, const int rows, const int x_padding, const int y_padding)
{
	// Get the menu items and calculate the total number of items
	const auto items = menu->getChildren();
	const int totalItems = static_cast<int>(items.size());

	// Calculate the number of rows and the maximum number of items per row
	const int columns = (totalItems + rows - 1) / rows;
	const int itemsPerRow = (totalItems + rows - 1) / rows;

	// Calculate the starting position for the menu items
	const float startX = -(columns - 1) * (x_padding / 2.0f);
	const float startY = (rows - 1) * (y_padding / 2.0f);

	// Loop through each row and position the items in it
	for (int row = 0; row < rows; row++)
	{
		// Calculate the y position for the items in this row
		const float yPos = startY - (row * y_padding);

		// Loop through each item in this row and position it
		for (int column = 0; column < columns; column++)
		{
			// Calculate the index of the item in the menu items array
			const int index = (row * columns) + column;
			GameToolbox::log("index: {}", index);

			// Check if this index is valid for the menu items array
			if (index < totalItems)
			{
				// Get the item and set its position
				auto item = dynamic_cast<ax::MenuItem*>(items.at(index));
				const float xPos = startX + (column * x_padding);
				item->setPosition(ax::Vec2(xPos, yPos));
			}
			else
			{
				// We've reached the end of the items, so we break out of the loop
				break;
			}
		}
	}
}

void GameToolbox::alignItemsHorizontallyWithPadding(ax::Vector<ax::Node*> _children, float padding)
{
	float width = -padding;
	for (const auto& child : _children)
		width += child->getContentSize().width * child->getScaleX() + padding;

	float x = -width / 2.0f;

	for (const auto& child : _children)
	{
		child->setPosition(x + child->getContentSize().width * child->getScaleX() / 2.0f, 0);
		x += child->getContentSize().width * child->getScaleX() + padding;
	}
}

void GameToolbox::alignItemsVerticallyWithPadding(ax::Vector<ax::Node*> _children, float padding)
{
	float height = -padding;

	for (const auto& child : _children)
		height += child->getContentSize().height * child->getScaleY() + padding;

	float y = height / 2.0f;

	for (const auto& child : _children)
	{
		child->setPosition(0, y - child->getContentSize().height * child->getScaleY() / 2.0f);
		y -= child->getContentSize().height * child->getScaleY() + padding;
	}
}
float GameToolbox::repeat(float a, float length)
{
	return std::clamp(a - floorf(a / length) * length, 0.0f, length);
}
float GameToolbox::SquareDistance(float xa, float ya, float xb, float yb)
{
	return ((xb - xa) * (xb - xa)) + ((yb - ya) * (yb - ya));
}
float GameToolbox::SquareDistance(ax::Vec2 a, ax::Vec2 b)
{
	return SquareDistance(a.x, a.y, b.x, b.y);
}
float GameToolbox::slerp(float a, float b, float ratio)
{
	float delta = repeat((b - a), 360.f);
	if (delta > 180.f)
		delta -= 360.f;
	return a + delta * std::clamp(ratio, 0.f, 1.f);
}
float GameToolbox::iLerp(float a, float b, float ratio, float dt)
{
	const float rDelta = dt / (1.f / 60.f);
	const float s = 1.f - ratio;

	float iRatio = 1.f - powf(s, rDelta);

	return MathUtil::lerp(a, b, iRatio);
}
float GameToolbox::iSlerp(float a, float b, float ratio, float dt)
{
	const float rDelta = dt / (1.f / 60.f);
	const float s = 1.f - ratio;

	float iRatio = 1.f - powf(s, rDelta);

	return slerp(a, b, iRatio);
}

std::optional<std::string> GameToolbox::getResponse(ax::network::HttpResponse* response)
{
	if(!response) 
		return std::nullopt;
	
	int code = response->getResponseCode();
	GameToolbox::log("response code: {}", code);
	
	auto buffer = response->getResponseData();
	std::string ret {buffer->begin(), buffer->end()};
	
	if(code != 200)
	{
		if(!ret.empty())
			GameToolbox::log("recieved error: {}", ret);
		return std::nullopt;
	}
	
	return std::optional<std::string>{ret};
}


std::vector<std::string> GameToolbox::splitByDelim(const std::string& s, char delim)
{
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(std::move(item));
	}
	return elems;
};



#define BUFFER_INC_FACTOR (2)

int GameToolbox::ccInflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int *outLength, unsigned int outLenghtHint)
{
	/* ret value */
	int err = Z_OK;
	
	int bufferSize = outLenghtHint;
	*out = new unsigned char[bufferSize];
	
	z_stream d_stream; /* decompression stream */
	d_stream.zalloc = (alloc_func)0;
	d_stream.zfree = (free_func)0;
	d_stream.opaque = (voidpf)0;
	
	d_stream.next_in  = in;
	d_stream.avail_in = inLength;
	d_stream.next_out = *out;
	d_stream.avail_out = bufferSize;
	
	/* window size to hold 256k */
	if( (err = inflateInit2(&d_stream, 15 + 32)) != Z_OK )
		return err;
	
	for (;;)
	{
		err = inflate(&d_stream, Z_NO_FLUSH);
		
		if (err == Z_STREAM_END)
		{
			break;
		}
		
		switch (err)
		{
			case Z_NEED_DICT:
				err = Z_DATA_ERROR;
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				inflateEnd(&d_stream);
				return err;
		}
		
		// not enough memory ?
		if (err != Z_STREAM_END)
		{
			*out = (unsigned char*)realloc(*out, bufferSize * BUFFER_INC_FACTOR);
			
			/* not enough memory, ouch */
			if (! *out )
			{
				GameToolbox::log("cocos2d: ZipUtils: realloc failed");
				inflateEnd(&d_stream);
				return Z_MEM_ERROR;
			}
			
			d_stream.next_out = *out + bufferSize;
			d_stream.avail_out = bufferSize;
			bufferSize *= BUFFER_INC_FACTOR;
		}
	}
	
	*outLength = bufferSize - d_stream.avail_out;
	err = inflateEnd(&d_stream);
	return err;
}
	
int GameToolbox::ccInflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int outLengthHint)
{
	unsigned int outLength = 0;
	int err = ccInflateMemoryWithHint(in, inLength, out, &outLength, outLengthHint);
	
	if (err != Z_OK || *out == NULL) {
		if (err == Z_MEM_ERROR)
		{
			GameToolbox::log("cocos2d: ZipUtils: Out of memory while decompressing map data!");
		} else
			if (err == Z_VERSION_ERROR)
			{
				GameToolbox::log("cocos2d: ZipUtils: Incompatible zlib version!");
			} else
				if (err == Z_DATA_ERROR)
				{
					GameToolbox::log("cocos2d: ZipUtils: Incorrect zlib compressed data!");
				}
				else
				{
					GameToolbox::log("cocos2d: ZipUtils: Unknown error while decompressing map data!");
				}
		
		delete[] *out;
		*out = NULL;
		outLength = 0;
	}
	
	return outLength;
}

int GameToolbox::ccInflateMemory(unsigned char *in, unsigned int inLength, unsigned char **out)
{
	// 256k for hint
	return ccInflateMemoryWithHint(in, inLength, out, 256 * 1024);
}

std::string GameToolbox::getClipboardString()
{
	#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32)
		if (!OpenClipboard(nullptr)) return "";

		HANDLE hData = GetClipboardData(CF_TEXT);
		if (hData == nullptr) return "";

		char* pszText = static_cast<char*>(GlobalLock(hData));
		if (pszText == nullptr) return "";

		std::string text(pszText);
		GlobalUnlock(hData);
		CloseClipboard();
		return text;
	#else
		return "";
	#endif
}