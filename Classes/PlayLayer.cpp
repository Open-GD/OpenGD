#include "PlayLayer.h"
#include "GameToolbox.h"
#include "MenuItemSpriteExtra.h"

USING_NS_AX;

Scene* PlayLayer::scene(GJGameLevel* level) {
    auto scene = Scene::create();
    scene->addChild(PlayLayer::create(level));
    return scene;
}

bool PlayLayer::init(GJGameLevel* level) {
    if (!Layer::init()) return false;
    auto winSize = Director::getInstance()->getWinSize();

    this->m_pGround = GroundLayer::create(1);
    this->addChild(this->m_pGround);
    this->m_pGround->setSpeed(623);

    //temp back button
    auto backbtn = MenuItemSpriteExtra::create("GJ_arrow_01_001.png", [&](Node* btn) {
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MenuLayer::scene()));
        });
    auto menu = Menu::create();
    menu->addChild(backbtn);
    menu->setPosition({ 50, winSize.height - 50 });
    addChild(menu, 99999);

    this->m_pBG = Sprite::create(GameToolbox::getTextureString("game_bg_01_001.png"));
    const Texture2D::TexParams texParams = {
        backend::SamplerFilter::LINEAR,
        backend::SamplerFilter::LINEAR,
        backend::SamplerAddressMode::REPEAT,
        backend::SamplerAddressMode::REPEAT
    };
    this->m_pBG->getTexture()->setTexParameters(texParams);
    this->m_pBG->setTextureRect(Rect(0, 0, 1024 * 5, 1024));
    this->m_pBG->setPosition(winSize / 2);
    this->m_pBG->setColor({ 0, 102, 255 });
    this->addChild(this->m_pBG, -1);


    auto testObject = GameObject::create("square_01_001.png");
    testObject->setPosition(winSize.width / 2, 237);
    _pObjects = { testObject };

    for(GameObject* object : _pObjects)
    {
        object->setOuterBounds(Rect(object->getPosition(), Vec2(60, 60)));
        object->setInnerBounds(Rect(object->getPosition(), Vec2(60, 60)));

        this->addChild(object);
    }

    this->m_pPlayer = PlayerObject::create(GameToolbox::randomInt(1, 12), this);
    this->m_pPlayer->setPosition({ 0, 236 });
    this->addChild(this->m_pPlayer);
    this->m_pPlayer->setAnchorPoint({ 0, 0 });

    m_pPlayer->setOuterBounds(Rect(m_pPlayer->getPosition(), { 30, 30 }));
    m_pPlayer->setInnerBounds(Rect(m_pPlayer->getPosition(), { 30, 30 }));

    this->scheduleUpdate();

    return true;
}

void PlayLayer::update(float dt) {
    float step = std::min(2.0f, dt * 60.0f);

    m_pPlayer->setOuterBounds(Rect(m_pPlayer->getPosition(), { 60, 60 }));
    m_pPlayer->setInnerBounds(Rect(m_pPlayer->getPosition() + Vec2(30, 30), {15, 15}));

    auto winSize = Director::getInstance()->getWinSize();

    if (!this->m_pPlayer->isDead()) {
        step /= 4.0f;
        for (int i = 0; i < 4; i++) {
            this->m_pPlayer->update(step);

            if (this->m_pPlayer->isDead())
                break;
        }
    }

    this->m_pBG->setPositionX(this->m_pBG->getPositionX() - dt * 62);
    if (this->m_pBG->getPositionX() <= -1024)
        this->m_pBG->setPositionX(this->m_pBG->getPositionX() + 1024);

    this->checkCollisions(dt);
    this->updateCamera(dt);
}

void PlayLayer::updateCamera(float dt) {
    auto winSize = Director::getInstance()->getWinSize();
    auto cam = m_obCamPos;

    auto player = this->m_pPlayer;

    float unk4 = 0;

    if (player->getPositionY() <= cam.y + winSize.height - 180) {
        if (player->getPosition().y < cam.y + 240)
            unk4 = player->getPosition().y - 240;
    }
    else {
        unk4 = player->getPosition().y - winSize.height + 180;
    }

    if (player->getLastGroundPos().y == 236 && player->getPositionY() <= cam.y + winSize.height - 180)
        unk4 = 0;

    cam.y += (unk4 - cam.y) / 20;

    if (cam.y < 0)
        cam.y = 0;

    // if (cam.y - winSize.height)
    //     cam.y -= winSize.height;

    if (this->m_bFirstAttempt)
    {
        if (cam.x <= 30.0f)
            cam.x = 30.0f;
    }

    float temp = this->m_fEndOfLevel - winSize.width;
    if (this->m_fEndOfLevel > 0.0f)
    {
        if (cam.x >= temp)
            cam.x = temp;
    }

    if (player->getPositionX() >= winSize.width / 2) { // wrong but works for now
        cam.x += static_cast<float>(dt * 60 * .9 * 5.77);
    }

    this->m_pGround->setPositionX(this->m_pGround->getPositionX() + (cam.x - m_obCamPos.x));
    this->m_pBG->setPositionX(this->m_pBG->getPositionX() + (cam.x - m_obCamPos.x));

    this->m_obCamPos = cam;
    GameToolbox::log("camPosX: {}, camPosY: {}", m_obCamPos.x, m_obCamPos.y);
    Camera::getDefaultCamera()->setPosition(this->m_obCamPos + winSize / 2);
}

void PlayLayer::checkCollisions(float dt)
{
    unsigned int childCount = _pObjects.size();
    
    for (unsigned int i = 0; i < childCount; i++)
    {
        auto object = _pObjects[i];

        if (object->getGameObjectType() == GameObjectType::kObjectTypePlayer) return;


        if (object->getGameObjectType() == GameObjectType::kObjectTypeTile)
        {
            this->m_pPlayer->setDead(object->getOuterBounds().intersectsRect(m_pPlayer->getInnerBounds()));
            m_pPlayer->setOnGround(object->getOuterBounds().intersectsRect(m_pPlayer->getOuterBounds()));

        }
    }
}