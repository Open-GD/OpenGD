#include "LevelSelectLayer.h"
#include "GJGameLevel.h"
#include "MenuItemSpriteExtra.h"
#include "AudioEngine.h"
#include "PlayLayer.h"
#include "GameToolbox.h"
#include "MenuLayer.h"
#include "BoomScrollLayer.h"
#include "LevelPage.h"

USING_NS_AX;


Scene* LevelSelectLayer::scene()
{
	auto scene = Scene::create();
	scene->addChild(LevelSelectLayer::create());
	return scene;
}

bool LevelSelectLayer::init()
{

	if(!Layer::init()) return false;

	auto director = Director::getInstance();
	auto winSize = director->getWinSize();

	_background = Sprite::create("GJ_gradientBG.png");
	_background->setAnchorPoint({0.0f, 0.0f});
	addChild(_background, -2);

	_background->setScaleX((winSize.width + 10.0f) / _background->getTextureRect().size.width);
	_background->setScaleY((winSize.height + 10.0f) / _background->getTextureRect().size.height);
	_background->setPosition({-5.0f, -5.0f});
	_background->setColor({0x28, 0x7D, 0xFF});

	// why does game manager store the ground id? who knows!
	_ground = GroundLayer::create(1);
	_ground->setPositionY(-25.f);
	addChild(_ground, -1);

	// auto topBar = Sprite::create("GJ_topBar_001.png");
	// topBar->setAnchorPoint({0.5f, 1.0f});
	// topBar->setPosition({winSize.width / 2, winSize.height + 1.0f});

	// addChild(topBar, 1);

	GameToolbox::createCorners(this, false, false, true, true);
	

	//for(uint32_t i = 0; i < 20; i++)
	//{
	//	layers.push_back(LevelPage::create(GJGameLevel::createWithMinimumData(fmt::format("Stereo Madness {}", i), "RobTop", 1)));
	//}
	constexpr auto levelData = std::to_array<std::tuple<const char*, const char*, int>>({
		{ "Stereo Madness", "Robtop", 1 },
		{ "Back on Track", "Robtop", 2 },
		{ "Polargeist", "Robtop", 3 },
		{ "Dry Out", "Robtop", 4 },
		{ "Base After Base", "Robtop", 5 },
		{ "Cant Let Go", "Robtop", 6 },
		{ "Jumper", "Robtop", 7 },
		{ "Performance Test", "OpenGD", -1 }
	});
	
	//TODO: add getters on bsl and level page because they are actually owning the stuff
	
	std::vector<Layer*> layers;
	layers.reserve(levelData.size());
	
	for (const auto [name, creator, id] : levelData)
	{
		auto level = GJGameLevel::createWithMinimumData(name, creator, id);
		layers.push_back(LevelPage::create(level));
	}
	
	auto bsl = BoomScrollLayer::create(layers, 0);
	addChild(bsl);

	/* std::vector<GJGameLevel> mainLevels;
	cocos2d::CCArray* levelPages = cocos2d::CCArray::create();
	for (size_t i = 0; i < 22; i++)
		mainLevels->addObject(GLM->getMainLevel(i, true));

	for (size_t i = 0; i < 3; i++)
		levelPages->addObject(LevelPage::create(nullptr)); // the class takes a level object?

	GJGameLevel* defaultLevel = GJGameLevel::create();
	defaultLevel->setLevelID(-1);
	mainLevels->addObject(defaultLevel);

	m_pBoomScrollLayer =
		BoomScrollLayer::create(mainLevels, 0, true, levelPages, static_cast<DynamicScrollDelegate*>(this));
	addChild(m_pBoomScrollLayer);

	m_pBoomScrollLayer->setPagesIndicatorPosition({winSize / 2, director->getScreenBottom() + 15.0f});

	m_fWindowWidth = winSize.width;

	m_pBoomScrollLayer->m_pExtendedLayer->m_pDelegate = static_cast<BoomScrollLayerDelegate*>(this);

	if (page)
	{
		if (page == 21) m_pBoomScrollLayer->instantMoveToPage(20);
		m_pBoomScrollLayer->instantMoveToPage(page);
	}
	else
	{
		scrollLayerMoved(&Globals::gScollLayerPos); //
	} */

	// auto download = Label::createWithBMFont("Download the soundtracks", "bigFont.fnt");
	// download->setScale(0.5f);

	// MenuItemSpriteExtra* downloadBtn = MenuItemSpriteExtra::create(download, [&](Node* btn) {
	// //auto a = GJMoreGamesLayer::create();
	// //addChild(a);
	// });
	// downloadBtn->setScale(2.0f);

	// Menu* DLMenu = Menu::create();
	// addChild(DLMenu);

	// DLMenu->setPosition({winSize.width / 2, 0 + 35.0f});

	auto btnMenu = Menu::create();
	addChild(btnMenu, 5);

	//bool controller = PlatformToolbox::isControllerConnected();
	bool controller = false;

	auto left =
		Sprite::createWithSpriteFrameName(controller ? "controllerBtn_DPad_Left_001.png" : "navArrowBtn_001.png");
	if (!controller) left->setFlippedX(true);

	MenuItemSpriteExtra* leftBtn = MenuItemSpriteExtra::create(left, [&, bsl](Node* btn) {
		bsl->changePageLeft();
	});
	btnMenu->addChild(leftBtn);

	//leftBtn->setScale(2.0f);
	leftBtn->setPosition(btnMenu->convertToNodeSpace({ 25.0f, winSize.height / 2 }));

	auto right = Sprite::createWithSpriteFrameName(controller ? "controllerBtn_DPad_Right_001.png" : "navArrowBtn_001.png");

	MenuItemSpriteExtra* rightBtn = MenuItemSpriteExtra::create(right, [&, bsl](Node* btn) {
		bsl->changePageRight();
	});
	btnMenu->addChild(rightBtn);

	//right->setScale(2.0f);
	rightBtn->setPosition(btnMenu->convertToNodeSpace({ winSize.width - 25.0f, winSize.height / 2 }));

	auto back = Sprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
	MenuItemSpriteExtra* backBtn = MenuItemSpriteExtra::create(back, [&](Node* btn) {
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MenuLayer::scene()));
	});
	//backBtn->setScale(1.6f);

	Menu* backMenu = Menu::create();
	addChild(backMenu, 1);

	backMenu->addChild(backBtn);

	backMenu->setPosition({ 0 + 25.0f, winSize.height - 22.0f });

	// //GM->0x298 = 0;

	auto infoMenu = Menu::create();
	addChild(infoMenu);

	Sprite* info = Sprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
	MenuItemSpriteExtra* infoBtn = MenuItemSpriteExtra::create(info, [&](Node* btn) {
		//auto a = GJMoreGamesLayer::create();
		//addChild(a);
	});
	infoMenu->addChild(infoBtn, 1);

	infoMenu->setPosition({ winSize.width - 20.0f, winSize.height - 20.0f });

	auto listener = EventListenerKeyboard::create();

	// int currentlevel = 0;

	listener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event*) {
		if (code == EventKeyboard::KeyCode::KEY_ESCAPE) {
			auto scene = MenuLayer::scene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
		} else if (code == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
			bsl->changePageLeft();
		} else if (code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
			bsl->changePageRight();
		} else if (code == EventKeyboard::KeyCode::KEY_SPACE) {
			// ax::AudioEngine::stopAll();
			// ax::AudioEngine::play2d("playSound_01.ogg", false, 0.2f);
			// auto currentLevelPage = dynamic_cast<LevelPage*>(bsl->_layers.at(bsl->_currentPage));
			// if (currentLevelPage) {
			// 	auto level = currentLevelPage->_level;
			// 	ax::Director::getInstance()->replaceScene(ax::TransitionFade::create(0.5f, PlayLayer::scene(level)));
			// 	LevelPage::replacingScene = true;
			// 	MenuLayer::music = false;
			// }
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//if (controller) GameToolbox::addBackButton(this, backBtn);
	return true;
}