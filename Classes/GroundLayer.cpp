#include "GroundLayer.h"
#include "GameToolbox.h"

USING_NS_AX;

bool GroundLayer::init(int groundID) {
    if (!Layer::init()) return false;

    auto winSize = Director::getInstance()->getWinSize();
    
    auto name = fmt::format("groundSquare_{:03}.png", groundID);
    this->m_pSprite = Sprite::create(GameToolbox::getTextureString(name));
    this->m_fOneGroundSize = this->m_pSprite->getTextureRect().size.width;
    this->m_pSprite->getTexture()->setTexParameters({ backend::SamplerFilter::NEAREST, backend::SamplerFilter::NEAREST, backend::SamplerAddressMode::REPEAT, backend::SamplerAddressMode::REPEAT });
    this->m_pSprite->setTextureRect({0, 0, winSize.width + this->m_fOneGroundSize, this->m_pSprite->getTextureRect().size.height });
    this->m_pSprite->setAnchorPoint({0, 0});
    this->m_pSprite->setPosition({0, -50});
    this->m_pSprite->setColor({0, 102, 255});
    this->addChild(this->m_pSprite);

    auto line = Sprite::create(GameToolbox::getTextureString("floor.png"));
    this->addChild(line);
    line->setPosition({winSize.width / 2, this->m_pSprite->getContentSize().height + this->m_pSprite->getPositionY()});
       
    auto gradient1 = Sprite::createWithSpriteFrameName("groundSquareShadow_001.png");
    this->addChild(gradient1);
    gradient1->setScale(1.6f);
    
    auto gradient2 = Sprite::createWithSpriteFrameName("groundSquareShadow_001.png");
    this->addChild(gradient2);
    gradient2->setScale(1.6f);
    gradient2->setFlippedX(true);
    gradient2->setPositionX(winSize.width);

    this->m_fSpeed = 700;
    
    scheduleUpdate();
    
    return true;
}


void GroundLayer::update(float dt) {
    this->m_pSprite->setPositionX(this->m_pSprite->getPositionX() - dt * this->m_fSpeed);

    if (this->m_pSprite->getPositionX() <= -this->m_fOneGroundSize)
        this->m_pSprite->setPositionX(this->m_pSprite->getPositionX() + this->m_fOneGroundSize);
}

GroundLayer* GroundLayer::create(int groundID) {
    GroundLayer* pRet = new(std::nothrow) GroundLayer();

    if (pRet && pRet->init(groundID)) {
        pRet->autorelease();
        return pRet;
    } else {
        AX_SAFE_DELETE(pRet);
        return nullptr;
    }
}