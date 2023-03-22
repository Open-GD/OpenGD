#include "LevelBrowserLayer.h"
#include "GJGameLevel.h"
#include "LevelCell.h"
#include "LevelSearchLayer.h"

ax::Scene* LevelBrowserLayer::scene(GJSearchObject* search) {
    auto scene = ax::Scene::create();
    scene->addChild(LevelBrowserLayer::create(search));
    return scene;
}

LevelBrowserLayer* LevelBrowserLayer::create(GJSearchObject* search) {
    LevelBrowserLayer* ret = new LevelBrowserLayer();
    if (ret->init(search)) {
        ret->autorelease();
        return ret;
    } else {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}

bool LevelBrowserLayer::init(GJSearchObject* search) {
    if (!Layer::init())
        return false;

    auto director = ax::Director::getInstance();
    auto winSize  = director->getWinSize();

    GameToolbox::createCorners(this, false, false, true, true);

	auto backBtnMenu = ax::Menu::create();
	auto backBtn = MenuItemSpriteExtra::create("GJ_arrow_01_001.png", [](Node*) {
		ax::Director::getInstance()->replaceScene(ax::TransitionFade::create(.5, LevelSearchLayer::scene()));
	});
	backBtnMenu->addChild(backBtn);
	backBtnMenu->setPosition({ 24.0, winSize.height - 23.0f });
	this->addChild(backBtnMenu, 5);

    auto bg = ax::Sprite::create("GJ_gradientBG.png");
    bg->setStretchEnabled(false);
    bg->setScaleX(winSize.width / bg->getContentSize().width);
    bg->setScaleY(winSize.height / bg->getContentSize().height);
    bg->setAnchorPoint({0, 0});
    bg->setColor({0, 102, 255});
    addChild(bg);

    listView = ax::ui::ListView::create();
    listView->setAnchorPoint({0.5, 0.5});
    listView->setBackGroundColorType(ax::ui::Layout::BackGroundColorType::SOLID);
    listView->setBackGroundColor(ax::Color3B::ORANGE);

    auto list = ListLayer::create(listView, "Online Levels");
    listView->setPosition({180, 120});
    list->setPosition({106.5, 45});
    listView->setTopPadding(25);

    addChild(list);

    std::string levelStr = search->_searchQuery;
    std::string postData = fmt::format("str={}&secret=Wmfd2893gb7&type={}", search->_searchQuery, (int)search->_screenID);
    GameToolbox::log("postData: {}", postData);
    ax::network::HttpRequest* request = new ax::network::HttpRequest();
    request->setTag(search->_searchQuery);
    request->setUrl("http://www.boomlings.com/database/getGJLevels21.php");
    request->setRequestType(ax::network::HttpRequest::Type::POST);
    request->setHeaders(std::vector<std::string>{"User-Agent: "});
    request->setRequestData(postData.c_str(), postData.length());
    request->setResponseCallback(AX_CALLBACK_2(LevelBrowserLayer::onHttpRequestCompleted, this));
    request->setTag("GET test3");
    ax::network::HttpClient::getInstance()->send(request);
    request->release();
}

void LevelBrowserLayer::onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response) {
    if (auto str = GameToolbox::getResponse(response)) {
        GameToolbox::log("{}", (*str));
        auto splits = GameToolbox::splitByDelim((*str), '#');
        auto levels = GameToolbox::splitByDelim(splits[0], '|');

        for(std::string level : levels)
        {
            auto levelCell = LevelCell::create(GJGameLevel::createWithResponse(level));
            listView->pushBackCustomItem(levelCell);
        }
        //listView->setContentSize({356, (90.f * i)});

    } else {
        GameToolbox::log("request failed");
    }
}