#include "PlayLayer.h"
#include "GameToolbox.h"
#include "MenuItemSpriteExtra.h"
#include "ImGui/ImGuiPresenter.h"
#include "ImGui/imgui/imgui.h"

USING_NS_AX;
USING_NS_AX_EXT;

bool showDn = false;

Scene *PlayLayer::scene(GJGameLevel *level)
{
    auto scene = Scene::create();
    scene->addChild(PlayLayer::create(level));
    return scene;
}

bool PlayLayer::init(GJGameLevel *level)
{
    if (!Layer::init())
        return false;

    auto winSize = Director::getInstance()->getWinSize();

    this->m_pGround = GroundLayer::create(1);
    this->addChild(this->m_pGround);
    this->m_pGround->setSpeed(623);

    // temp back button
    auto backbtn = MenuItemSpriteExtra::create("GJ_arrow_01_001.png", [&](Node *btn)
                                               { Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MenuLayer::scene())); });
    auto menu = Menu::create();
    menu->addChild(backbtn);
    menu->setPosition({50, winSize.height - 50});
    addChild(menu, 99999);

    dn = ax::DrawNode::create();
    dn->setPosition({-15, -15});
    addChild(dn, 99999);

    this->m_pBG = Sprite::create(GameToolbox::getTextureString("game_bg_01_001.png"));
    m_pBG->setStretchEnabled(false);
    const Texture2D::TexParams texParams = {
        backend::SamplerFilter::LINEAR,
        backend::SamplerFilter::LINEAR,
        backend::SamplerAddressMode::REPEAT,
        backend::SamplerAddressMode::REPEAT
    };
    this->m_pBG->getTexture()->setTexParameters(texParams);
    this->m_pBG->setTextureRect(Rect(0, 0, 1024 * 5, 1024));
    this->m_pBG->setPosition(winSize / 2);
    this->m_pBG->setColor({0, 102, 255});
    this->addChild(this->m_pBG, -1);

    auto testObject = GameObject::create("square_01_001.png");
    testObject->setStretchEnabled(false);
    testObject->setPosition(winSize.width / 2, 150);
    auto testObject2 = GameObject::create("square_01_001.png");
    testObject2->setStretchEnabled(false);
    testObject2->setPosition(winSize.width / 2, 120);
    _pObjects = {testObject, testObject2};

    for (GameObject *object : _pObjects)
    {
        object->setOuterBounds(Rect(object->getPosition(), Vec2(30, 30)));
        object->setInnerBounds(Rect(object->getPosition(), Vec2(30, 30)));
        object->setContentSize(Vec2(30, 30));

        this->addChild(object);
    }

    this->m_pPlayer = PlayerObject::create(GameToolbox::randomInt(1, 12), this);
    this->m_pPlayer->setPosition({0, 105});
    this->addChild(this->m_pPlayer);
    this->m_pPlayer->setAnchorPoint({0, 0});

    this->scheduleUpdate();

    return true;
}

double lastY = 0;

void PlayLayer::update(float dt)
{
    float step = std::min(2.0f, dt * 60.0f);
    step *= m_testFloat;

    m_pPlayer->setOuterBounds(Rect(m_pPlayer->getPosition(), {30, 30}));
    m_pPlayer->setInnerBounds(Rect(m_pPlayer->getPosition() + Vec2(15, 15), {7.5, 7.5}));

    auto winSize = Director::getInstance()->getWinSize();

    if (!m_freezePlayer && !this->m_pPlayer->isDead())
    {
        step /= 4.0f;
        lastY = m_pPlayer->getYVel();
        for (int i = 0; i < 4; i++)
        {
            this->m_pPlayer->update(step);

            this->checkCollisions(step);

            if (this->m_pPlayer->isDead())
                break;
        }
    }

    this->m_pBG->setPositionX(this->m_pBG->getPositionX() - dt * 62);
    if (this->m_pBG->getPositionX() <= -1024)
        this->m_pBG->setPositionX(this->m_pBG->getPositionX() + 1024);

    this->updateCamera(dt);
}

