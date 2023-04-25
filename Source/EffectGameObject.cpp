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

	if (id == 29)
		_targetColorId = 1000;
	else if (id == 30)
		_targetColorId = 1001;

	if (!_bgl->_colorChannels.contains(_targetColorId))
	{
		_bgl->_colorChannels.insert({_targetColorId, SpriteColor(Color3B::WHITE, 255, 0)});
		_bgl->_originalColors.insert({_targetColorId, SpriteColor(Color3B::WHITE, 255, 0)});
	}

	switch (id)
	{
	case 30:
	case 29:
	case 899: {
		_bgl->_colorChannels.at(_targetColorId)._copyingColorID = -1;
		if (_copiedColorId > -1)
		{
			if (!_bgl->_colorChannels.contains(_copiedColorId))
			{
				_bgl->_colorChannels.insert({_copiedColorId, SpriteColor(Color3B::WHITE, 255, 0)});
				_bgl->_originalColors.insert({_copiedColorId, SpriteColor(Color3B::WHITE, 255, 0)});
			}
			_bgl->_colorChannels.at(_targetColorId)._copyingColorID = _copiedColorId;
			_color = _bgl->_colorChannels.at(_copiedColorId)._color;

			auto hsv = ax::HSV(_color);
			hsv.h += _hsv.h;
			if (_saturationTicked)
				hsv.s += _hsv.s;
			else
				hsv.s *= _hsv.s;
			if (_brightnessTicked)
				hsv.v += _hsv.v;
			else
				hsv.v *= _hsv.v;

			if (hsv.s > 1)
				hsv.s = 1;
			if (hsv.v > 1)
				hsv.v = 1;
			if (hsv.s < 0)
				hsv.s = 0;
			if (hsv.v < 0)
				hsv.v = 0;

			_color = GameToolbox::hsvToRgb(hsv);
		}
		this->runAction(
			ActionTween::create(this->_duration, "col1", _bgl->_colorChannels.at(_targetColorId)._color.r, _color.r));
		this->runAction(
			ActionTween::create(this->_duration, "col2", _bgl->_colorChannels.at(_targetColorId)._color.g, _color.g));
		this->runAction(
			ActionTween::create(this->_duration, "col3", _bgl->_colorChannels.at(_targetColorId)._color.b, _color.b));

		if (_opacity * 255.f != _bgl->_colorChannels.at(_targetColorId)._opacity)
			this->runAction(ActionTween::create(this->_duration, "col4",
												_bgl->_colorChannels.at(_targetColorId)._opacity, _opacity * 255.f));
		_bgl->_colorChannels.at(_targetColorId)._blending = _blending;
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
	case 1006: {
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
			{
				if (!_bgl->_colorChannels.contains(_copiedColorId))
				{
					_bgl->_colorChannels.insert({_copiedColorId, SpriteColor(Color3B::WHITE, 255, 0)});
					_bgl->_originalColors.insert({_copiedColorId, SpriteColor(Color3B::WHITE, 255, 0)});
				}
				target = _bgl->_colorChannels.at(_copiedColorId)._color;
			}

			auto hsv = ax::HSV(target);
			hsv.h += _hsv.h;
			if (_saturationTicked)
				hsv.s += _hsv.s;
			else
				hsv.s *= _hsv.s;
			if (_brightnessTicked)
				hsv.v += _hsv.v;
			else
				hsv.v *= _hsv.v;

			if (hsv.s > 1)
				hsv.s = 1;
			if (hsv.v > 1)
				hsv.v = 1;
			if (hsv.s < 0)
				hsv.s = 0;
			if (hsv.v < 0)
				hsv.v = 0;

			target = GameToolbox::hsvToRgb(hsv);
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
	case 1049:
		if (_activateGroup)
		{
			for (size_t i = 0; i < _bgl->_groups[_targetGroupId]._objects.size(); i++)
			{
				_bgl->_groups[_targetGroupId]._objects[i]->_toggledOn = true;
			}
		}
		else
		{
			for (size_t i = 0; i < _bgl->_groups[_targetGroupId]._objects.size(); i++)
			{
				_bgl->_groups[_targetGroupId]._objects[i]->_toggledOn = false;
				_bgl->_groups[_targetGroupId]._objects[i]->removeFromGameLayer();
			}
		}

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
	else if (key == "col4")
	{
		_bgl->_colorChannels.at(_targetColorId)._opacity = value;
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
		// pRet->setVisible(false);
		pRet->autorelease();
		return pRet;
	}
	AX_SAFE_DELETE(pRet);
	return nullptr;
}