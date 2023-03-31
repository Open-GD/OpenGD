#pragma once

#include <axmol.h>

class GameManager;

class ResourcesLoadingLayer : public ax::Layer
{
private:
	ax::RepeatForever* _gdProcessAction;
	ax::Director* _dir;
	ax::FileUtils* _fu;
	GameManager* _gm;
	std::string _writablePath;
	ax::Vec2 _posMiddle;

#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32)
	void handleWindows();
	bool isWindowsGDPathValid(std::string exepath);
	static std::string getRunningGDPathWindows();
#endif

#if (AX_TARGET_PLATFORM == AX_PLATFORM_MAC)
	void handleMac();
#endif

#if (AX_TARGET_PLATFORM == AX_PLATFORM_LINUX)
	void handleLinux();
#endif





public:
	void loadLoadingLayer();
	bool init();
	static ResourcesLoadingLayer* create();
	static ax::Scene* scene();
};