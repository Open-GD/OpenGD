#include "PlayLayer.h"
#include "GameToolbox.h"
#include "MenuItemSpriteExtra.h"
#include "ImGui/ImGuiPresenter.h"
#include "ImGui/imgui/imgui.h"
#include "AudioEngine.h"
#include "EffectGameObject.h"
#include "LevelTools.h"
#include <fstream>

USING_NS_AX;
USING_NS_AX_EXT;

bool showDn = false, noclip = false, ship = false;

float gameSpeed = 1, fps = 0;

static PlayLayer *Instance = nullptr;

Scene *PlayLayer::scene(GJGameLevel *level)
{
    auto scene = Scene::create();
    scene->addChild(PlayLayer::create(level));
    return scene;
}


int PlayLayer::sectionForPos(float x)
{
    int section = x / 100;
    if (section < 0)
        section = 0;
    return section;
}
    
PlayLayer* PlayLayer::create(GJGameLevel *level)
{
    auto ret = new (std::nothrow) PlayLayer();
    if (ret && ret->init(level))
    {
        ret->autorelease();
        return ret;
    }
    
    AX_SAFE_DELETE(ret);
    return nullptr;
}
    
std::vector<std::string> split(std::string &tosplit, char splitter)
{
    std::vector<std::string> vec;
    std::istringstream ss(tosplit);
    std::string token;
    while (std::getline(ss, token, splitter))
    {
        vec.push_back(token);
    }
    return vec;
}

void PlayLayer::fillColorChannel(std::vector<std::string> &colorString, int id)
{
    for (size_t j = 0; j < colorString.size() - 1; j += 2)
    {
        switch (std::stoi(colorString[j]))
        {
        case 1:
            m_pColorChannels.insert({1001, ax::Color3B(std::stof(colorString[j + 1]), 0, 0)});
            break;
        case 2:
            m_pColorChannels.at(1001).g = std::stof(colorString[j + 1]);
            break;
        case 3:
            m_pColorChannels.at(1001).b = std::stof(colorString[j + 1]);
            break;
        }
    }
}

