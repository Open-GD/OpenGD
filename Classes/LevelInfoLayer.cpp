#include "LevelInfoLayer.h"

#include <AudioEngine.h>
#include "CreatorLayer.h"
#include "InfoLayer.h"
#include "LoadingCircle.h"
#include "LevelPage.h"
#include "MenuItemSpriteExtra.h"
#include "PlayLayer.h"
#include "RateLevelLayer.h"
#include "AlertLayer.h"

USING_NS_AX;
using namespace ax::network;

LevelInfoLayer* LevelInfoLayer::create(std::vector<std::string> levelinfo)
{
	LevelInfoLayer* pRet = new LevelInfoLayer();

	if (pRet->init(levelinfo))
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

Scene* LevelInfoLayer::scene(std::vector<std::string> levelinfo)
{
	auto scene = Scene::create();
	scene->addChild(LevelInfoLayer::create(levelinfo));
	return scene;
}

bool LevelInfoLayer::init(std::vector<std::string> levelinfo)
{
	std::string _levelname = levelinfo[3];
	std::string _levelcreator = levelinfo[54];
	int _leveldownloads = std::stoi(levelinfo[13]);
	int _levellength = std::stoi(levelinfo[37]);
	int _levellikes = std::stoi(levelinfo[19]);
	int _levelID = std::stoi(levelinfo[1]);

	if (!Layer::init()) return false;
	auto winSize = Director::getInstance()->getWinSize();

	auto bg = Sprite::create("GJ_gradientBG-hd.png");
	bg->setScaleX(winSize.width / bg->getContentSize().width);
	bg->setScaleY(winSize.height / bg->getContentSize().height);
	bg->setAnchorPoint({ 0, 0 });
	bg->setColor({ 0, 102, 255 });
	this->addChild(bg);

	GameToolbox::createCorners(this, false, false, true, true);

	auto backBtnMenu = Menu::create();
	auto backBtn = MenuItemSpriteExtra::create("GJ_arrow_01_001.png", [](Node*) {
		Director::getInstance()->replaceScene(TransitionFade::create(.5, CreatorLayer::scene()));
	});
	backBtnMenu->addChild(backBtn);
	backBtnMenu->setPosition({ 24.0, winSize.height - 23.0f });
	this->addChild(backBtnMenu);

	auto levelName = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), _levelname);
	levelName->setPosition({ winSize.width / 2, winSize.height - 30.f });

	if (levelName->getContentSize().width > 300.0f)
	{
		levelName->setScale(300.0f / levelName->getContentSize().width);
	}

	this->addChild(levelName);

	auto levelCreator = Label::createWithBMFont(GameToolbox::getTextureString("goldFont.fnt"), StringUtils::format("By %s", _levelcreator));
	levelCreator->setPosition({ winSize.width / 2, levelName->getPositionY() - 30.f});

	if (levelCreator->getContentSize().width > 300.0f)
	{
		levelCreator->setScale(300.0f / levelCreator->getContentSize().width);
	}
	if (levelCreator->getScale() > 0.8)
	{
		levelCreator->setScale(0.8);
	}

	this->addChild(levelCreator);

	auto loadingCircle = LoadingCircle::create();
	loadingCircle->setPosition({ winSize.width / 2, (winSize.height / 2) + 35.0f });
	loadingCircle->getCircle()->setScale(2.f);

	this->addChild(loadingCircle);

	// Info (length, downloads, likes)
	ax::Vector<Node*> final = {};

	Node* timeNode = Node::create();
	auto timeSprite = Sprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
	timeNode->addChild(timeSprite);

	const char* length = [&](int i) {
		switch (i) {
		case 0:
			return "Tiny";
		case 1:
			return "Short";
		case 2:
			return "Medium";
		default:
			return "Long";
		}
	}(_levellength);

	auto timeLabel = MenuItemLabel::create(Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), length));
	timeLabel->setScale(0.5);
	timeNode->addChild(timeLabel);
	GameToolbox::alignItemsHorizontallyWithPadding(timeNode->getChildren(), 5);
	final.pushBack(timeNode);

	Node* downloadNode = Node::create();
	auto downloadSprite = Sprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
	downloadNode->addChild(downloadSprite);
	auto downloadLabel = MenuItemLabel::create(Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), StringUtils::format("%i", _leveldownloads)));
	downloadLabel->setScale(0.5);
	downloadNode->addChild(downloadLabel);
	GameToolbox::alignItemsHorizontallyWithPadding(downloadNode->getChildren(), 5);
	final.pushBack(downloadNode);

	Node* likeNode = Node::create();
	auto likeSprite = Sprite::createWithSpriteFrameName("GJ_likesIcon_001.png");
	likeNode->addChild(likeSprite);
	auto likeLabel = MenuItemLabel::create(Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), StringUtils::format("%i", _levellikes)));
	likeLabel->setScale(0.5);
	likeNode->addChild(likeLabel);
	GameToolbox::alignItemsHorizontallyWithPadding(likeNode->getChildren(), 5);
	final.pushBack(likeNode);

	GameToolbox::alignItemsHorizontallyWithPadding(final, 100);

	auto layer = Layer::create();

	for (Node* child : final)
	{
		layer->addChild(child);
	}

	layer->setPosition({winSize.width / 2, (winSize.height / 2) - 30.0f});
	addChild(layer);

	// Normal Progressbar
	auto normalBar = ax::Sprite::create(GameToolbox::getTextureString("GJ_progressBar_001.png"));
	normalBar->setPosition({ winSize.width / 2, 78.0 });
	normalBar->setColor({ 0, 0, 0 });
	normalBar->setOpacity(125);

	addChild(normalBar, 3);

	auto normalProgress = ax::Sprite::create(GameToolbox::getTextureString("GJ_progressBar_001.png"));
	normalProgress->setPosition({ 1.36f, 10 });
	normalProgress->setColor({ 0, 255, 0 });
	normalProgress->setOpacity(255);
	normalProgress->setAnchorPoint({ 0, 0.5 });
	normalProgress->setTextureRect({0, 0, normalBar->getContentSize().width * (0 / 100.f), normalBar->getTextureRect().size.height});
	normalProgress->setScale(0.992f);
	normalProgress->setScaleX(0.992f);
	normalProgress->setScaleY(0.86f);

	auto normalPercentage = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), StringUtils::format("%i%%", 0));
	normalPercentage->setScale(0.5);
	normalPercentage->setPosition(normalBar->getContentSize() / 2);

	normalBar->addChild(normalPercentage, 4);
	normalBar->addChild(normalProgress);
	this->addChild(normalBar);

	auto normalLabel = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Normal Mode");
	normalLabel->setPosition({ normalBar->getPositionX(), normalBar->getPositionY() + 20.0f });
	normalLabel->setScale(0.5);

	this->addChild(normalLabel);

	// Practice Progress Bar
	auto practiceBar = ax::Sprite::create(GameToolbox::getTextureString("GJ_progressBar_001.png"));
	practiceBar->setPosition({ winSize.width / 2, normalBar->getPositionY() - 50.f});
	practiceBar->setColor({ 0, 0, 0 });
	practiceBar->setOpacity(125);
	addChild(practiceBar, 3);

	auto practiceProgress = ax::Sprite::create(GameToolbox::getTextureString("GJ_progressBar_001.png"));
	practiceProgress->setPosition({ 1.36f, 10 });
	practiceProgress->setColor({ 0, 255, 0 });
	practiceProgress->setOpacity(255);
	practiceProgress->setAnchorPoint({ 0, 0.5 });
	practiceProgress->setTextureRect({ 0, 0, practiceBar->getContentSize().width * (0 / 100.f), practiceBar->getTextureRect().size.height });
	practiceProgress->setScale(0.992f);
	practiceProgress->setScaleX(0.992f);
	practiceProgress->setScaleY(0.86f);

	auto practicePercentage = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), StringUtils::format("%i%%", 0));
	practicePercentage->setScale(0.5);
	practicePercentage->setPosition(normalBar->getContentSize() / 2);

	practiceBar->addChild(practicePercentage, 4);
	practiceBar->addChild(practiceProgress);
	this->addChild(practiceBar);

	auto practiceLabel = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Practice Mode");
	practiceLabel->setPosition({ normalBar->getPositionX(), practiceBar->getPositionY() + 20.0f });
	practiceLabel->setScale(0.5);

	this->addChild(practiceLabel);

	// Side Menu
	auto menu = Menu::create();

	auto deleteBtn = MenuItemSpriteExtra::create(Sprite::createWithSpriteFrameName("GJ_deleteBtn_001.png"), [](Node*){
		auto alert = AlertLayer::create("Delete Level", "Are you sure you want to delete this level?", "NO", "YES", NULL, NULL);
		alert->setBtn2Callback([=](TextButton*) {
			alert->close();
			Director::getInstance()->replaceScene(TransitionFade::create(.5, CreatorLayer::scene()));
		});
		alert->show();
	});
	menu->addChild(deleteBtn);
	deleteBtn->setPosition(Node::convertToNodeSpace(menu->getPosition()));
	deleteBtn->setPositionX(deleteBtn->getPositionX() - 30.f);
	deleteBtn->setPositionY(deleteBtn->getPositionY() - 30.f);

	auto updateBtn = MenuItemSpriteExtra::create(Sprite::createWithSpriteFrameName("GJ_updateBtn_001.png"), [](Node*) {});
	menu->addChild(updateBtn);
	updateBtn->setPosition(deleteBtn->getPositionX(), deleteBtn->getPositionY() - 50.f);

	auto infoBtn = MenuItemSpriteExtra::create(Sprite::createWithSpriteFrameName("GJ_infoBtn_001.png"), [levelinfo](Node*) {
		InfoLayer::create(levelinfo)->show();
	});
	menu->addChild(infoBtn);
	infoBtn->setPosition(updateBtn->getPositionX(), updateBtn->getPositionY() - 50.f);

	auto rateDiffSprite = Sprite::createWithSpriteFrameName("GJ_rateDiffBtn_001.png");
	auto rateDiffBtn = MenuItemSpriteExtra::create(rateDiffSprite, [_levelID](Node*) {
		RateLevelLayer::create(_levelID)->show();
		});
	rateDiffBtn->setDisabledImage(Sprite::createWithSpriteFrameName("GJ_rateDiffBtn2_001.png"));

	menu->addChild(rateDiffBtn);
	rateDiffBtn->setPosition(infoBtn->getPositionX(), infoBtn->getPositionY() - 50.f);

	auto likeBtn = MenuItemSpriteExtra::create(Sprite::createWithSpriteFrameName("GJ_likeBtn_001.png"), [](Node*) {});
	likeBtn->setDisabledImage(Sprite::createWithSpriteFrameName("GJ_likeBtn2_001.png"));

	menu->addChild(likeBtn);
	likeBtn->setPosition(rateDiffBtn->getPositionX(), rateDiffBtn->getPositionY() - 50.f);

	this->addChild(menu);


	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [=](EventKeyboard::KeyCode key, Event*) {
		switch (key)
		{
		case EventKeyboard::KeyCode::KEY_SPACE:
			AudioEngine::stopAll();
			AudioEngine::play2d("playSound_01.ogg", false, 0.5f);
			//Director::getInstance()->replaceScene(ax::TransitionFade::create(0.5f, PlayLayer::scene(level)));
			break;
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			Director::getInstance()->replaceScene(TransitionFade::create(.5, CreatorLayer::scene()));
			break;
		}
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		std::string levelID(std::to_string(_levelID));
        std::string postData = fmt::format("levelID={}&secret=Wmfd2893gb7", levelID);
        GameToolbox::log("postData: {}", postData);
        HttpRequest* request = new HttpRequest();
        request->setTag(levelID);
        request->setUrl("http://www.boomlings.com/database/downloadGJLevel22.php");
        request->setRequestType(HttpRequest::Type::POST);
        request->setHeaders(std::vector<std::string>{"User-Agent: "});
        request->setRequestData(postData.c_str(), postData.length());
        request->setResponseCallback(AX_CALLBACK_2(LevelInfoLayer::onHttpRequestCompleted, this));
        request->setTag("GET test3");
        HttpClient::getInstance()->send(request);
        request->release();

	return true;
}

void LevelInfoLayer::onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response) {
    if (auto str = GameToolbox::getResponse(response)) {

        GJGameLevel* level = GJGameLevel::createWithResponse(*str);
        GameToolbox::log("playing level<{}>:{},{}*", level->_LevelID, level->_LevelName, level->_Stars);

		// Play button
		auto winSize = Director::getInstance()->getWinSize();
		auto playBtnMenu = Menu::create();
		playBtnMenu->setPosition({ winSize.width / 2, (winSize.height / 2) + 35.0f });
		this->addChild(playBtnMenu);

	    auto playBtn = MenuItemSpriteExtra::create(Sprite::createWithSpriteFrameName("GJ_playBtn2_001.png"), [level](Node*) {
		AudioEngine::stopAll();
		AudioEngine::play2d("playSound_01.ogg", false, 0.5f);
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, PlayLayer::scene(level)));
		});
		playBtnMenu->addChild(playBtn);
    } else {
        GameToolbox::log("request failed");
    }
}

