#include "PlayLayer.h"
#include "GameToolbox.h"
#include "MenuItemSpriteExtra.h"
#include "ImGui/ImGuiPresenter.h"
#include "ImGui/imgui/imgui.h"
#include "AudioEngine.h"
#include "LevelTools.h"

USING_NS_AX;
USING_NS_AX_EXT;

bool showDn = false, noclip = false;

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

    setLevel(level);

    auto winSize = Director::getInstance()->getWinSize();

    this->m_pGround = GroundLayer::create(1);
    this->addChild(this->m_pGround);

    dn = ax::DrawNode::create();
    dn->setPosition({-15, -15});
    addChild(dn, 99999);

    this->m_pBG = Sprite::create(GameToolbox::getTextureString("game_bg_01_001.png"));
    m_pBG->setStretchEnabled(false);
    const Texture2D::TexParams texParams = {
        backend::SamplerFilter::LINEAR,
        backend::SamplerFilter::LINEAR,
        backend::SamplerAddressMode::REPEAT,
        backend::SamplerAddressMode::REPEAT};
    this->m_pBG->getTexture()->setTexParameters(texParams);
    this->m_pBG->setTextureRect(Rect(0, 0, 1024 * 5, 1024));
    this->m_pBG->setPosition(winSize.x / 2, winSize.y / 4);
    this->m_pBG->setColor({0, 102, 255});
    this->addChild(this->m_pBG, -1);

    if(_pObjects.size() != 0) {
        for (size_t i = 0; i < sectionForPos(_pObjects[_pObjects.size() - 1]->getPositionX()); i++)
        {
            std::vector<GameObject *> vec;
            m_pSectionObjects.push_back(vec);
        }
    }

    for (GameObject *object : _pObjects)
    {
        int section = sectionForPos(object->getPositionX());
        m_pSectionObjects[section - 1 < 0 ? 0 : section - 1].push_back(object);
        object->setOuterBounds(Rect(object->getPosition(), Vec2(30, 30)));
        object->setInnerBounds(Rect(object->getPosition(), Vec2(30, 30)));
        object->setContentSize(Vec2(30, 30));

        this->addChild(object);
    }

    this->m_pPlayer = PlayerObject::create(GameToolbox::randomInt(1, 12), this);
    this->m_pPlayer->setPosition({-20, 105});
    this->addChild(this->m_pPlayer);
    this->m_pPlayer->setAnchorPoint({0, 0});

    this->m_pBar = SimpleProgressBar::create();
    this->m_pBar->setPercentage(0.f);
    this->m_pBar->setPosition({winSize.width / 2, winSize.height - 20});
    this->addChild(m_pBar);

    m_pBar->setPosition(this->m_obCamPos + winSize / 2);
    m_pBar->setPositionY((this->m_obCamPos + winSize).height - 10);

    scheduleOnce([=](float d) {
        AudioEngine::play2d(LevelTools::getAudioFilename(getLevel()->_MusicID), false, 0.1f);
        scheduleUpdate();
        m_pPlayer->setDead(false);
    }, 1.f, "k");

    return true;
}

double lastY = 0;

void PlayLayer::update(float dt)
{
    float step = std::min(2.0f, dt * 60.0f);

    m_pPlayer->setOuterBounds(Rect(m_pPlayer->getPosition(), {30, 30}));
    m_pPlayer->setInnerBounds(Rect(m_pPlayer->getPosition() + Vec2(11.25f, 11.25f), {7.5, 7.5}));
    m_pPlayer->noclip = noclip;

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

    float sSize = 570.f; // after this point level end layer is called
    if(this->_pObjects.size() != 0)
        sSize = this->_pObjects[this->_pObjects.size() - 1]->getPositionX();

    m_pBar->setPercentage(m_pPlayer->getPositionX() / sSize * 100.f);

    if (this->m_pBG->getPositionX() <= -1024)
        this->m_pBG->setPositionX(this->m_pBG->getPositionX() + 1024);

    this->updateCamera(step * 4.f);
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
        unk4 = player->getPosition().y - winSize.height + 180;

    if (player->getLastGroundPos().y == 236 && player->getPositionY() <= cam.y + winSize.height - 180)
        unk4 = 0;

    cam.y += (unk4 - cam.y) / 20;

    if (cam.y < 0)
        cam.y = 0;

    // if (cam.y - winSize.height)
    //     cam.y -= winSize.height;

    if (this->m_bFirstAttempt)
        if (cam.x <= 30.0f)
            cam.x = 30.0f;

    float temp = this->m_fEndOfLevel - winSize.width;
    if (this->m_fEndOfLevel > 0.0f)
        if (cam.x >= temp)
            cam.x = temp;

    if (player->getPositionX() >= winSize.width / 2.5f && !player->isDead()) // wrong but works for now
    {
        this->m_pBG->setPositionX(this->m_pBG->getPositionX() - dt * .9f * m_pGround->getSpeed() * 0.1175f);
        m_pGround->update(dt * .9f);
        cam.x += dt * .9f * 5.770002f;
    }
        

    this->m_pGround->setPositionX(this->m_pGround->getPositionX() + (cam.x - m_obCamPos.x));
    this->m_pBG->setPositionX(this->m_pBG->getPositionX() + (cam.x - m_obCamPos.x));

    this->m_obCamPos = cam;
    // GameToolbox::log("camPosX: {}, camPosY: {}", m_obCamPos.x, m_obCamPos.y);
    Camera::getDefaultCamera()->setPosition(this->m_obCamPos + winSize / 2);

    m_pBar->setPosition(this->m_obCamPos + winSize / 2);
    m_pBar->setPositionY((this->m_obCamPos + winSize).height - 10);
}