void PlayLayer::loadLevel(std::string levelStr)
{
    std::vector<std::string> objData = split(levelStr, ';'), levelData;

    levelData = split(objData[0], ',');
    objData.erase(objData.begin());

    for (size_t i = 0; i < levelData.size(); i += 2)
    {
        if (levelData[i] == "kS1")
        {
            m_pColorChannels.insert({1000, ax::Color3B(std::stof(levelData[i + 1]), 0, 0)});
        }
        else if (levelData[i] == "kS2")
        {
            m_pColorChannels.at(1000).g = std::stof(levelData[i + 1]);
        }
        else if (levelData[i] == "kS3")
        {
            m_pColorChannels.at(1000).b = std::stof(levelData[i + 1]);
        }
        else if (levelData[i] == "kS4")
        {
            m_pColorChannels.insert({1001, ax::Color3B(std::stof(levelData[i + 1]), 0, 0)});
        }
        else if (levelData[i] == "kS5")
        {
            m_pColorChannels.at(1001).g = std::stof(levelData[i + 1]);
        }
        else if (levelData[i] == "kS6")
        {
            m_pColorChannels.at(1001).b = std::stof(levelData[i + 1]);
        }
        else if (levelData[i] == "kS29")
        {
            auto colorString = split(levelData[i + 1], '_');
            fillColorChannel(colorString, 1000);
        }
        else if (levelData[i] == "kS30")
        {
            auto colorString = split(levelData[i + 1], '_');
            fillColorChannel(colorString, 1001);
        }
        else if (levelData[i] == "kS31")
        {
            auto colorString = split(levelData[i + 1], '_');
            fillColorChannel(colorString, 1002);
        }
        else if (levelData[i] == "kS32")
        {
            auto colorString = split(levelData[i + 1], '_');
            fillColorChannel(colorString, 1004);
        }
        else if (levelData[i] == "kS37")
        {
            auto colorString = split(levelData[i + 1], '_');
            fillColorChannel(colorString, 1003);
        }
        else if (levelData[i] == "kS38")
        {
            auto colorString = split(levelData[i + 1], '|');
            for (std::string colorData : colorString)
            {
                auto innerData = split(colorData, '_');
                int key;
                Color3B col;
                for (size_t j = 0; j < innerData.size() - 1; j += 2)
                {
                    switch (std::stoi(innerData[j]))
                    {
                    case 1:
                        col.r = std::stof(innerData[j + 1]);
                        break;
                    case 2:
                        col.g = std::stof(innerData[j + 1]);
                        break;
                    case 3:
                        col.b = std::stof(innerData[j + 1]);
                        break;
                    case 6:
                        key = std::stoi(innerData[j + 1]);
                    }
                }
                m_pColorChannels.insert({key, col});
            }
        }
    }

    for (std::string data : objData)
    {
        auto d = split(data, ',');

        GameObject *obj = nullptr;

        Hitbox hb = {0, 0, 0, 0};

        for (size_t i = 0; i < d.size(); i += 2)
        {
            int key = std::stoi(d[i]);

            if (key != 1 && obj == nullptr)
                break;

            switch (key)
            {
            case 1:
            {
                int id = std::stoi(d[i + 1]);

                if (std::find(std::begin(GameObject::_pTriggers), std::end(GameObject::_pTriggers), id) != std::end(GameObject::_pTriggers))
                    obj = EffectGameObject::create((std::string)GameObject::_pBlocks.at(id) + ".png");
                else
                    obj = GameObject::create((std::string)GameObject::_pBlocks.at(id) + ".png");

                if (obj == nullptr)
                    break;

                obj->setStretchEnabled(false);
                obj->setActive(true);
                obj->setID(id);
                obj->updateObjectType();

                if (GameObject::_pHitboxes.contains(id))
                {
                    hb = GameObject::_pHitboxes.at(id);
                }

                _pObjects.push_back(obj);
            }
            break;
            case 2:
                obj->setPositionX(std::stof(d[i + 1]));
                break;
            case 3:
                obj->setPositionY(std::stof(d[i + 1]) + 90.0f);
                break;
            case 4:
                obj->setFlippedX(std::stoi(d[i + 1]));
                break;
            case 5:
                obj->setFlippedY(std::stoi(d[i + 1]));
                break;
            case 6:
                obj->setRotation(std::stof(d[i + 1]));
                break;
            case 7:
                dynamic_cast<EffectGameObject *>(obj)->m_cColor.r = std::stof(d[i + 1]);
                break;
            case 8:
                dynamic_cast<EffectGameObject *>(obj)->m_cColor.g = std::stof(d[i + 1]);
                break;
            case 9:
                dynamic_cast<EffectGameObject *>(obj)->m_cColor.b = std::stof(d[i + 1]);
                break;
            case 10:
                dynamic_cast<EffectGameObject *>(obj)->m_fDuration = std::stof(d[i + 1]);
                break;
            case 21:
                obj->_mainColorChannel = std::stoi(d[i + 1]);
                break;
            case 22:
                obj->_secColorChannel = std::stoi(d[i + 1]);
                break;
            case 23:
                dynamic_cast<EffectGameObject *>(obj)->m_nTargetColorId = std::stof(d[i + 1]);
                break;
            case 25:
                obj->setGlobalZOrder(std::stoi(d[i + 1]));
                break;
            }
        }
        if (obj)
        {
            if (std::find(std::begin(GameObject::_pSolids), std::end(GameObject::_pSolids), obj->getID()) != std::end(GameObject::_pSolids))
                obj->setGameObjectType(kGameObjectTypeSolid);
            else if (std::find(std::begin(GameObject::_pTriggers), std::end(GameObject::_pTriggers), obj->getID()) != std::end(GameObject::_pTriggers))
            {
                obj->setGameObjectType(kGameObjectTypeSpecial);
                obj->setVisible(false);
            }
            else
            {
                switch (obj->getID())
                {
                case 10:
                    obj->setGameObjectType(kGameObjectTypeNormalGravityPortal);
                    break;
                case 11:
                    obj->setGameObjectType(kGameObjectTypeInverseGravityPortal);
                    break;
                case 12:
                    obj->setGameObjectType(kGameObjectTypeCubePortal);
                    break;
                case 13:
                    obj->setGameObjectType(kGameObjectTypeShipPortal);
                    break;
                case 35:
                    obj->setGameObjectType(kGameObjectTypeYellowJumpPad);
                    break;
                case 36:
                    obj->setGameObjectType(kGameObjectTypeYellowJumpRing);
                    break;
                default:
                    obj->setGameObjectType(kGameObjectTypeHazard);
                    break;
                }
            }

            switch (obj->getGameObjectType())
            {
            case kGameObjectTypeYellowJumpRing:
            case kGameObjectTypeYellowJumpPad:
            case kGameObjectTypeNormalGravityPortal:
            case kGameObjectTypeInverseGravityPortal:
            case kGameObjectTypeCubePortal:
            case kGameObjectTypeShipPortal:
            case kGameObjectTypeSolid:
            case kGameObjectTypeSpecial:
            case kGameObjectTypeHazard:
            {
                obj->setOuterBounds(Rect(obj->getPosition() + Vec2(hb.x, hb.y) + Vec2(15, 15), {hb.w, hb.h}));
                break;
            }
            }
        }
    }
}

