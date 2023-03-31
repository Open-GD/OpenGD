#include "GameToolbox.h"
#include "ResourcesLoadingLayer.h"
#include <LoadingLayer.h>

USING_NS_AX;



ResourcesLoadingLayer* ResourcesLoadingLayer::create() {
	ResourcesLoadingLayer* pRet = new ResourcesLoadingLayer();
	if (pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

Scene* ResourcesLoadingLayer::scene()
{
	auto scene = ax::Scene::create();
	scene->addChild(ResourcesLoadingLayer::create());
	return scene;
}

bool ResourcesLoadingLayer::checkPath(std::string path)
{
	if (path.empty())
		return false;
	if (!std::filesystem::exists(path))
		return false;
	else
	{
		bool check = true;
		check &= std::filesystem::exists(fmt::format("{}\\GeometryDash.exe", path));
		check &= std::filesystem::exists(fmt::format("{}\\Resources\\", path));
		check &= std::filesystem::exists(fmt::format("{}\\libcocos2d.dll", path)); 
		return check;
	}
}

void ResourcesLoadingLayer::loadLoadingLayer()
{
	this->runAction(Sequence::create(DelayTime::create(0), CallFunc::create([&]()
		{

			auto scene = ax::Scene::create();
			scene->addChild(LoadingLayer::create());
			Director::getInstance()->replaceScene(scene);

		}), nullptr));
}

bool ResourcesLoadingLayer::init()
{
	auto director = Director::getInstance();
	auto winSize = director->getWinSize();
	std::string writablePath = FileUtils::getInstance()->getWritablePath();

	/*Sprite* bgGradient = Sprite::createWithTexture(director->getTextureCache()->addImage("Custom/opengd_gradient.png"));
	bgGradient->setContentSize(winSize);
	bgGradient->setPosition(winSize / 2.f);
	bgGradient->setOpacity(102);
	addChild(bgGradient);

	Sprite* bgGradient2 = Sprite::createWithTexture(director->getTextureCache()->addImage("Custom/opengd_gradient.png"));
	bgGradient2->setContentSize(winSize);
	bgGradient2->setRotation(180);
	bgGradient2->setPosition(winSize / 2.f);
	bgGradient2->setOpacity(51);
	bgGradient->addChild(bgGradient2);

	bgGradient->setColor({ 175,40,240 });
	bgGradient2->setColor({ 175,40,240 });
	*/

	std::string gamePath{};
	if (std::filesystem::exists(fmt::format("{}\\gdPath.txt", writablePath)))
	{
		gamePath = FileUtils::getInstance()->getStringFromFile(fmt::format("{}\\gdPath.txt", writablePath));

		GameToolbox::log("Got Path From Save:{}", gamePath);

		if (checkPath(gamePath))
		{
			FileUtils::getInstance()->addSearchPath(fmt::format("{}\\Resources\\", gamePath), true);

			loadLoadingLayer();

			return true;
		}
	}

	gamePath = GameToolbox::getSteamGamePathByAppID(322170, "GeometryDash.exe");

	GameToolbox::log("Got Path From Steam:{}", gamePath);

	if (checkPath(gamePath))
	{
		FileUtils::getInstance()->addSearchPath(fmt::format("{}\\Resources\\", gamePath), true);

		FileUtils::getInstance()->writeStringToFile(gamePath, fmt::format("{}\\gdPath.txt", writablePath));

		loadLoadingLayer();

		return true;
	}

	// method 2 isnt complete. it should start GD using steam://run/322170, wait for it to run, get its path.

	/*if (checkPath(gamePath))
	{
		loadLoadingLayer();
		return true;
	}
	*/

	loadLoadingLayer();

	return true;
}