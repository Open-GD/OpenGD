#include "MenuItemSpriteExtra.h"

USING_NS_AX;

static constexpr float BUTTON_MULTIPLIER = 1.26f;

MenuItemSpriteExtra::MenuItemSpriteExtra(const char* spriteStr, Node* sprNode, std::function<void(Node*)> callback) {
	m_pSprite = !spriteStr ? sprNode : Sprite::createWithSpriteFrameName(spriteStr);
	//m_pSprite->setStretchEnabled(false);
	float nodescale = m_pSprite->getScale();
	m_fScaleMult = BUTTON_MULTIPLIER;
	m_fSelectedScale = nodescale * m_fScaleMult;
	m_fUnselectedScale = nodescale;
	m_fAnimDuration = 0.3f;
	m_fCallback = callback;
}

void MenuItemSpriteExtra::setScaleMultiplier(float s) {
	m_fScaleMult = s;
	m_fSelectedScale = m_fUnselectedScale * s;
}
void MenuItemSpriteExtra::selected() {
	m_pSprite->runAction(EaseBounceOut::create(ScaleTo::create(m_fAnimDuration, m_fSelectedScale)));
	MenuItemSprite::selected();
}

void MenuItemSpriteExtra::unselected() {
	m_pSprite->stopAllActions();
	m_pSprite->runAction(EaseBounceOut::create(ScaleTo::create(0.4f, m_fUnselectedScale)));
	MenuItemSprite::unselected();
}

void MenuItemSpriteExtra::setScale(float s) {
	m_fUnselectedScale = s;
	m_fSelectedScale = s * m_fScaleMult;
	m_pSprite->setScale(s);
}
void MenuItemSpriteExtra::activate() {
	m_pSprite->stopAllActions();
	m_pSprite->setScale(m_fUnselectedScale);
	m_fCallback(this);
}

Node* MenuItemSpriteExtra::getSprite() { return m_pSprite; }

bool MenuItemSpriteExtra::init() {
	if (!initWithNormalSprite(m_pSprite, m_pSprite, m_pSprite, nullptr)) return false;
	
	m_pSprite->setAnchorPoint({0.5, 0.5});
	m_pSprite->setPosition(m_pSprite->getContentSize() * 0.5f);
	return true;
}

MenuItemSpriteExtra* MenuItemSpriteExtra::create(const char* sprite, std::function<void(Node*)> callback) {
	MenuItemSpriteExtra* pRet = new(std::nothrow) MenuItemSpriteExtra(sprite, nullptr, callback);

	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}

MenuItemSpriteExtra* MenuItemSpriteExtra::create(Node* sprite, std::function<void(Node*)> callback) {
	MenuItemSpriteExtra* pRet = new(std::nothrow) MenuItemSpriteExtra(nullptr, sprite, callback);

	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}