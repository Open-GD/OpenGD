#include "LevelInfoLayer.h"

#include <AudioEngine.h>
#include "InfoLayer.h"
#include "LevelPage.h"
#include "PlayLayer.h"
#include "RateLevelLayer.h"
#include "AlertLayer.h"
#include "GarageLayer.h"

USING_NS_AX;

LevelInfoLayer* LevelInfoLayer::create(GJGameLevel* level)
{
	LevelInfoLayer* pRet = new LevelInfoLayer();

	if (pRet->init(level))
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

Scene* LevelInfoLayer::scene(GJGameLevel* level)
{
	auto scene = Scene::create();
	scene->addChild(LevelInfoLayer::create(level));
	return scene;
}

bool LevelInfoLayer::init(GJGameLevel* level)
{
	if (!Layer::init()) return false;
	auto winSize = Director::getInstance()->getWinSize();

	_level = level;

	GameToolbox::createBG(this);
	GameToolbox::createCorners(this, false, false, true, true);

	auto backBtnMenu = Menu::create();
	auto backBtn = MenuItemSpriteExtra::create("GJ_arrow_01_001.png", [](Node*) { GameToolbox::popSceneWithTransition(0.5f); });
	backBtnMenu->addChild(backBtn);
	backBtnMenu->setPosition({24.0, winSize.height - 23.0f});
	this->addChild(backBtnMenu);

	auto levelName = GameToolbox::createBMFont(level->_LevelName, "bigFont.fnt");
	levelName->setPosition({winSize.width / 2, winSize.height - 17.0f});
	GameToolbox::limitLabelWidth(levelName, 300, 0.8f);
	this->addChild(levelName);

	auto levelCreator = GameToolbox::createBMFont(fmt::format("By {}", level->_LevelCreator), "goldFont.fnt");
	 if (level->_LevelCreator == "-") levelCreator->setColor(ax::Color3B(90, 255, 255)); // thanks gd colon
	levelCreator->setPosition({winSize.width / 2, levelName->getPositionY() - 30.f});
	GameToolbox::limitLabelWidth(levelCreator, 300, 0.8f);
	this->addChild(levelCreator);

	auto rate = Sprite::createWithSpriteFrameName("GJ_featuredCoin_001.png");
	rate->setPosition({ winSize.width / 2 - 100, winSize.height / 2 + 70 });
	rate->setVisible(false);
	this->addChild(rate);

	if (level->_FeatureScore > 0) rate->setVisible(true);
	if (level->_Epic) {
		rate->createWithSpriteFrameName("GJ_epicCoin_001.png");
		rate->initWithSpriteFrameName("GJ_epicCoin_001.png");
		rate->setVisible(true);
	}

	auto diff = Sprite::createWithSpriteFrameName(GJGameLevel::getDifficultySprite(level, kLevelInfoLayer));
	diff->setPosition(rate->getPosition());
	this->addChild(diff);

	if(level->_Stars > 0) {
		auto star = Sprite::createWithSpriteFrameName("star_small01_001.png");
		star->setPosition({ diff->getPositionX() + 8, diff->getPositionY() - 38 });
		this->addChild(star);

		auto starCount = GameToolbox::createBMFont(std::to_string(level->_Stars), "bigFont.fnt");
		if (level->_normalPercent >= 100) starCount->setColor({ 255, 255, 0 });
		starCount->setScale(.38);
		starCount->setPosition({ diff->getPositionX() - 6, star->getPositionY() });
		this->addChild(starCount);
	}
	auto downIcon = Sprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
	downIcon->setPosition({ winSize.width / 2 + 90.5f, winSize.height / 2 + 90 });
	this->addChild(downIcon);

	auto downCount = GameToolbox::createBMFont(std::to_string(level->_Downloads), "bigFont.fnt");
	GameToolbox::limitLabelWidth(downCount, 60, 0.5f);
	downCount->setAnchorPoint({ 0, 0.5f });
	downCount->setPosition({ downIcon->getPositionX() + 20, downIcon->getPositionY()});
	this->addChild(downCount);

	auto likeIcon = Sprite::createWithSpriteFrameName(level->_Dislikes > 0 ? "GJ_dislikesIcon_001.png" : "GJ_likesIcon_001.png");
	likeIcon->setPosition({ downIcon->getPositionX(), downIcon->getPositionY() - 28});
	this->addChild(likeIcon);

	auto likeCount = GameToolbox::createBMFont(level->_Dislikes > 0 ? std::to_string(level->_Dislikes) : std::to_string(level->_Likes), "bigFont.fnt");
	GameToolbox::limitLabelWidth(likeCount, 60, 0.5f);
	likeCount->setAnchorPoint(downCount->getAnchorPoint());
	likeCount->setPosition({ downCount->getPositionX(), likeIcon->getPositionY()});
	this->addChild(likeCount);

	auto timeIcon = Sprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
	timeIcon->setPosition({ downIcon->getPositionX(), likeIcon->getPositionY() - 28});
	this->addChild(timeIcon);

	auto lenght = GameToolbox::createBMFont(GameToolbox::lengthString(level->_Length), "bigFont.fnt");
	GameToolbox::limitLabelWidth(lenght, 60, 0.5f);
	lenght->setAnchorPoint(downCount->getAnchorPoint());
	lenght->setPosition({ downCount->getPositionX(), timeIcon->getPositionY()});
	this->addChild(lenght);

	// Play button
	auto menu = Menu::create();
	menu->setPosition(0, 0);

	playBtn = MenuItemSpriteExtra::create(Sprite::createWithSpriteFrameName("GJ_playBtn2_001.png"), [&](Node*) {
		AudioEngine::stopAll();
		AudioEngine::play2d("playSound_01.ogg", false, 0.5f);
		Director::getInstance()->replaceScene(ax::TransitionFade::create(0.5f, PlayLayer::scene(_level)));
	});
	playBtn->setPosition({ winSize.width / 2, winSize.height / 2 + 51.0f });
	playBtn->setEnabled(false);
	playBtn->setVisible(false);
	menu->addChild(playBtn);

	auto shop = MenuItemSpriteExtra::create("garageRope_001.png", [](Node*) { Director::getInstance()->pushScene(TransitionMoveInT::create(0.5f, GarageLayer::scene(true))); });
	shop->setPosition({ winSize.width - 85, winSize.height - 25 });
	shop->setDestination({ 0, -8 });
	menu->addChild(shop);

	loading = LoadingCircle::create();
	loading->setPosition(playBtn->getPosition());
	loading->setVisible(true);

	this->addChild(loading);
	this->addChild(menu);

	// Normal Progressbar
	auto normalBar = ax::Sprite::create(GameToolbox::getTextureString("GJ_progressBar_001.png"));
	normalBar->setPosition({winSize.width / 2, 78.0});
	normalBar->setColor({0, 0, 0});
	normalBar->setOpacity(125);

	addChild(normalBar, 3);

	auto normalProgress = ax::Sprite::create(GameToolbox::getTextureString("GJ_progressBar_001.png"));
	normalProgress->setPosition({1.36f, 10});
	normalProgress->setColor({0, 255, 0});
	normalProgress->setOpacity(255);
	normalProgress->setAnchorPoint({0, 0.5});
	normalProgress->setTextureRect(
		{0, 0, normalBar->getContentSize().width * (level->_normalPercent / 100.f),
		 normalBar->getTextureRect().size.height});
	normalProgress->setScale(0.992f);
	normalProgress->setScaleX(0.992f);
	normalProgress->setScaleY(0.86f);

	auto normalPercentage = Label::createWithBMFont(
		GameToolbox::getTextureString("bigFont.fnt"), StringUtils::format("%i%%", level->_normalPercent));
	normalPercentage->setScale(0.5);
	normalPercentage->setPosition(normalBar->getContentSize() / 2);

	normalBar->addChild(normalPercentage, 4);
	normalBar->addChild(normalProgress);
	this->addChild(normalBar);

	auto normalLabel = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Normal Mode");
	normalLabel->setPosition({normalBar->getPositionX(), normalBar->getPositionY() + 20.0f});
	normalLabel->setScale(0.5);
	this->addChild(normalLabel);

	// Practice Progress Bar
	auto practiceBar = ax::Sprite::create(GameToolbox::getTextureString("GJ_progressBar_001.png"));
	practiceBar->setPosition({winSize.width / 2, normalBar->getPositionY() - 50.f});
	practiceBar->setColor({0, 0, 0});
	practiceBar->setOpacity(125);
	addChild(practiceBar, 3);

	auto practiceProgress = ax::Sprite::create(GameToolbox::getTextureString("GJ_progressBar_001.png"));
	practiceProgress->setPosition({1.36f, 10});
	practiceProgress->setColor({0, 255, 0});
	practiceProgress->setOpacity(255);
	practiceProgress->setAnchorPoint({0, 0.5});
	practiceProgress->setTextureRect(
		{0, 0, practiceBar->getContentSize().width * (level->_practicePercent / 100.f),
		 practiceBar->getTextureRect().size.height});
	practiceProgress->setScale(0.992f);
	practiceProgress->setScaleX(0.992f);
	practiceProgress->setScaleY(0.86f);

	auto practicePercentage = Label::createWithBMFont(
		GameToolbox::getTextureString("bigFont.fnt"), StringUtils::format("%i%%", level->_normalPercent));
	practicePercentage->setScale(0.5);
	practicePercentage->setPosition(normalBar->getContentSize() / 2);

	practiceBar->addChild(practicePercentage, 4);
	practiceBar->addChild(practiceProgress);
	this->addChild(practiceBar);

	auto practiceLabel = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Practice Mode");
	practiceLabel->setPosition({normalBar->getPositionX(), practiceBar->getPositionY() + 20.0f});
	practiceLabel->setScale(0.5);

	this->addChild(practiceLabel);

	// Side Menu
	auto buttonsMenu = Menu::create();

	auto deleteBtn = MenuItemSpriteExtra::create(Sprite::createWithSpriteFrameName("GJ_deleteBtn_001.png"), [](Node*) {
		auto alert =
			AlertLayer::create("Delete Level", "Are you sure you want to delete this level?", "NO", "YES", NULL, NULL);
			alert->setBtn2Callback([=](TextButton*) {
				alert->close();
				Director::getInstance()->popScene();
			});
		alert->show();
	});
	buttonsMenu->addChild(deleteBtn);
	deleteBtn->setPosition(Node::convertToNodeSpace(buttonsMenu->getPosition()));
	deleteBtn->setPositionX(deleteBtn->getPositionX() - 30.f);
	deleteBtn->setPositionY(deleteBtn->getPositionY() - 30.f);

	auto updateBtn = MenuItemSpriteExtra::create("GJ_updateBtn_001.png", [](Node*) {});
	updateBtn->setPosition(deleteBtn->getPositionX(), deleteBtn->getPositionY() - 50.f);
	buttonsMenu->addChild(updateBtn);

	auto infoBtn = MenuItemSpriteExtra::create("GJ_infoBtn_001.png", [level](Node*) { InfoLayer::create(level)->show(); });
	infoBtn->setPosition(updateBtn->getPositionX(), updateBtn->getPositionY() - 50.f);
	buttonsMenu->addChild(infoBtn);

	auto rateDiffSprite = Sprite::createWithSpriteFrameName("GJ_rateDiffBtn_001.png");
	auto rateDiffBtn = MenuItemSpriteExtra::create(rateDiffSprite, [level](Node*) { RateLevelLayer::create(level->_LevelID)->show(); });
	rateDiffBtn->setDisabledImage(Sprite::createWithSpriteFrameName("GJ_rateDiffBtn2_001.png"));
	rateDiffBtn->setPosition(infoBtn->getPositionX(), infoBtn->getPositionY() - 50.f);
	buttonsMenu->addChild(rateDiffBtn);

	auto likeBtn = MenuItemSpriteExtra::create(Sprite::createWithSpriteFrameName("GJ_likeBtn_001.png"), [](Node*) {});
	likeBtn->setDisabledImage(Sprite::createWithSpriteFrameName("GJ_likeBtn2_001.png"));
	likeBtn->setPosition(rateDiffBtn->getPositionX(), rateDiffBtn->getPositionY() - 50.f);
	buttonsMenu->addChild(likeBtn);

	this->addChild(buttonsMenu);

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [=](EventKeyboard::KeyCode key, Event*) {
		switch (key) {
		case EventKeyboard::KeyCode::KEY_SPACE:
			if (level->_LevelString.empty()) break;
			AudioEngine::stopAll();
			AudioEngine::play2d("playSound_01.ogg", false, 0.5f);
			Director::getInstance()->replaceScene(ax::TransitionFade::create(0.5f, PlayLayer::scene(level)));
			break;
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			GameToolbox::popSceneWithTransition(0.5f);
			break;
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	if(!level->_LevelString.empty()) 
	{
		loading->setVisible(false);
		playBtn->setEnabled(true);
		playBtn->setVisible(true);
		return true;
	}

	std::string postData = fmt::format("levelID={}&secret=Wmfd2893gb7", level->_LevelID);
	GameToolbox::executeHttpRequest(
		"http://www.boomlings.com/database/downloadGJLevel22.php", postData, ax::network::HttpRequest::Type::POST,
		AX_CALLBACK_2(LevelInfoLayer::onHttpRequestCompleted, this));

	return true;
}

void LevelInfoLayer::onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response)
{
	loading->setVisible(false);
	if (auto str = GameToolbox::getResponse(response))
	{
		playBtn->setEnabled(true);
		playBtn->setVisible(true);
		_level = GJGameLevel::createWithResponse((*str));
		_level->_MusicID = _level->_OfficialSongID;
	}
	else
	{
		auto alert = AlertLayer::create("Error", "Level download failed, please try\nagain later.", "OK", "", NULL, NULL);
		addChild(alert);
		alert->show();
		GameToolbox::log("request failed");
	}
}
