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

CreatorLayer* CreatorLayer::Instance = nullptr;

CreatorLayer* CreatorLayer::getInstance()
{
    return Instance;
}

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
        director->replaceScene(TransitionFade::create(0.5f, LevelSearchLayer::scene()));
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

    glfwSetKeyCallback(static_cast<GLViewImpl*>(director->getOpenGLView())->getWindow(), keyCallback);

    director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#endif

    return true;
}

void CreatorLayer::onKeyPressed(ax::EventKeyboard::KeyCode keyCode, ax::Event* event) {
    switch (keyCode) 
    {
    case EventKeyboard::KeyCode::KEY_BACK:
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MenuLayer::scene()));
        break;
    }
}
#ifdef AX_PLATFORM_PC
void CreatorLayer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_C:
            if (mods == GLFW_KEY_LEFT_CONTROL)
                glfwSetClipboardString(window, getInstance()->_levelField->getString().data());
                break;
        case GLFW_KEY_V:
            if (mods == GLFW_KEY_LEFT_CONTROL)
            {
                std::string str(getInstance()->_levelField->getString());
                getInstance()->_levelField->setString(str);
            }
            break;
        default:
            break;
        }
    }
}
#endif
