#include "CreatorLayer.h"
#include <AudioEngine.h>
#include <LevelPage.h>
#include "GJGameLevel.h"
#include "GameToolbox.h"
#include "LevelEditorLayer.h"
#include "LevelInfoLayer.h"
#include "LevelSearchLayer.h"
#include "MenuItemSpriteExtra.h"
#include "MenuLayer.h"
#include "PlayLayer.h"
#include "base64.h"

USING_NS_AX;
using namespace ax::network;

Scene* CreatorLayer::scene() {
    return CreatorLayer::create();
}

CreatorLayer* CreatorLayer::create() {
    CreatorLayer* ret = new CreatorLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    } else {
        delete ret;
        ret = nullptr;
        return nullptr;
    }
}

bool CreatorLayer::init() {
    if (!Scene::init())
        return false;

    auto director = Director::getInstance();
    auto winSize  = director->getWinSize();

    auto bg = Sprite::create("GJ_gradientBG.png");
    bg->setStretchEnabled(false);
    bg->setScaleX(winSize.width / bg->getContentSize().width);
    bg->setScaleY(winSize.height / bg->getContentSize().height);
    bg->setAnchorPoint({0, 0});
    bg->setColor({0, 102, 255});
    addChild(bg);

    GameToolbox::createAllCorners(this);

    auto createBtnSpr   = Sprite::createWithSpriteFrameName("GJ_createBtn_001.png");
    auto savedBtnSpr    = Sprite::createWithSpriteFrameName("GJ_savedBtn_001.png");
    auto featuredBtnSpr = Sprite::createWithSpriteFrameName("GJ_featuredBtn_001.png");
    auto searchBtnSpr   = Sprite::createWithSpriteFrameName("GJ_searchBtn_001.png");

    auto savedBtn    = MenuItemSpriteExtra::create(savedBtnSpr, [=](Node*) { GameToolbox::log("on saved"); });
    auto featuredBtn = MenuItemSpriteExtra::create(featuredBtnSpr, [=](Node*) { GameToolbox::log("on featured"); });
    auto searchBtn   = MenuItemSpriteExtra::create(searchBtnSpr, [=](Node*) {
        //	director->replaceScene(TransitionFade::create(0.5f, LevelSearchLayer::scene()));

        std::string levelID(_levelField->getString());
        std::string postData = fmt::format("type=0&str={}&secret=Wmfd2893gb7", levelID);
        GameToolbox::log("postData: {}", postData);
        HttpRequest* request = new HttpRequest();
        request->setTag(levelID);
        request->setUrl("http://www.boomlings.com/database/getGJLevels21.php");
        request->setRequestType(HttpRequest::Type::POST);
        request->setHeaders(std::vector<std::string>{"User-Agent: "});
        request->setRequestData(postData.c_str(), postData.length());
        request->setResponseCallback(AX_CALLBACK_2(CreatorLayer::onHttpSearchRequestCompleted, this));
        request->setTag("GET test3");
        HttpClient::getInstance()->send(request);
        request->release();
    });
    auto createBtn   = MenuItemSpriteExtra::create(createBtnSpr, [=](Node*) {
        auto scene = LevelEditorLayer::scene(GJGameLevel::createWithMinimumData("cool level", "partur", 5));
        director->pushScene(TransitionFade::create(0.5f, scene));
    });

    auto backBtn = MenuItemSpriteExtra::create("GJ_arrow_03_001.png", [=](Node*) {
        director->replaceScene(TransitionFade::create(0.5f, MenuLayer::scene()));
    });

    auto menu = Menu::create(createBtn, savedBtn, featuredBtn, searchBtn, nullptr);
    GameToolbox::alignItemsInColumnsWithPadding(menu, 2, 130, 130);
    menu->addChild(backBtn);
    backBtn->setPosition(menu->convertToNodeSpace({25, winSize.height - 25}));

    addChild(menu);

    _levelField = ui::TextField::create("Enter ID", GameToolbox::getTextureString("bigFont.fnt"), 20);
    _levelField->setPlaceHolderColor({120, 170, 240});
    _levelField->setMaxLength(10);
    _levelField->setMaxLengthEnabled(true);
    _levelField->setCursorEnabled(true);
    _levelField->setString("128");
    _levelField->setPosition({winSize.width / 2 + 150, winSize.height / 2});
    addChild(_levelField);

#ifdef AX_PLATFORM_PC
    auto listener = EventListenerKeyboard::create();

    listener->onKeyPressed  = AX_CALLBACK_2(CreatorLayer::onKeyPressed, this);
    listener->onKeyReleased = AX_CALLBACK_2(CreatorLayer::onKeyReleased, this);
    director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#endif

    return true;
}

void CreatorLayer::onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event) {
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_CTRL:
        _ctrlDown = true;
        break;

    case EventKeyboard::KeyCode::KEY_V: {
        if (_ctrlDown) {
            GameToolbox::log("paste");
            _levelField->setString(GameToolbox::getClipboardString());
        }
        break;
    }

    case EventKeyboard::KeyCode::KEY_BACK: {
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MenuLayer::scene()));
    }
    }
}

void CreatorLayer::onKeyReleased(ax::EventKeyboard::KeyCode keyCode, ax::Event* event) {
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_CTRL:
        _ctrlDown = false;
        break;
    }
}


void CreatorLayer::onHttpSearchRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response) {
    if (auto str = GameToolbox::getResponse(response)) {
        // validate by checking if level ID key/value is set
        if ((*str).find(fmt::format("1:{}", _levelField->getString())) == std::string::npos)
            return GameToolbox::log("invalid response: {}", *str);

        GJGameLevel* level = GJGameLevel::createWithResponse(*str);
        Director::getInstance()->replaceScene(ax::TransitionFade::create(0.5f, LevelInfoLayer::scene(level)));
    } else {
        GameToolbox::log("request failed");
    }
}