void PlayLayer::updateCamera(float dt)
{
    auto winSize = Director::getInstance()->getWinSize();
    auto cam = m_obCamPos;

    auto player = this->m_pPlayer;

    float unk4 = 0;

    if (player->getPositionY() <= cam.y + winSize.height - 180)
    {
        if (player->getPosition().y < cam.y + 240)
            unk4 = player->getPosition().y - 240;
    }
    else
    {
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

    if (player->getPositionX() >= winSize.width / 2)
    { // wrong but works for now
        cam.x += static_cast<float>(dt * 60 * .9 * 5.77);
    }

    this->m_pGround->setPositionX(this->m_pGround->getPositionX() + (cam.x - m_obCamPos.x));
    this->m_pBG->setPositionX(this->m_pBG->getPositionX() + (cam.x - m_obCamPos.x));

    this->m_obCamPos = cam;
    // GameToolbox::log("camPosX: {}, camPosY: {}", m_obCamPos.x, m_obCamPos.y);
    Camera::getDefaultCamera()->setPosition(this->m_obCamPos + winSize / 2);
}

void PlayLayer::checkCollisions(float dt)
{

    if (m_pPlayer->getPositionY() < 105.0f)
    {
        if (m_pPlayer->isGravityFlipped())
        {
            m_pPlayer->setDead(true);
            return;
        }

        m_pPlayer->setPositionY(105.0f);

        m_pPlayer->setOnGround(true);
    }
    else if (m_pPlayer->getPositionY() > 1290.0f)
    {
        m_pPlayer->setDead(true);
        return;
    }

    size_t childCount = _pObjects.size();

    dn->setVisible(showDn);
    dn->clear();

    dn->drawRect({m_pPlayer->getOuterBounds().getMinX(), m_pPlayer->getOuterBounds().getMinY()}, {m_pPlayer->getOuterBounds().getMaxX(), m_pPlayer->getOuterBounds().getMaxY()}, ax::Color4B::RED);
    dn->drawRect({m_pPlayer->getInnerBounds().getMinX(), m_pPlayer->getInnerBounds().getMinY()}, {m_pPlayer->getInnerBounds().getMaxX(), m_pPlayer->getInnerBounds().getMaxY()}, ax::Color4B::RED);

    for (unsigned int i = 0; i < childCount; i++)
    {
        auto object = _pObjects[i];

        dn->drawRect({object->getOuterBounds().getMinX(), object->getOuterBounds().getMinY()}, {object->getOuterBounds().getMaxX(), object->getOuterBounds().getMaxY()}, ax::Color4B::RED);

        if (object->getOuterBounds().intersectsRect(m_pPlayer->getOuterBounds()))
        {
            switch (object->getGameObjectType())
            {
            case GameObjectType::kObjectTypeTile:
                m_pPlayer->setOnGround(true);
                break;

            default:
                break;
            }
        }
        if (object->getOuterBounds().intersectsRect(m_pPlayer->getInnerBounds()))
        {
            switch (object->getGameObjectType())
            {
            case GameObjectType::kObjectTypeTile:
                m_pPlayer->setDead(true);
                break;

            default:
                break;
            }
        }
    }
}

void PlayLayer::onDrawImGui()
{
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    ImGui::Text("Hello, world!");

    ImGui::SliderFloat("Player Speed", &m_testFloat, 0.2f, 3.0f);
    ImGui::Checkbox("Freeze Player", &m_freezePlayer);

    if (ImGui::Button("Back to menu"))
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MenuLayer::scene()));

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);

    ImGui::Text("yVel %.3f", m_pPlayer->getYVel());

    ImGui::Checkbox("Show Hitboxes", &showDn);

    if (ImGui::Button("Speed"))
        Director::getInstance()->getScheduler()->setTimeScale(0.1f);

    if (ImGui::Button("Reset"))
    {
        m_pPlayer->setPosition({2, 230});
        m_obCamPos.x = 0;
        m_pGround->setPositionX(0);
        m_pPlayer->setDead(false);
        m_pBG->setPositionX(0);
    }
}

void PlayLayer::onEnter()
{
    Layer::onEnter();

    auto current = Director::getInstance()->getRunningScene();
    ImGuiPresenter::getInstance()->addRenderLoop("#playlayer", AX_CALLBACK_0(PlayLayer::onDrawImGui, this), current);
}

void PlayLayer::onExit()
{
    ImGuiPresenter::getInstance()->removeRenderLoop("#playlayer");
    Layer::onExit();
}
