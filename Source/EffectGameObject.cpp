#include "EffectGameObject.h"
#include "BaseGameLayer.h"
#include "GameToolbox.h"
#include "PlayLayer.h"

USING_NS_AX;

void EffectGameObject::triggerActivated(float idk)
{
	if (!_bgl)
		return;

	auto pl = PlayLayer::getInstance();

	this->_wasTriggerActivated = true;

	auto id = getID();

	if (!_bgl->_colorChannels.contains(_targetColorId))
	{
		_bgl->_colorChannels.insert({_targetColorId, SpriteColor(Color3B::WHITE, 255, 0)});
		_bgl->_originalColors.insert({_targetColorId, SpriteColor(Color3B::WHITE, 255, 0)});
	}

	if (id == 29)
		_targetColorId = 1000;
	else if (id == 30)
		_targetColorId = 1001;

	switch (id)
	{
	case 30:
	case 29:
	case 899: {
		this->runAction(
			ActionTween::create(this->_duration, "col1", _bgl->_colorChannels.at(_targetColorId)._color.r, _color.r));
		this->runAction(
			ActionTween::create(this->_duration, "col2", _bgl->_colorChannels.at(_targetColorId)._color.g, _color.g));
		this->runAction(
			ActionTween::create(this->_duration, "col3", _bgl->_colorChannels.at(_targetColorId)._color.b, _color.b));
		break;
	}
	case 22:
		if (pl)
			pl->_enterEffectID = 1;
		break;
	case 23:
		if (pl)
			pl->_enterEffectID = 5;
		break;
	case 24:
		if (pl)
			pl->_enterEffectID = 4;
		break;
	case 25:
		if (pl)
			pl->_enterEffectID = 6;
		break;
	case 26:
		if (pl)
			pl->_enterEffectID = 7;
		break;
	case 27:
		if (pl)
			pl->_enterEffectID = 2;
		break;
	case 28:
		if (pl)
			pl->_enterEffectID = 3;
		break;
	case 1007:
		runAction(ActionTween::create(_duration, "fade", _bgl->_groups[_targetGroupId]._alpha, _opacity));
		break;
	case 1006:
		if (!_bgl->_colorChannels.contains(_targetGroupId))
		{
			_bgl->_colorChannels.insert({_targetGroupId, SpriteColor(Color3B::WHITE, 255, 0)});
			_bgl->_originalColors.insert({_targetGroupId, SpriteColor(Color3B::WHITE, 255, 0)});
		}
		Color3B original = _bgl->_colorChannels.at(_targetGroupId)._color;

		Color3B target = _color;

		if (_pulseMode)
		{
			if (_copiedColorId == -1)
				target = original;
			else
				target = _bgl->_colorChannels.at(_copiedColorId)._color;
			auto hsv = ax::HSV(original);
			// if(_hsv.h != 0) hsv.h += _hsv.h + 180;
			// if(_saturationTicked) hsv.s *= _hsv.s;
			// else hsv.s += _hsv.s;
			// if(_brightnessTicked) hsv.v *= _hsv.v;
			// else hsv.v += _hsv.v;
			target = hsv.toColor3B();
		}
		auto seq1 = ax::Sequence::create({ActionTween::create(_fadeIn, "pul1", original.r, target.r),
										  ActionTween::create(_hold, "pul1", target.r, target.r),
										  ActionTween::create(_fadeOut, "pul1", target.r, original.r)});

		auto seq2 = ax::Sequence::create({ActionTween::create(_fadeIn, "pul2", original.g, target.g),
										  ActionTween::create(_hold, "pul2", target.g, target.g),
										  ActionTween::create(_fadeOut, "pul2", target.g, original.g)});

		auto seq3 = ax::Sequence::create({ActionTween::create(_fadeIn, "pul3", original.b, target.b),
										  ActionTween::create(_hold, "pul3", target.b, target.b),
										  ActionTween::create(_fadeOut, "pul3", target.b, original.b)});

		this->runAction(seq1);
		this->runAction(seq2);
		this->runAction(seq3);
		break;
	}
}

void EffectGameObject::updateTweenAction(float value, std::string_view key)
{
	if (!_bgl)
		return;

	if (key == "col1")
	{
		_bgl->_colorChannels.at(_targetColorId)._color.r = value;
	}
	else if (key == "col2")
	{
		_bgl->_colorChannels.at(_targetColorId)._color.g = value;
	}
	else if (key == "col3")
	{
		_bgl->_colorChannels.at(_targetColorId)._color.b = value;
	}
	else if (key == "pul1")
	{
		_bgl->_colorChannels.at(_targetGroupId)._color.r = value;
	}
	else if (key == "pul2")
	{
		_bgl->_colorChannels.at(_targetGroupId)._color.g = value;
	}
	else if (key == "pul3")
	{
		_bgl->_colorChannels.at(_targetGroupId)._color.b = value;
	}
	else if (key == "fade")
	{
		_bgl->_groups[_targetGroupId]._alpha = value;
	}
}

EffectGameObject* EffectGameObject::create(std::string_view frame)
{
	auto pRet = new (std::nothrow) EffectGameObject();

	if (pRet && pRet->init(frame))
	{
		pRet->_bgl = BaseGameLayer::getInstance();
		pRet->setVisible(false);
		pRet->autorelease();
		return pRet;
	}
	AX_SAFE_DELETE(pRet);
	return nullptr;
}