#include "MenuLayer.h"
/*
#include "GarageLayer.h"
*/

#include "MenuGameLayer.h"
#include "CreatorLayer.h"
#include "AlertLayer.h"
#include "AudioEngine.h"
#include "MenuItemSpriteExtra.h"
#include "GameToolbox.h"
#include "PlayLayer.h"

/*
#include "ColoursPalette.h"
#include "ListLayer.h"
#include "GJMoreGamesLayer.h"
*/
USING_NS_AX;

bool music = true;

Scene* MenuLayer::scene() {
    auto scene = Scene::create();
    scene->addChild(MenuLayer::create());
    return scene;
}

bool MenuLayer::init(){
    if (!Layer::init()) return false;
    
    if(music){
        AudioEngine::play2d("audiotracks/menuLoop.mp3", true, 0.5f);
        music = false;
    }
    addChild(MenuGameLayer::create(), -1);
    
    float offsetScale = 1.13F;
    auto winSize = Director::getInstance()->getWinSize();
    
    auto log_oSpr = Sprite::createWithSpriteFrameName("GJ_logo_001.png");
    //log_oSpr->setPosition({ winSize.width / 2, winSize.height - 100 });
    log_oSpr->setPosition({ winSize.width / 2, winSize.height - 110 });
    this->addChild(log_oSpr);
    auto playBtn = MenuItemSpriteExtra::create("GJ_playBtn_001.png", [&](Node* btn) {
        auto scene = PlayLayer::scene(new GJGameLevel("My awesome level", "MikaKC", 1));
        Director::getInstance()->pushScene(TransitionFade::create(0.5f, scene));
    });
    auto garageBtn = MenuItemSpriteExtra::create("GJ_garageBtn_001.png", [&](Node* btn) {
        //Director::getInstance()->replaceScene(TransitionFade::create(0.5f, GarageLayer::scene()));
    });    

    auto creatorBtn = MenuItemSpriteExtra::create("GJ_creatorBtn_001.png", [&](Node* btn) {
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, CreatorLayer::scene()));
    });

    auto mainButtonMenu = Menu::create(garageBtn, playBtn, creatorBtn, nullptr);

    mainButtonMenu->setPositionY(mainButtonMenu->getPositionY() -10);
    mainButtonMenu->alignItemsHorizontallyWithPadding(50);
    addChild(mainButtonMenu);
    

    auto robBtn = MenuItemSpriteExtra::create("robtoplogo_small.png", [&](Node* btn) {
        Application::getInstance()->openURL("http://www.robtopgames.com");
    });
    
    auto otherMenu = Menu::create();
    otherMenu->addChild(robBtn);
    addChild(otherMenu);
    
    robBtn->setPosition(otherMenu->convertToNodeSpace({190, 90}));
    robBtn->setScale(0.9f);    
    
    auto achievementsBtn = MenuItemSpriteExtra::create("GJ_achBtn_001.png", [&](Node* btn) {
        AlertLayer::create("coming soon", "this feature has not been added yet!")->show();
    });
    achievementsBtn->setScale(1.f);
    
    auto optionsBtn = MenuItemSpriteExtra::create("GJ_optionsBtn_001.png", [&](Node* btn) {
        
    });

    auto statsBtn = MenuItemSpriteExtra::create("GJ_statsBtn_001.png", [&](Node* btn) {
        auto alert = AlertLayer::create("WIP!", "This feature is not yet supported!", "Close", "Click me!", NULL, NULL);
        alert->setBtn2Callback([=](TextButton*){
            alert->close();
            AlertLayer::create("Woah hello ;)", "apparently you can do this now. its pretty cool!", "Close", "My mood rn", NULL, [=](TextButton*) {
                Application::getInstance()->openURL("https://www.youtube.com/watch?v=XSsRrlM3tNg");
            })->show();
        });
        alert->show();
    });

    auto bottomMenu = Menu::create(achievementsBtn, optionsBtn, statsBtn, nullptr);    

    bottomMenu->setPositionY(90);
    //bottomMenu->setPositionY(100);
    bottomMenu->alignItemsHorizontallyWithPadding(10);

    this->addChild(bottomMenu);

    auto moreGamesBtn = MenuItemSpriteExtra::create("GJ_moreGamesBtn_001.png", [&](Node* btn) {
        //auto a = GJMoreGamesLayer::create();
        //addChild(a);
    });
    otherMenu->addChild(moreGamesBtn);
    //moreGamesBtn->setPosition(menu->convertToNodeSpace({winSize.width - 86, 90}));
    moreGamesBtn->setPosition(otherMenu->convertToNodeSpace({winSize.width - 175, 130}));
    moreGamesBtn->setScale(1.f);

    // To make it close to GD
    otherMenu->setScale(offsetScale);

    
    otherMenu->setPosition({(winSize.width / 2) + 82, (winSize.height / 2) + 47});

    return true;
}