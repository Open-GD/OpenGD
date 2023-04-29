#pragma once

#include <string>

#include "2d/CCLayer.h"
#include "math/Vec2.h"

class GameManager;
namespace ax 
{ 
	class RepeatForever; 
	class Director;
	class FileUtils;
	class Scene;
}

class ResourcesLoadingLayer : public ax::Layer
{
private:
	ax::RepeatForever* _gdProcessAction;
	ax::Director* _dir;
	ax::FileUtils* _fu;
	GameManager* _gm;
	std::string _writablePath;
	ax::Vec2 _posMiddle;

	void handleWindows();
	bool isWindowsGDPathValid(std::string exepath);
	static std::string getRunningGDPathWindows();

	void handleMac();
	void handleLinux();
	
public:
	void loadLoadingLayer();
	bool init();
	static ResourcesLoadingLayer* create();
	static ax::Scene* scene();
};