#pragma once
#include <axmol.h>
#include <string>
#include <fmt/chrono.h>
#include <optional>
#include <network/HttpClient.h>

#include <regex>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <string>

#include <vector>

enum IconType {
	kIconTypeCube		= 0,
	kIconTypeShip		= 1,
	kIconTypeBall		= 2,
	kIconTypeUfo		 = 3,
	kIconTypeWave		= 4,
	kIconTypeRobot	   = 5,
	kIconTypeSpider	  = 6,
	kIconTypeSwing	   = 7,
	kIconTypeDeathEffect = 98,
	kIconTypeSpecial	 = 99,
};

enum popTransition {
	kTransitionFade = 0,
	kTransitionShop
};

//general purpose namespace for helper functions that can be useful with any class at any moment
namespace GameToolbox
{
	enum GTRes {
		Low = 0,
		Medium,
		High
	};

	extern std::vector<std::string> _uhdTextureIgnoreList;


	static bool _isCtrlPressed;

	void popSceneWithTransition(float time, popTransition type = kTransitionFade);
	void limitLabelWidth(ax::Label* label, float width, float normalScale, float minScale = 0);
	ax::Label* createBMFont(std::string text, std::string font);
	ax::Label* createBMFont(std::string text, std::string font, int width);
	ax::Label* createBMFont(std::string text, std::string font, int width, ax::TextHAlignment alignment);
	const char* lengthString(int len);
	int randomInt(int min, int max);
	int randomInt(int max);
	float randomFloat(int min, int max);
	float randomFloat(int max);
	
	ax::Color3B colorForIdx(int col);
		const char* getNameGamemode(IconType mode);
		int getValueForGamemode(IconType mode);

	ax::Color3B randomColor3B();
	std::string getTextureString(std::string texture);
	void alignItemsInColumnsWithPadding(ax::Menu* menu, const int rows, const int x_padding, const int y_padding);
	void alignItemsVerticallyWithPadding(ax::Vector<ax::Node*> children, float padding);
	void alignItemsHorizontallyWithPadding(ax::Vector<ax::Node*> children, float padding);
		void createBG(ax::Node* layer, ax::Color3B color);
		void createBG(ax::Node* layer);
	void createCorners(ax::Node* layer, bool topRight, bool topLeft, bool botLeft, bool botRight);
	inline void createAllCorners(ax::Node* self) { return createCorners(self, true, true, true, true); }
	inline std::string getFileContentsResources(std::string_view file) { return ax::FileUtils::getInstance()->getStringFromFile(file); }
	
	float SquareDistance(float xa, float ya, float xb, float yb);
	float SquareDistance(ax::Vec2 a, ax::Vec2 b);

	float iLerp(float a, float b, float ratio, float dt);

	float slerp(float a, float b, float ratio);
	float iSlerp(float a, float b, float ratio, float dt);
	float repeat(float a, float length);
	
	// windows only lmaoooo
	std::string getSteamGamePathByAppID(int appID, std::string exeName);

	std::optional<std::string> getResponse(ax::network::HttpResponse* response);
	std::vector<std::string> splitByDelim(const std::string& s, char delim);

	void executeHttpRequest(const std::string& url, const std::string& postData, ax::network::HttpRequest::Type type, const ax::network::ccHttpRequestCallback& callback);
	
	int ccInflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int *outLength, unsigned int outLenghtHint);
	int ccInflateMemoryWithHint(unsigned char *in, unsigned int inLength, unsigned char **out, unsigned int outLengthHint);
	int ccInflateMemory(unsigned char *in, unsigned int inLength, unsigned char **out);
	
	ax::BlendFunc getBlending();

	template <typename T>
	inline T inRange(T value, T min, T max) {
		return MIN(MAX(value, min), max);
	}
	
	template <typename... T>
		void log(fmt::format_string<T...> fmt, T&&... args) {
		return fmt::print("[{:%H:%M:%S}] {}\n", fmt::gmtime(std::time(NULL)), fmt::format(fmt, std::forward<T>(args)...));
	}

	bool hasStringInVector(std::vector<std::string> vect, std::string val);
};
