#include "PlayerObject.h"
#include "GameToolbox.h"
#include "PlayLayer.h"
#include "AudioEngine.h"
#include "PlayLayer.h"

USING_NS_AX;

void PlayerObject::playDeathEffect() {
    AudioEngine::stopAll();
    AudioEngine::play2d("explode_11.ogg", false, 0.1f);

    getPlayLayer()->unscheduleUpdate();

    scheduleOnce([=](float d) {
        static_cast<PlayLayer *>(getPlayLayer())->resetLevel();
    }, 1.f, "playerdeath");

    runAction(FadeTo::create(0.2f, 0));
}

bool PlayerObject::init(int playerFrame, Layer *gameLayer_)
{
    // cap the icon limit
    int frame = GameToolbox::inRange(playerFrame, 1, 13);

    auto sprStr1 = StringUtils::format("player_%02d_001.png", frame);
    auto sprStr2 = StringUtils::format("player_%02d_2_001.png", frame);
    GameToolbox::log("1: {}, 2: {}", sprStr1, sprStr2);

    // initialize
    if (!GameObject::init(sprStr1))
        return false;

    gameLayer = gameLayer_;

    // Check if layer is playlayer
    if (dynamic_cast<PlayLayer *>(gameLayer_) == nullptr)
    {
        inPlayLayer = false;
    }
    else
    {
        inPlayLayer = true;
    }

    setTextureRect(Rect(0, 0, 30, 30)); // player hitbox lol

    m_pMainSprite = Sprite::createWithSpriteFrameName(sprStr1);
    m_pMainSprite->setStretchEnabled(false);
    addChild(m_pMainSprite, 1);

    m_pSecondarySprite = Sprite::createWithSpriteFrameName(sprStr2);
    m_pSecondarySprite->setStretchEnabled(false);

    m_pMainSprite->addChild(m_pSecondarySprite, -1);
    // secondarySprite->setPosition(mainSprite->convertToNodeSpace(Vec2(0, 0))); // this shit DONT WORK!! cuz rob made it a global var
    m_pSecondarySprite->setPosition({15, 15});

    m_pShipSprite = Sprite::createWithSpriteFrameName("ship_01_001.png");
    m_pShipSprite->setStretchEnabled(false);
    m_pShipSprite->setVisible(false);
    addChild(m_pShipSprite, 2);

    // particles
    dragEffect1 = ParticleSystemQuad::create("Resources/dragEffect.plist");
    dragEffect1->setPositionType(ParticleSystem::PositionType::FREE);
    dragEffect1->stopSystem();

    gameLayer->addChild(dragEffect1, 1);

    dragEffect2 = ParticleSystemQuad::create("Resources/dragEffect.plist");
    dragEffect2->setPositionType(ParticleSystem::PositionType::FREE);
    dragEffect2->stopSystem();
    dragEffect2->setPositionY(2);

    gameLayer->addChild(dragEffect2, 1);

    dragEffect3 = ParticleSystemQuad::create("Resources/dragEffect.plist");
    dragEffect3->setPositionType(ParticleSystem::PositionType::FREE);
    dragEffect3->stopSystem();
    dragEffect3->setPositionY(2);

    gameLayer->addChild(dragEffect3, 1);

    // particle properties
    dragEffect2->setSpeed(dragEffect2->getSpeed() * 0.2f);
    dragEffect2->setSpeedVar(dragEffect2->getSpeedVar() * 0.2f);

    dragEffect3->setSpeed(dragEffect3->getSpeed() * 0.2f);
    dragEffect3->setSpeedVar(dragEffect3->getSpeedVar() * 0.2f);
    dragEffect3->setAngleVar(dragEffect3->getAngleVar() * 2.f);
    dragEffect3->setStartSize(dragEffect3->getStartSize() * 1.5f);
    dragEffect3->setStartSizeVar(dragEffect3->getStartSizeVar() * 1.5f);

    dragEffect2->setStartColor({255, 255, 255, 100});
    dragEffect2->setEndColor({255, 255, 255, 0});

    dragEffect3->setStartColor({255, 255, 255, 190});
    dragEffect3->setEndColor({255, 255, 255, 0});

    // other particles
    shipDragEffect = ParticleSystemQuad::create("Resources/shipDragEffect.plist");
    shipDragEffect->setPositionType(ParticleSystem::PositionType::GROUPED);
    shipDragEffect->stopSystem();

    gameLayer->addChild(shipDragEffect, 1);

    landEffect1 = ParticleSystemQuad::create("Resources/landEffect.plist");
    landEffect1->setPositionType(ParticleSystem::PositionType::GROUPED);
    landEffect1->stopSystem();

    gameLayer->addChild(landEffect1, 1);

    landEffect2 = ParticleSystemQuad::create("Resources/landEffect.plist");
    landEffect2->setPositionType(ParticleSystem::PositionType::GROUPED);
    landEffect2->stopSystem();

    gameLayer->addChild(landEffect2, 1);

    // streak
    motionStreak = MotionStreak::create(0.3f, 3, 10, {255, 255, 255}, "streak.png");
    motionStreak->setBlendFunc(BlendFunc::ADDITIVE);

    gameLayer->addChild(motionStreak);

    motionStreak->setStartingPositionInitialized(false);

    // scheduleUpdate();

    auto dir = Director::getInstance();
    auto listener = EventListenerTouchOneByOne::create();

    listener->setEnabled(true);
    listener->setSwallowTouches(true);

    // trigger when you start touch
    listener->onTouchBegan = AX_CALLBACK_2(PlayerObject::onTouchBegan, this);
    listener->onTouchEnded = AX_CALLBACK_2(PlayerObject::onTouchEnded, this);

    dir->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void PlayerObject::setMainColor(Color3B col)
{
    this->m_pMainSprite->setColor(col);
}

void PlayerObject::setSecondaryColor(Color3B col)
{
    this->m_pSecondarySprite->setColor(col);
}

void PlayerObject::setShipColor(Color3B col)
{
    this->m_pShipSprite->setColor(col);
}

Color3B PlayerObject::getMainColor()
{
    return this->m_pMainSprite->getColor();
}

Color3B PlayerObject::getSecondaryColor()
{
    return this->m_pSecondarySprite->getColor();
}

Color3B PlayerObject::getShipColor()
{
    return this->m_pShipSprite->getColor();
}

void PlayerObject::update(float dt)
{
    if (this->m_bIsDead)
        return;

    if (!this->m_bIsLocked)
    {
        float dtSlow = dt * 0.9f;
        this->updateJump(dtSlow);

        float velY = (float)((double)dtSlow * m_dYVel);
        float velX = (float)((double)dtSlow * m_dXVel * (!m_bIsPlatformer ? 1.f : direction));

        ax::Vec2 velocity{velX, velY};

        setPosition(getPosition() + velocity);
    }

    if (this->getPositionX() >= 500 && !this->inPlayLayer)
    {
        this->m_bIsHolding = true;
    }

    // if (!this->m_bFlyMode)
    // this->motionStreak->setPosition(this->getPosition() + ccp({-10, 0}));

    // auto particle = Sprite::create("square.png");
    // particle->setStretchEnabled(false);
    // particle->setScale(0.05);
    // particle->setPosition(this->getPosition());
    // this->gameLayer->addChild(particle, 999);
}
void PlayerObject::updateShipRotation()
{
    Vec2 pos = getPosition();

}
void PlayerObject::updateJump(float dt)
{
    float localGravity = m_dGravity;

    const int flipGravityMult = flipMod();

    //leave this here so we dont have to rewrite stuff in the future
    float playerSize = 1.0;

    if (this->isShip()/* || this->m_isUFO || this->m_isWave*/)
    {
        float upperVelocityLimit = 8.0 / playerSize;
        float lowerVelocityLimit = -6.4 / playerSize;

        if(this->isShip())
        {
            float shipAccel = 0.8f;

            if(this->m_bIsHolding) shipAccel = -1.0f;

            if(!this->m_bIsHolding && !this->playerIsFalling())
            {
                shipAccel = 1.2f;
            }

            float extraBoost = 0.4f;
            if(this->m_bIsHolding && this->playerIsFalling())
                extraBoost = 0.5;

            this->m_dYVel -= localGravity * dt * flipGravityMult * shipAccel * extraBoost / playerSize;
        }
        if(!this->isGravityFlipped())
        {
            if(this->m_dYVel <= lowerVelocityLimit) this->m_dYVel = lowerVelocityLimit;
        }
        else
        {
            if(this->m_dYVel <= -upperVelocityLimit) this->m_dYVel = -upperVelocityLimit;

            upperVelocityLimit = 6.4f / playerSize;
        }
        if(this->m_dYVel >= upperVelocityLimit) this->m_dYVel = upperVelocityLimit;
    }
    else
    {
        float gravityMultiplier = 1.0f;

        bool shouldJump = m_bIsHolding;

        if (shouldJump && isOnGround())
        {
            m_bIsRising = true;
            m_bOnGround = false;

            float jumpAccel = m_dJumpHeight;

            m_dYVel = flipGravityMult * jumpAccel;

            runRotateAction();
        }
        else
        {
            float tfIsThisGravity = m_dGravity;

            if (m_bIsRising)
            {
                m_dYVel -= localGravity * dt * flipGravityMult * gravityMultiplier;
                bool condition;
                if (!isGravityFlipped())
                {
                    condition = m_dYVel <= tfIsThisGravity * 2.0f;
                }
                else
                {
                    condition = m_dYVel >= m_dGravity * 2.0f;
                }

                if (condition)
                {
                    m_bIsRising = false;
                }
            }
            else
            {
                if (!isGravityFlipped())
                {
                    if (m_dYVel < tfIsThisGravity * 2.0f)
                    {
                        m_bOnGround = false;
                    }
                }
                else
                {
                    if (m_dYVel > m_dGravity * 2.0f)
                    {
                        m_bOnGround = false;
                    }
                }

                m_dYVel -= localGravity * dt * flipGravityMult * gravityMultiplier;
                if (!isGravityFlipped())
                {
                    m_dYVel = std::max(m_dYVel, -15.0);
                }
                else
                {
                    m_dYVel = std::min(m_dYVel, 15.0);
                }
                if (!isUpsideDown())
                {
                    if (m_dYVel >= tfIsThisGravity * 2.0f)
                        return;
                }
                else
                {
                    if (m_dYVel <= m_dGravity * 2.0f)
                        return;
                }

                if (getActionByTag(0) == nullptr)
                    runRotateAction();

                if (isUpsideDown())
                {
                    if (m_dYVel >= -4.0)
                        return;
                }
                else
                {
                    if (m_dYVel <= 4.0)
                        return;
                }
            }
        }
    }
}

bool PlayerObject::playerIsFalling()
{
    if(this->isGravityFlipped())
    {
        return this->m_dYVel > this->m_dGravity;
    }
    else
    {
        return this->m_dYVel < this->m_dGravity;
    }
}

void PlayerObject::collidedWithObject(float dt, GameObject *obj)
{
    Vec2 pos = getPosition();
    Rect rect = obj->getOuterBounds();

    Rect playerRectO = getOuterBounds();
    Rect playerRectI = getInnerBounds();

    float flipModV = flipMod();

    float mod = flipModV * 10.0f;

    if (isShip())
        mod = flipModV * 6.0f;

    float topP = (pos.y + (playerRectO.origin.height * -0.5f * -flipMod())) - mod;
    float bottomP = (pos.y + (playerRectO.origin.height * -0.5f * flipMod())) + mod;

    float MaxY = rect.getMaxY();
    float MinY = rect.getMinY();

    // h
    float MaxYP = playerRectO.getMaxY();
    float MinYP = playerRectO.getMinY();

    float t = topP;
    float b = bottomP;

    if (isGravityFlipped())
    {
        if (!isShip())
            goto topCollision;
        t = bottomP;
        b = topP;
    }
    if (b >= MaxY || t >= MaxY)
    {
        if (m_dYVel < 0.0f)
        {
            checkSnapJumpToObject(obj);
            // idk snapping to Y
            playerRectI.origin.x = rect.origin.x;
            if (playerRectI.intersectsRect(rect))
            {
                playerRectI.origin.x = pos.x;
                ((PlayLayer*)getPlayLayer())->destroyPlayer();
                return;
            }
            if (MaxYP >= (MinY + MaxY) / 2.f)
            {
                setPositionY(MaxY);
                hitGround(isGravityFlipped() ? isShip() : false);
            }
        }
    }
    
    if (!isGravityFlipped())
    {
        if(!isShip())
            goto death;
        t = bottomP;
        b = topP;
    }
    topCollision:
    if (b <= MinY || t <= MinY)
    {
        if (m_dYVel > 0.0f)
        {
            checkSnapJumpToObject(obj);
            // idk snapping to Y
            playerRectI.origin.x = rect.origin.x;
            if (playerRectI.intersectsRect(rect))
            {
                playerRectI.origin.x = pos.x;
                ((PlayLayer*)getPlayLayer())->destroyPlayer();
                return;
            }

            if (MinYP <= (MinY + MaxY) / 2.f)
            {
                setPositionY(MinY - 30);
                hitGround(!isGravityFlipped() ? isShip() : false);
            }
        }
    }
    death:
    if (playerRectI.intersectsRect(rect))
        ((PlayLayer*)getPlayLayer())->destroyPlayer();
}

void PlayerObject::checkSnapJumpToObject(GameObject *obj)
{
    if (obj) 
    {
        if (m_snappedObject && m_snappedObject->getID() != obj->getID() && m_snappedObject->getGameObjectType() == kGameObjectTypeSolid)
        {
            Vec2 oldSnapPos = m_snappedObject->getPosition();
            Vec2 newSnapPos = obj->getPosition();

            float unknownUse = 1.0;
            float upTwoGap = 90.0;
            float downOneGap = 150.0;
            float upOneGap = 90.0;
            float xShift = 1.0;

            if (m_playerSpeed == 0.9) 
            {
                upOneGap = 120.0;
            }
            else if (m_playerSpeed == 0.7) 
            {
                upTwoGap = 60.0;
                downOneGap = 120.0;
            }
            else if (m_playerSpeed == 1.1) 
            {
                unknownUse = 0.0;
                xShift = 2.00;
                upTwoGap = 120.0;
                downOneGap = 195.0;
                upOneGap = 150.0;
            }
            else if (m_playerSpeed == 1.3) 
            {
                unknownUse = 0.0;
                xShift = 2.00;
                upTwoGap = 135.0;
                downOneGap = 225.0;
                upOneGap = 180.0;
            }
            else 
            {
                upOneGap = 120.0;
            }

            upOneGap += oldSnapPos.x;
            downOneGap += oldSnapPos.x;

            float someMultiplier = (isUpsideDown() ? 30.0 : -30.0);

            if (unknownUse >= upOneGap)
                oldSnapPos.y = fabs(newSnapPos.x - upOneGap) + someMultiplier;

            float value1 = fabs(newSnapPos.y - (oldSnapPos.y + someMultiplier));
            float value2 = fabs(newSnapPos.y - (oldSnapPos.y + someMultiplier * 2));
            float value3 = fabs(newSnapPos.x - downOneGap);
            float value4 = fabs(newSnapPos.x - (upTwoGap + oldSnapPos.x));
            float value5 = fabs(newSnapPos.y - oldSnapPos.y);

            if (
                (unknownUse >= upOneGap && unknownUse >= value5) ||
                (unknownUse >= value3 && unknownUse >= value1) ||
                (unknownUse >= value4 && unknownUse >= value2)
                ) 
            {
                float newPos = obj->getPositionX() + this->m_snapDifference;
                float oldPos = this->getPositionX();

                if (xShift < fabs(newPos - oldPos)) {
                    if (newPos > oldPos) {
                        newPos += oldPos;
                    }
                    else {
                        newPos = oldPos - xShift;
                    }
                }
                this->setPositionX(newPos);
            }
        }

        m_snappedObject = obj;
        m_snapDifference = this->getPositionX() - obj->getPositionX();
    }
}
void PlayerObject::hitGround(bool reverseGravity)
{
    setOnGround(true);
    m_dYVel = 0.0f;
    if(getActionByTag(0))
        stopRotation();
    m_obLastGroundPos = getPosition();
}
float PlayerObject::flipMod()
{
    return this->m_bGravityFlipped ? -1.0f : 1.0f;
}

bool PlayerObject::isGravityFlipped()
{
    return this->m_bGravityFlipped;
}

bool PlayerObject::isShip()
{
    return this->m_bIsShip;
}

bool PlayerObject::isUpsideDown()
{
    return this->m_bUpsideDown;
}

bool PlayerObject::isDead()
{
    return this->m_bIsDead;
}

bool PlayerObject::isOnGround()
{
    return this->m_bOnGround;
}

ax::Vec2 PlayerObject::getLastGroundPos()
{
    return this->m_obLastGroundPos;
}

void PlayerObject::logValues()
{
    GameToolbox::log("xVel: {} | yVel: {} | gravity: {} | jumpHeight: {} ", m_dXVel, m_dYVel, m_dGravity, m_dJumpHeight);
}

void PlayerObject::runRotateAction()
{
    stopRotation();
    auto action = RotateBy::create(0.43333f, 180);
    action->setTag(0);
    runAction(action);
}

void PlayerObject::stopRotation()
{
    stopActionByTag(0);

    if (getActionByTag(1) == nullptr)
    {
        if (getRotation() != 0)
        {
            auto degrees = (int)getRotation() % 360;
            auto action = RotateTo::create(0.075f, (90 * roundf(degrees / 90.0f)));
            action->setTag(1);
            runAction(action);
        }
    }
}

void PlayerObject::jump()
{
    this->m_dYVel = this->m_dJumpHeight;
}
bool PlayerObject::onTouchBegan(ax::Touch *touch, ax::Event *event)
{
    if (this->inPlayLayer)
    {
        m_bIsHolding = true;
        return true;
    }
    return false;
}

void PlayerObject::onTouchEnded(ax::Touch *touch, ax::Event *event)
{
    if (this->inPlayLayer)
    {
        m_bIsHolding = false;
    }
}

PlayerObject *PlayerObject::create(int playerFrame, Layer *gameLayer)
{
    auto pRet = new (std::nothrow) PlayerObject();

    if (pRet && pRet->init(playerFrame, gameLayer))
    {
        pRet->autorelease();
        return pRet;
    }
    AX_SAFE_DELETE(pRet);
    return pRet;
}