bool PlayLayer::init(GJGameLevel *level)
{
    if (!Layer::init())
        return false;

    setLevel(level);

    Instance = this;

    auto winSize = Director::getInstance()->getWinSize();

    this->m_pGround = GroundLayer::create(1);
    this->addChild(this->m_pGround, 1);

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

    std::string levelStr = FileUtils::getInstance()->getStringFromFile("level.txt");
    loadLevel(levelStr);

    if (_pObjects.size() != 0)
    {
        this->m_lastObjXPos = std::numeric_limits<float>().min();

        for (GameObject *object : _pObjects)
            if (this->m_lastObjXPos < object->getPositionX())
                this->m_lastObjXPos = object->getPositionX();

        if (this->m_lastObjXPos < 570.f)
            this->m_lastObjXPos = 570.f;

        for (size_t i = 0; i < sectionForPos(this->m_lastObjXPos); i++)
        {
            std::vector<GameObject *> vec;
            m_pSectionObjects.push_back(vec);
        }

        for (GameObject *object : _pObjects)
        {
            int section = sectionForPos(object->getPositionX());
            m_pSectionObjects[section - 1 < 0 ? 0 : section - 1].push_back(object);

            this->addChild(object);
        }
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

    scheduleOnce([=](float d)
                 {
        AudioEngine::play2d(LevelTools::getAudioFilename(getLevel()->_MusicID), false, 0.1f);
        scheduleUpdate();
        m_pPlayer->setIsDead(false); },
                 1.f, "k");

    return true;
}

double lastY = 0;

void PlayLayer::update(float dt)
{
    if (m_freezePlayer)
    {
        AudioEngine::pauseAll();
        return;
    }
    else
    {
        AudioEngine::resumeAll();
    }

    Vec2 playerPosOld = m_pPlayer->getPosition();

    float step = std::min(2.0f, dt * 60.0f);

    m_pPlayer->m_bIsPlatformer = m_platformerMode;
    m_pPlayer->noclip = noclip;

    auto winSize = Director::getInstance()->getWinSize();

    if (this->m_pColorChannels.contains(1000))
        this->m_pBG->setColor(this->m_pColorChannels.at(1000));

    if (!m_freezePlayer && !this->m_pPlayer->isDead())
    {
        step /= 4.0f;
        lastY = m_pPlayer->getYVel();
        for (int i = 0; i < 4; i++)
        {
            this->m_pPlayer->update(step);

            m_pPlayer->setOuterBounds(Rect(m_pPlayer->getPosition(), {30, 30}));
            m_pPlayer->setInnerBounds(Rect(m_pPlayer->getPosition() + Vec2(11.25f, 11.25f), {7.5, 7.5}));

            this->checkCollisions(step);
            if (this->m_pPlayer->isDead())
                break;
        }
    }

    m_pBar->setPercentage(m_pPlayer->getPositionX() / this->m_lastObjXPos * 100.f);

    if (this->m_pBG->getPositionX() <= -1024)
        this->m_pBG->setPositionX(this->m_pBG->getPositionX() + 1024);

    this->updateCamera(step * 4.f);

    Vec2 playerPosNew = m_pPlayer->getPosition();
}

void PlayLayer::destroyPlayer()
{
    if (m_pPlayer->noclip) return;
    m_pPlayer->setIsDead(true);
    m_pPlayer->playDeathEffect();
}

void PlayLayer::updateCamera(float dt)
{
    auto winSize = Director::getInstance()->getWinSize();
    auto cam = m_obCamPos;

    auto player = this->m_pPlayer;

    float playerPosY = player->getPositionY();
    float playerPosX = player->getPositionX();

    float unk4 = 0;
    float unk5 = 20;

    if (!player->isShip())
    {

        float unk2 = 180.0f;
        float unk3 = 120.0f;

        if (player->isGravityFlipped())
        {
            std::swap(unk2, unk3);
        }

        if (playerPosY <= cam.y + winSize.height - unk2)
        {
            if (playerPosY < cam.y + unk3)
                unk4 = playerPosY - unk3;
        }
        else
            unk4 = playerPosY - winSize.height + unk2;

        if (player->getLastGroundPos().y == 105 && playerPosY <= cam.y + winSize.height - unk2)
            unk4 = 0;
    }
    else
    {
        unk4 = 0.0f;
        if (this->m_fCameraYCenter + (winSize.height / -2.0f > 0.0f))
            unk4 = this->m_fCameraYCenter + (winSize.height / -2.0f);
        unk5 = 30.0f;
    }

    cam.y += (unk4 - cam.y) / unk5;

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

    if (playerPosX >= winSize.width / 2.5f && !player->isDead() && !player->m_bIsPlatformer) // wrong but works for now
    {
        this->m_pBG->setPositionX(this->m_pBG->getPositionX() - dt * .9f * m_pGround->getSpeed() * 0.1175f);
        m_pGround->update(dt * .9f);
        cam.x += dt * .9f * 5.770002f;
    }
    else if (player->m_bIsPlatformer)
        cam.x = playerPosX - winSize.width / 2.f;

    this->m_pGround->setPositionX(this->m_pGround->getPositionX() + (cam.x - m_obCamPos.x));
    this->m_pBG->setPositionX(this->m_pBG->getPositionX() + (cam.x - m_obCamPos.x));

    this->m_obCamPos = cam;
    // GameToolbox::log("camPosX: {}, camPosY: {}", m_obCamPos.x, m_obCamPos.y);
    Camera::getDefaultCamera()->setPosition(this->m_obCamPos + winSize / 2);
    
    m_pBar->setPosition(this->m_obCamPos + winSize / 2);
    m_pBar->setPositionY((this->m_obCamPos + winSize).height - 10);
}

void PlayLayer::updateVisibility()
{
    auto winSize = ax::Director::getInstance()->getWinSize();

    float unk = 70.0f;

    int prevSection = floorf(this->m_obCamPos.x / 100) - 1.0f;
    int nextSection = ceilf((this->m_obCamPos.x + winSize.width) / 100) + 1.0f;

    for (size_t i = prevSection; i < nextSection; i++)
    {
        if (i > 0)
        {
            if (i < m_pSectionObjects.size())
            {
                auto section = m_pSectionObjects[i];
                for (size_t j = 0; j < section.size(); j++)
                {

                    GameObject *obj = section[j];

                    obj->setActive(true);

                    // if (obj->getType() == GameObjectType::kBallFrame || obj->getType() == GameObjectType::kYellowJumpRing)
                    //     obj->setScale(this->getAudioEffectsLayer()->getAudioScale())

                    // auto pos = obj->getPosition();

                    // float unk2 = 0.0f;

                    // if (obj->getGameObjectType == GameObjectType::)
                    // {
                    //     unk2 = obj->getTextureRect().origin.x * obj->getScaleX() * 0.4f;
                    // }

                    // unsigned char opacity = this->getRelativeMod(pos, unk, unk2) * 255.0f;

                    // if (!obj->getDontTransform())
                    // {
                    //     obj->setOpacity(opacity);
                    //     this->applyEnterEffect(obj);
                    // }
                }
            }
        }
    }

    for (size_t i = this->_prevSection; i < this->_nextSection; i++)
    {
        if (i > 0)
        {
            if (i < this->m_pSectionObjects.size())
            {
                auto section = m_pSectionObjects[i];
                for (size_t j = 0; j < section.size(); j++)
                {
                    section[j]->setActive(false);
                }
            }
        }
    }
    this->_prevSection = prevSection;
    this->_nextSection = nextSection;
}

void PlayLayer::changeGameMode(GameObject* obj, int gameMode)
{
    switch(gameMode)
    {
        case 0:
        {
            this->m_pPlayer->setIsShip(false);
        }
        break;
        case 1:
        {
            this->m_fCameraYCenter = obj->getPositionY(); // TODO check if portal is lower than certan y pos, if so set center to predefined pointS
            this->m_pPlayer->setIsShip(true);
        }
    }
}
void PlayLayer::moveCameraToPos(Vec2 pos)
{
    auto moveX = [this](float a, float b, float c) -> void
    {
        this->stopActionByTag(0);
        auto tweenAction = ActionTween::create(b, "cTX", m_obCamPos.x, a);
        auto easeAction = EaseInOut::create(tweenAction, c);
        easeAction->setTag(0);
        this->runAction(easeAction);
    };
    auto moveY = [this](float a, float b, float c) -> void
    {
        this->stopActionByTag(1);
        auto tweenAction = ActionTween::create(b, "cTY", m_obCamPos.y, a);
        auto easeAction = EaseInOut::create(tweenAction, c);
        easeAction->setTag(1);
        this->runAction(easeAction);
    };
    moveX(pos.x, 1.2f, 1.8f);
    moveY(pos.y, 1.2f, 1.8f);
}

void PlayLayer::processTriggers()
{
    int current_section = this->sectionForPos(m_pPlayer->getPositionX());
    if (m_pSectionObjects.size() == 0)
        return;

    std::vector<GameObject *> section = m_pSectionObjects[sectionForPos(m_pPlayer->getPositionX()) <= 0 ? 0 : sectionForPos(m_pPlayer->getPositionX()) - 1];

    int i = 0;
    while (i < section.size())
    {
        i++;
    }
}

void PlayLayer::checkCollisions(float dt)
{
    auto playerOuterBounds = this->m_pPlayer->getOuterBounds();

    if (m_pPlayer->getPositionY() < 105.0f)
    {
        if (m_pPlayer->isGravityFlipped())
        {
            destroyPlayer();
            return;
        }

        m_pPlayer->setPositionY(105.0f);

        m_pPlayer->hitGround(false);
    }
    else if (m_pPlayer->getPositionY() > 1290.0f)
    {
        destroyPlayer();
        return;
    }

    dn->setVisible(showDn);
    dn->clear();

    renderRect(playerOuterBounds, ax::Color4B::RED);
    renderRect(m_pPlayer->getInnerBounds(), ax::Color4B::GREEN);

    int current_section = this->sectionForPos(m_pPlayer->getPositionX());

    std::deque<GameObject *> m_pHazards;

    for (int i = current_section - 2; i < current_section + 1; i++)
    {
        if (i < m_pSectionObjects.size() && i >= 0)
        {
            std::vector<GameObject *> section = m_pSectionObjects[i];

            for (int j = 0; j < section.size(); j++)
            {
                GameObject *obj = section[j];

                auto objBounds = obj->getOuterBounds();

                if (objBounds.size.width <= 0 || objBounds.size.height <= 0)
                    continue;

                if (obj->getGameObjectType() == kGameObjectTypeHazard)
                {
                    m_pHazards.push_back(obj);
                    renderRect(objBounds, ax::Color4B::RED);
                }
                else if (obj->isActive())
                {
                    if (!obj->m_bHasBeenActivated)
                    {
                        if (auto trigger = dynamic_cast<EffectGameObject *>(obj))
                        {
                            if (trigger->getPositionX() <= m_pPlayer->getPositionX())
                                trigger->triggerActivated(dt);
                        }
                    }
                    renderRect(objBounds, ax::Color4B::BLUE);
                    if (playerOuterBounds.intersectsRect(objBounds))
                    {
                        // GameToolbox::log("game object type 2: {}", obj->getGameObjectType());
                        switch (obj->getGameObjectType())
                        {
                        case GameObjectType::kGameObjectTypeInverseGravityPortal:
                            //if (!m_pPlayer->isGravityFlipped())
                            //    this->playGravityEffect(true);

                            //m_pPlayer->setPortal(obj->getPosition());

                            m_pPlayer->flipGravity(true);
                            break;

                        case GameObjectType::kGameObjectTypeNormalGravityPortal:
                            //if (m_pPlayer->isGravityFlipped())
                            //    this->playGravityEffect(false);

                            //m_pPlayer->setPortal(obj->getPosition());

                            m_pPlayer->flipGravity(false);
                            break;
                            
                        case GameObjectType::kGameObjectTypeShipPortal:
                            this->changeGameMode(obj, 1);
                            break;

                        case GameObjectType::kGameObjectTypeCubePortal:
                            this->changeGameMode(obj, 0);
                            // this->getPlayer()->setPortal(obj->getPosition());

                            //this->m_pPlayer->setIsShip(false);
                            /* this->toggleGlitter(false);
                            this->animateOutGround(false);

                            this->moveCameraY = false; */
                            break;

                        case GameObjectType::kGameObjectTypeYellowJumpPad:
                            if (!obj->m_bHasBeenActivated)
                            {
                                //obj->triggerActivated();

                                m_pPlayer->propellPlayer();
                            }
                            break;

                        case GameObjectType::kGameObjectTypeYellowJumpRing:
                            if (!obj->m_bHasBeenActivated)
                            {
                                //this->getPlayer()->setTouchedRing(obj);
                                //obj->powerOnObject();

                                //this->getPlayer()->ringJump();
                            }
                            break;
                        case GameObjectType::kGameObjectTypeSpecial:
                            break;
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

        if (playerOuterBounds.intersectsRect(hazard->getOuterBounds()))
        {
            destroyPlayer();
            return;
        }
    }
    m_pHazards.clear();
}

void PlayLayer::onDrawImGui()
{
    extern bool _showDebugImgui;
    if (!_showDebugImgui)
        return;
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    ImGui::SetNextWindowPos({1000.0f, 200.0f}, ImGuiCond_FirstUseEver);

    ImGui::Begin("PlayLayer Debug");
    ImGui::Text("Hello, world!");

    ImGui::Checkbox("Freeze Player", &m_freezePlayer);
    ImGui::Checkbox("Platformer Mode (Basic)", &m_platformerMode);

    if (ImGui::Button("Back to menu"))
    {
        AudioEngine::stopAll();
        AudioEngine::play2d("quitSound_01.ogg", false, 0.1f);
        unscheduleUpdate();
        music = true;
        Instance = nullptr;

        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MenuLayer::scene()));
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);

    ImGui::Text("yVel %.3f", m_pPlayer->getYVel());

    ImGui::Checkbox("Show Hitboxes", &showDn);
    ImGui::Checkbox("Gain the power of invincibility", &noclip);

    if (ImGui::InputFloat("Speed", &gameSpeed))
        Director::getInstance()->getScheduler()->setTimeScale(gameSpeed);

    if (ImGui::InputFloat("FPS", &fps))
        Director::getInstance()->setAnimationInterval(1.0f / fps);

    ImGui::Text("Sections: %i", m_pSectionObjects.size());
    if (m_pSectionObjects.size() > 0 && sectionForPos(m_pPlayer->getPositionX()) - 1 < m_pSectionObjects.size())
        ImGui::Text("Current Section Size: %i", m_pSectionObjects[sectionForPos(m_pPlayer->getPositionX()) <= 0 ? 0 : sectionForPos(m_pPlayer->getPositionX()) - 1].size());

    if (ImGui::Checkbox("Become ship", &ship))
        this->m_pPlayer->setIsShip(ship);

    if (ImGui::Button("Reset"))
    {
        this->resetLevel();
    }
    ImGui::End();
}

void PlayLayer::resetLevel()
{
    m_pPlayer->setPosition({2, 105});
    m_obCamPos.x = 0;
    m_pGround->setPositionX(0);
    m_pPlayer->reset();
    m_pBG->setPositionX(0);

    for (auto obj : this->_pObjects)
    {
        obj->m_bHasBeenActivated = false;
        obj->setActive(false);
    }

    AudioEngine::stopAll();
    AudioEngine::play2d(LevelTools::getAudioFilename(getLevel()->_MusicID), false, 0.1f);
    scheduleUpdate();
}
void PlayLayer::renderRect(ax::Rect rect, ax::Color4B col)
{
    dn->drawRect({rect.getMinX(), rect.getMinY()}, {rect.getMaxX(), rect.getMaxY()}, col);
    dn->drawSolidRect({rect.getMinX(), rect.getMinY()}, {rect.getMaxX(), rect.getMaxY()}, Color4B(col.r, col.g, col.b, 100));
}

void PlayLayer::onEnter()
{
    Layer::onEnter();

    auto listener = EventListenerKeyboard::create();
    auto dir = Director::getInstance();

    listener->onKeyPressed = AX_CALLBACK_2(PlayLayer::onKeyPressed, this);
    listener->onKeyReleased = AX_CALLBACK_2(PlayLayer::onKeyReleased, this);
    dir->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    auto current = dir->getRunningScene();
    ImGuiPresenter::getInstance()->addRenderLoop("#playlayer", AX_CALLBACK_0(PlayLayer::onDrawImGui, this), current);
}

void PlayLayer::onExit()
{
    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
    ImGuiPresenter::getInstance()->removeRenderLoop("#playlayer");
    Layer::onExit();
}

void PlayLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event)
{
    GameToolbox::log("Key with keycode {} pressed", static_cast<int>(keyCode));
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_R:
    {
        this->resetLevel();
    }
    break;
    case EventKeyboard::KeyCode::KEY_F:
    {
        extern bool _showDebugImgui;
        _showDebugImgui = !_showDebugImgui;
    }
    break;
    case EventKeyboard::KeyCode::KEY_SPACE:
    {
        m_pPlayer->onTouchBegan(nullptr, nullptr);
    }
    break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    {
        m_pPlayer->onTouchBegan(nullptr, nullptr);
    }
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_A)
        m_pPlayer->direction = -1.f;
    else if (keyCode == EventKeyboard::KeyCode::KEY_D)
        m_pPlayer->direction = 1.f;
}

void PlayLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
    GameToolbox::log("Key with keycode {} released", static_cast<int>(keyCode));
    if (
        (keyCode == EventKeyboard::KeyCode::KEY_A && m_pPlayer->direction == -1.f) || (keyCode == EventKeyboard::KeyCode::KEY_D && m_pPlayer->direction == 1.f))
        m_pPlayer->direction = 0.f;
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_SPACE:
    {
        m_pPlayer->onTouchEnded(nullptr, nullptr);
    }
    break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    {
        m_pPlayer->onTouchEnded(nullptr, nullptr);
    }
    }
}

PlayLayer *PlayLayer::getInstance()
{
    return Instance;
}