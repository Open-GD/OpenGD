#include "MenuItemSpriteExtra.h"
#include "2d/CCSprite.h"
#include "2d/CCActionEase.h"

USING_NS_AX;

static constexpr float BUTTON_MULTIPLIER = 1.26f;

MenuItemSpriteExtra::MenuItemSpriteExtra(std::string_view spriteStr, Node* sprNode, std::function<void(Node*)> callback) {
	if (spriteStr.empty() && !sprNode) {
		m_pSprite = Sprite::createWithSpriteFrameName("GJ_newBtn_001.png");
	}
	else {
		m_pSprite = spriteStr.empty() ? sprNode : Sprite::createWithSpriteFrameName(spriteStr);
	}
	//m_pSprite->setStretchEnabled(false);
	float nodescale = m_pSprite ? m_pSprite->getScale() : 1.0f;
	m_fScaleMult = BUTTON_MULTIPLIER;
	m_fSelectedScale = nodescale * m_fScaleMult;
	m_fUnselectedScale = nodescale;
	m_fAnimDuration = 0.3f;
	m_fCallback = callback;
}

void MenuItemSpriteExtra::setDestination(ax::Vec2 dest) {
	_altAnim = true;
	_startPos  = m_pSprite->getPosition();
	_offsetPos = ax::Vec2{_startPos.x + dest.x, _startPos.y + dest.y};
}

void MenuItemSpriteExtra::setScaleMultiplier(float s) {
	m_fScaleMult = s;
	m_fSelectedScale = m_fUnselectedScale * s;
}

void MenuItemSpriteExtra::setSpriteFrame(std::string_view frame)
{
	if(auto sprite = dynamic_cast<ax::Sprite*>(m_pSprite))
	{
		sprite->setSpriteFrame(frame);
	}
}
void MenuItemSpriteExtra::selected() {
	if (_altAnim) m_pSprite->runAction(EaseInOut::create(MoveTo::create(m_fAnimDuration, _offsetPos), 1.5f));
	else m_pSprite->runAction(EaseBounceOut::create(ScaleTo::create(m_fAnimDuration, m_fSelectedScale)));
	MenuItemSprite::selected();
}

void MenuItemSpriteExtra::unselected() {
	m_pSprite->stopAllActions();
	if (_altAnim) m_pSprite->runAction(EaseInOut::create(MoveTo::create(m_fAnimDuration, _startPos), 2.0f));
	else m_pSprite->runAction(EaseBounceOut::create(ScaleTo::create(0.4f, m_fUnselectedScale)));
	MenuItemSprite::unselected();
}

void MenuItemSpriteExtra::setScale(float s) {
	m_fUnselectedScale = s;
	m_fSelectedScale = s * m_fScaleMult;
	m_pSprite->setScale(s);
}

void MenuItemSpriteExtra::setCallback(std::function<void(Node*)> callback)
{
	m_fCallback = callback;
}

void MenuItemSpriteExtra::activate() {
	m_pSprite->stopAllActions();
	if (_altAnim) m_pSprite->setPosition(_startPos);
	else m_pSprite->setScale(m_fUnselectedScale);
	m_fCallback(this);
}

Node* MenuItemSpriteExtra::getSprite() { return m_pSprite; }

bool MenuItemSpriteExtra::init() {
	if (!initWithNormalSprite(m_pSprite, m_pSprite, m_pSprite, nullptr)) return false;
	
	m_pSprite->setAnchorPoint({0.5, 0.5});
	m_pSprite->setPosition(m_pSprite->getContentSize() * 0.5f);
	return true;
}

MenuItemSpriteExtra* MenuItemSpriteExtra::create(std::string_view sprite, std::function<void(Node*)> callback) {
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
	MenuItemSpriteExtra* pRet = new(std::nothrow) MenuItemSpriteExtra("", sprite, callback);

	if (pRet && pRet->init()) {
		pRet->autorelease();
		return pRet;
	} else {
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}
