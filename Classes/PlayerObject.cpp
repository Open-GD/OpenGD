#include "PlayerObject.h"
#include "GameToolbox.h"
#include "PlayLayer.h"

USING_NS_AX;

bool PlayerObject::init(int playerFrame, Layer* gameLayer_) {
    // cap the icon limit
    int frame = GameToolbox::inRange(playerFrame, 1, 13);

    auto sprStr1 = StringUtils::format("player_%02d_001.png", frame);
    auto sprStr2 = StringUtils::format("player_%02d_2_001.png", frame);
    GameToolbox::log("1: {}, 2: {}", sprStr1, sprStr2);

    // initialize
    if (!GameObject::init(sprStr1)) return false;

    gameLayer = gameLayer_;
    
    // Check if layer is playlayer
    if (dynamic_cast<PlayLayer*>(gameLayer_) == nullptr) {
        inPlayLayer = false;
    }
    else {
        inPlayLayer = true;
    }

    setTextureRect(Rect(0, 0, 60, 60)); // player hitbox lol

    m_pMainSprite = Sprite::createWithSpriteFrameName(sprStr1);
    addChild(m_pMainSprite, 1);

    m_pSecondarySprite = Sprite::createWithSpriteFrameName(sprStr2);

    m_pMainSprite->addChild(m_pSecondarySprite, -1);
    //secondarySprite->setPosition(mainSprite->convertToNodeSpace(Vec2(0, 0))); // this shit DONT WORK!! cuz rob made it a global var
    //secondarySprite->setPosition(mainSprite->convertToNodeSpace(Vec2(15, 15)));
    m_pSecondarySprite->setPosition(Vec2(30, 30));

    m_pShipSprite = Sprite::createWithSpriteFrameName("ship_01_001.png");
    m_pShipSprite->setVisible(false);
    addChild(m_pShipSprite, 2);

    // particles
    dragEffect1 = ParticleSystemQuad::create("dragEffect.plist");
    dragEffect1->setPositionType(ParticleSystem::PositionType::FREE);
    dragEffect1->stopSystem();

    gameLayer->addChild(dragEffect1, 1);

    dragEffect2 = ParticleSystemQuad::create("dragEffect.plist");
    dragEffect2->setPositionType(ParticleSystem::PositionType::FREE);
    dragEffect2->stopSystem();
    dragEffect2->setPositionY(2);

    gameLayer->addChild(dragEffect2, 1);

    dragEffect3 = ParticleSystemQuad::create("dragEffect.plist");
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

    dragEffect2->setStartColor({ 255, 255, 255, 100 });
    dragEffect2->setEndColor({ 255, 255, 255, 0 });

    dragEffect3->setStartColor({ 255, 255, 255, 190 });
    dragEffect3->setEndColor({ 255, 255, 255, 0 });

    // other particles
    shipDragEffect = ParticleSystemQuad::create("shipDragEffect.plist");
    shipDragEffect->setPositionType(ParticleSystem::PositionType::GROUPED);
    shipDragEffect->stopSystem();

    gameLayer->addChild(shipDragEffect, 1);

    landEffect1 = ParticleSystemQuad::create("landEffect.plist");
    landEffect1->setPositionType(ParticleSystem::PositionType::GROUPED);
    landEffect1->stopSystem();

    gameLayer->addChild(landEffect1, 1);

    landEffect2 = ParticleSystemQuad::create("landEffect.plist");
    landEffect2->setPositionType(ParticleSystem::PositionType::GROUPED);
    landEffect2->stopSystem();

    gameLayer->addChild(landEffect2, 1);

    // streak
    motionStreak = MotionStreak::create(0.3f, 3, 10, { 255, 255, 255 }, "streak.png");
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

void PlayerObject::setMainColor(Color3B col) {
    this->m_pMainSprite->setColor(col);
}

void PlayerObject::setSecondaryColor(Color3B col) {
    this->m_pSecondarySprite->setColor(col);
}

void PlayerObject::setShipColor(Color3B col) {
    this->m_pShipSprite->setColor(col);
}

Color3B PlayerObject::getMainColor() {
    return this->m_pMainSprite->getColor();
}

Color3B PlayerObject::getSecondaryColor() {
    return this->m_pSecondarySprite->getColor();
}

Color3B PlayerObject::getShipColor() {
    return this->m_pShipSprite->getColor();
}

void PlayerObject::update(float dt) {
    // dt *= 60; // rob :skull:

    if (this->m_bIsDead)
        return;

    if (this->getPositionY() <= 236) { // TEMP ON GROUND CHECK
        this->m_bOnGround = true;
        this->stopRotation();
        this->m_dYVel = 0.f;
        this->m_obLastGroundPos = this->getPosition();
    }

    if (this->getPositionX() >= 500 && !this->inPlayLayer) {
        this->m_bIsHolding = true;
    }

    if (!this->m_bIsLocked) {
        this->updateJump(dt * 0.9f);

        this->setPosition(this->getPosition() + Vec2(static_cast<float>(dt * this->m_fSpeed * this->m_dXVel), static_cast<float>(dt * this->m_fSpeed * this->m_dYVel)));
    }

    // if (!this->m_bFlyMode)
    // this->motionStreak->setPosition(this->getPosition() + ccp({-10, 0}));

    // auto particle = Sprite::create("square.png");
    // particle->setScale(0.05);
    // particle->setPosition(this->getPosition());
    // this->gameLayer->addChild(particle, 999);
}

void PlayerObject::updateJump(float dt) {

    if (this->m_bIsHolding && this->m_bOnGround) {
        this->m_bOnGround = false;
        
        this->jump();
        this->runRotateAction();
        
        return;
    }

    if (!this->m_bOnGround)
    {
        this->m_dYVel -= this->m_dGravity * dt;
    }
    else {
        this->stopRotation();
        this->m_dYVel = 0;
    }
}

bool PlayerObject::isFlying() {
    return this->m_bFlying;
}

bool PlayerObject::isUpsideDown() {
    return this->m_bUpsideDown;
}

bool PlayerObject::isDead() {
    return this->m_bIsDead;
}

bool PlayerObject::isOnGround()
{
    return this->m_bOnGround;
}

ax::Vec2 PlayerObject::getLastGroundPos() {
    return this->m_obLastGroundPos;
}

void PlayerObject::logValues() {
    GameToolbox::log("xVel: {} | yVel: {} | gravity: {} | jumpHeight: {} ", m_dXVel, m_dYVel, m_dGravity, m_dJumpHeight);
}

void PlayerObject::runRotateAction() {
    this->stopRotation();
    auto action = RotateBy::create(0.43333f, 180);
    action->setTag(0);
    this->runAction(action);
}

void PlayerObject::stopRotation() {
    this->stopActionByTag(0);

    if (this->getRotation() != 0) {
        auto degrees = (int)this->getRotation() % 360;
        this->setRotation(90 * roundf(degrees / 90.0f));
    }
}

void PlayerObject::jump() {
    this->m_dYVel = this->m_dJumpHeight;
}

// void PlayerObject::jump() {
    // this->runAction(
    //     Sequence::create(
    //         Spawn::create(
    //             Sequence::create(
    //                 RotateBy::create(0.15, 80.f),
    //                 RotateBy::create(0.025, 0.f),
    //                 RotateBy::create(0.225, 80.f),
    //                 RotateBy::create(0, -160),
    //                 nullptr
    //             ),
    //             Sequence::create(
    //                 //MoveBy::create(0.025, { 0, 80.f }),
    //                 //MoveBy::create(0.15, { 0, 50.f }),
    //                 //MoveBy::create(0.05, { 0.f, 0.f }),
    //                 //MoveBy::create(0.15, { 0, -80.f }),
    //                 //MoveBy::create(0.025, { 0, -50.f }),
    //                 MoveBy::create(0.2, {0, 130.f}),
    //                 MoveBy::create(0.2, { 0, -130.f }),
    //                 nullptr
    //             ),
    //             nullptr
    //         ),
    //         nullptr
    //     )
    // );
// }
bool PlayerObject::onTouchBegan(ax::Touch* touch, ax::Event* event)
{
    if (this->inPlayLayer) {
        m_bIsHolding = true;
        return true;
    }
    return false;
}

void PlayerObject::onTouchEnded(ax::Touch* touch, ax::Event* event)
{
    if (this->inPlayLayer)
    {
        m_bIsHolding = false;
    }
}

PlayerObject* PlayerObject::create(int playerFrame, Layer* gameLayer) {
    auto pRet = new (std::nothrow) PlayerObject();

    if (pRet && pRet->init(playerFrame, gameLayer)) {
        pRet->autorelease();
        return pRet;
    }
    AX_SAFE_DELETE(pRet);
    return pRet;
}