void PlayLayer::checkCollisions(float dt)
{
    if (m_pPlayer->getPositionY() < 105.0f)
    {
        if (m_pPlayer->isGravityFlipped())
        {
            if (!noclip)
                m_pPlayer->setDead(true);
            return;
        }

        m_pPlayer->setPositionY(105.0f);

        m_pPlayer->hitGround(false);
    }
    else if (m_pPlayer->getPositionY() > 1290.0f)
    {
        if (!noclip)
            m_pPlayer->setDead(true);
        return;
    }

    dn->setVisible(showDn);
    dn->clear();

    renderRect(m_pPlayer->getOuterBounds(), ax::Color4B::RED);
    renderRect(m_pPlayer->getInnerBounds(), ax::Color4B::GREEN);

    int current_section = this->sectionForPos(m_pPlayer->getPositionX());

    std::vector<GameObject *> m_pHazards;

    for (int i = current_section - 1; i < current_section + 1; i++)
    {
        if (i < m_pSectionObjects.size() && i >= 0)
        {
            std::vector<GameObject *> section = m_pSectionObjects[i];

            for (int j = 0; j < section.size(); j++)
            {
                GameObject *obj = section[j];

                renderRect(obj->getOuterBounds(), ax::Color4B::BLUE);

                if (obj->getGameObjectType() == GameObjectType::kObjectTypeHazard)
                {
                    m_pHazards.push_back(obj);
                }
                else if (obj->isActive())
                {
                    if (m_pPlayer->getOuterBounds().intersectsRect(obj->getOuterBounds()))
                    {
                        switch (obj->getGameObjectType())
                        {
                        /* case GameObjectType::kInvertGravity:
                            if (!this->getPlayer()->getGravityFlipped())
                                this->playGravityEffect(true);

                            this->getPlayer()->setPortal(obj->getPosition());

                            this->getPlayer()->flipGravity(true);
                            break;

                        case GameObjectType::kNormalGravity:
                            if (this->getPlayer()->getGravityFlipped())
                                this->playGravityEffect(false);

                            this->getPlayer()->setPortal(obj->getPosition());

                            this->getPlayer()->flipGravity(false);
                            break;

                        case GameObjectType::kShipPortal:
                            this->switchToFlyMode(obj, false);
                            break;

                        case GameObjectType::kCubePortal:
                            this->getPlayer()->setPortal(obj->getPosition());

                            this->getPlayer()->toggleFlyMode(false);
                            this->toggleGlitter(false);
                            this->animateOutGround(false);

                            this->moveCameraY = false;
                            break;

                        case GameObjectType::kYellowJumpPad:
                            if (!obj->hasBeenActivated())
                            {
                                this->getPlayer()->setPortal(obj->getPosition() - cocos2d::CCPoint(0, 10));
                                obj->triggerActivated();

                                this->getPlayer()->propellPlayer();
                            }
                            break;

                        case GameObjectType::kYellowJumpRing:
                            if (!obj->hasBeenActivated())
                            {
                                this->getPlayer()->setTouchedRing(obj);
                                obj->powerOnObject();

                                this->getPlayer()->ringJump();
                            }
                            break; */
                        default:
                            m_pPlayer->collidedWithObject(dt, obj);
                            break;
                        }
                    }
                }
            }
        }
    }
    for (unsigned int i = 0; i < m_pHazards.size(); ++i)
    {
        GameObject *hazard = m_pHazards[i];

        if (m_pPlayer->getOuterBounds().intersectsRect(hazard->getOuterBounds()))
        {
            m_pPlayer->setDead(true);
            return;
        }
    }
    m_pHazards.clear();
}

void PlayLayer::onDrawImGui()
{
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    ImGui::Text("Hello, world!");

    ImGui::Checkbox("Freeze Player", &m_freezePlayer);

    if (ImGui::Button("Back to menu")) {
        AudioEngine::stopAll();
        AudioEngine::play2d("quitSound_01.ogg", false, 0.1f);
        unscheduleUpdate();
        music = true;

        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MenuLayer::scene()));
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);

    ImGui::Text("yVel %.3f", m_pPlayer->getYVel());

    ImGui::Checkbox("Show Hitboxes", &showDn);
    ImGui::Checkbox("Gain the power of invincibility", &noclip);

    if (ImGui::Button("Speed"))
        Director::getInstance()->getScheduler()->setTimeScale(0.1f);

    ImGui::Text("Sections: %i", m_pSectionObjects.size());
    if (m_pSectionObjects.size() > 0 && sectionForPos(m_pPlayer->getPositionX()) - 1 < m_pSectionObjects.size())
        ImGui::Text("Current Section Size: %i", m_pSectionObjects[sectionForPos(m_pPlayer->getPositionX()) <= 0 ? 0 : sectionForPos(m_pPlayer->getPositionX()) - 1].size());

    if (ImGui::Button("Reset"))
    {
        m_pPlayer->setPosition({2, 105});
        m_obCamPos.x = 0;
        m_pGround->setPositionX(0);
        m_pPlayer->setDead(false);
        m_pBG->setPositionX(0);
    }
}

void PlayLayer::renderRect(ax::Rect rect, ax::Color4B col)
{
    dn->drawRect({rect.getMinX(), rect.getMinY()}, {rect.getMaxX(), rect.getMaxY()}, col);
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
