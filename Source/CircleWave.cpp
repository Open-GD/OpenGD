#include "CircleWave.h"
#include "glad/gl.h"
#include "CCDirector.h"
#include "2d/CCActionInstant.h"
#include "2d/CCActionManager.h"
#include "2d/CCActionEase.h"

USING_NS_AX;

void CircleWave::draw(ax::Renderer *renderer, const ax::Mat4& transform, uint32_t flags) 
{
   // log_ << "ASSA";
	glBlendFunc(0x302, 1);
	glLineWidth(this->_lineWidth);

	if (this->_filled)
		this->drawSolidCircle(this->getPosition(), this->_radius, 0.0f, (this->_radius <= 400.0) ? (30) : (60), this->_color);
	else
		this->drawCircle(this->getPosition(), this->_radius, 0.0f, this->_radius <= 400.0 ? 30 : 60, false, this->_color);

	glBlendFunc(1u, 0x303u);

	this->drawSolidCircle(Director::getInstance()->getWinSize() / 2, 100, 90, 100, {1,1,1,1});
}

void CircleWave::setColor(Color4B col) 
{
	this->_color = {col.r / 255.f, col.g / 255.f, col.b / 255.f, col.a / 255.f};
}

Color4B CircleWave::getColor() 
{
	return {(uint8_t)(_color.r * 255.f), (uint8_t)(_color.g * 255.f), (uint8_t)(_color.b * 255.f), (uint8_t)(_color.a * 255.f)};
}

bool CircleWave::init(float duration, Color4B color, float radiusMin, float radiusMax, bool easing, float lineWidth, bool filled) 
{
	if (!DrawNode::init()) return false;

	this->setColor(color);
	this->_radius = radiusMin;
	this->_lineWidth = lineWidth;
	this->_filled = filled;

	if (easing)
	{
		this->_color.a = 0;
		auto radAction = ActionTween::create(duration, "radius", radiusMin, radiusMax);
		auto opacityAction = ActionTween::create(duration / 2, "opacity", 0.0, 1.0);
		auto opacityAction2 = ActionTween::create(duration / 2, "opacity", 1.0, 0.0);
		auto seq = Sequence::create(opacityAction, opacityAction2, CallFunc::create([=]() {
			this->removeFromParent();
		}), nullptr);
		auto action = Spawn::create(radAction, seq, nullptr);
		Director::getInstance()->getActionManager()->addAction(action, this, false);
	}
	else 
	{
		auto radAction = EaseOut::create(ActionTween::create(duration, "radius", radiusMin, radiusMax), 2.0f);
		auto opacityAction = EaseOut::create(ActionTween::create(duration / 2, "opacity", this->_color.a, 0.0), 2.0f);
		auto spawn = Spawn::create(radAction, opacityAction, nullptr);
		auto action = Sequence::create(spawn, CallFunc::create([=](){ this->removeFromParent();}), nullptr);
		Director::getInstance()->getActionManager()->addAction(action, this, false);
	}

	this->scheduleUpdate();

	return true;
}

void CircleWave::updateTweenAction(float value, std::string_view key)
{
	if (key == "radius") {
		this->_radius = value;
	} else if (key == "opacity") {
		this->_color.a = value;
	}
}

void CircleWave::update(float dt)
{
	if (this->_followedNode) {
		this->setPosition(_followedNode->getPosition() / 2);
	}

	this->clear();
	glBlendFunc(0x302, 1);
	glLineWidth(this->_lineWidth);

	if (this->_filled)
		this->drawSolidCircle(this->getPosition(), this->_radius, 0.0f, (this->_radius <= 400.0) ? (30) : (60), this->_color);
	else
		this->drawCircle(this->getPosition(), this->_radius, 0.0f, this->_radius <= 400.0 ? 30 : 60, false, this->_color);

	glBlendFunc(1u, 0x303u);
}

void CircleWave::followNode(Node* node) {
	this->_followedNode = node;
}

CircleWave* CircleWave::create(float duration, Color4B color, float radiusMin, float radiusMax, bool easing, bool filled, float lineWidth)
{
	auto pRet = new(std::nothrow) CircleWave();

	if (pRet && pRet->init(duration, color, radiusMin, radiusMax, easing, lineWidth, filled))
	{
		pRet->autorelease();
		return pRet;
	} 
	else
	{
		AX_SAFE_DELETE(pRet);
		return nullptr;
	}
}