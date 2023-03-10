#include "CreatorLayer.h"
#include "GameToolbox.h"
#include "MenuLayer.h"
#include "MenuItemSpriteExtra.h"
#include "ImGui/ImGuiPresenter.h"
#include "ImGui/imgui/imgui.h"

USING_NS_AX;
USING_NS_AX_EXT;

using GameToolbox::getTextureString;

Scene* CreatorLayer::scene() {
    
    return CreatorLayer::create();
}

bool CreatorLayer::init() {
    if (!Scene::init()) return false;
    
  //  ImGuiPresenter::getInstance()->setViewResolution(1280, 720);
   // ImGuiPresenter::getInstance()->addRenderLoop("#test", AX_CALLBACK_0(CreatorLayer::onDrawImGui, this), this);

    auto winSize = Director::getInstance()->getWinSize();

    auto bg = Sprite::create(getTextureString("GJ_gradientBG.png"));
    bg->setStretchEnabled(false);
    bg->setScaleX(winSize.width / bg->getContentSize().width);
    bg->setScaleY(winSize.height / bg->getContentSize().height);
    bg->setAnchorPoint({0, 0});
    bg->setColor({0, 102, 255});
    this->addChild(bg);
    
    auto corn1 = Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    corn1->setStretchEnabled(false);
    this->addChild(corn1);
    corn1->setPosition({0, 0});
    corn1->setAnchorPoint({0.0, 0.0});

    auto corn2 = Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    corn2->setStretchEnabled(false);
    this->addChild(corn2);
    corn2->setPosition({0, winSize.height});
    corn2->setAnchorPoint({0, 1});
    corn2->setFlippedY(true);

    auto corn3 = Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    corn3->setStretchEnabled(false);
    this->addChild(corn3);
    corn3->setPosition(winSize);
    corn3->setAnchorPoint({1, 1});
    corn3->setFlippedX(true);
    corn3->setFlippedY(true);

    auto corn4 = Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
    corn4->setStretchEnabled(false);
    this->addChild(corn4);
    corn4->setPosition({winSize.width, 0});
    corn4->setAnchorPoint({1, 0});
    corn4->setFlippedX(true);

    auto createBtnSpr = Sprite::createWithSpriteFrameName("GJ_createBtn_001.png");
    createBtnSpr->setStretchEnabled(false);
    auto savedBtnSpr = Sprite::createWithSpriteFrameName("GJ_savedBtn_001.png");
    savedBtnSpr->setStretchEnabled(false);
    auto featuredBtnSpr = Sprite::createWithSpriteFrameName("GJ_featuredBtn_001.png");
    featuredBtnSpr->setStretchEnabled(false);
    auto searchBtnSpr = Sprite::createWithSpriteFrameName("GJ_searchBtn_001.png");
    searchBtnSpr->setStretchEnabled(false);

    auto savedBtn = MenuItemSpriteExtra::createWithNode(savedBtnSpr, [&](Node*) {
        GameToolbox::log("on saved");
    });
    auto featuredBtn = MenuItemSpriteExtra::createWithNode(featuredBtnSpr, [&](Node*) {
        GameToolbox::log("on featured");
    });
    auto searchBtn = MenuItemSpriteExtra::createWithNode(searchBtnSpr, [&](Node*) {
        GameToolbox::log("on search");
    });
    auto createBtn = MenuItemSpriteExtra::createWithNode(createBtnSpr, [&](Node*) {
        GameToolbox::log("on create");
    });

    createBtn->setScale(1.1f);
    savedBtn->setScale(1.1f);
    featuredBtn->setScale(1.1f);
    searchBtn->setScale(1.1f);

    auto backBtn = MenuItemSpriteExtra::create("GJ_arrow_03_001.png", [&](Node*) {
        Director::getInstance()->replaceScene(TransitionFade::create(.5, MenuLayer::scene()));
    });

    auto menu = Menu::create(createBtn, savedBtn, featuredBtn, searchBtn, nullptr);
    GameToolbox::alignItemsInColumnsWithPadding(menu, 2, 290, 290);
    menu->addChild(backBtn);
    backBtn->setPosition(menu->convertToNodeSpace({48, winSize.height - 46}));
    
    this->addChild(menu);



    return true;
    
}

static bool show_test_window    = true;
static bool show_another_window = true;
static ImVec4 clear_color       = ImColor(114, 144, 154);

void CreatorLayer::onDrawImGui()
{
    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    {
        static float f = 0.0f;
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);
        if (ImGui::Button("Test Window"))
            show_test_window ^= 1;
        if (ImGui::Button("Another Window"))
            show_another_window ^= 1;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
    }

    // 2. Show another simple window, this time using an explicit Begin/End pair
    if (show_another_window)
    {
        ImGui::SetNextWindowSize(ImVec2(170, 80), ImGuiCond_FirstUseEver);
        ImGui::Begin("Another Window", &show_another_window);

        ImGui::Text("Hello");
        ImGui::End();
    }

    // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
    if (show_test_window)
    {
        ImGui::ShowDemoWindow();
    }
}