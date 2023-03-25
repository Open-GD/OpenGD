#include "EffectGameObject.h"
#include "GameToolbox.h"
#include "PlayLayer.h"

USING_NS_AX;


void EffectGameObject::triggerActivated(float idk)
{
	if (!_pl) return;

	this->_triggerActivated = true;

	auto id = getID();

	if(id == 29)
		_targetColorId = 1000;
	else if (id == 30)
		_targetColorId = 1001;

	switch (id)
	{
		case 30:
		case 29:
		case 899:
		{
			if (!_pl->m_pColorChannels.contains(_targetColorId))
				_pl->m_pColorChannels.insert({ _targetColorId, SpriteColor(Color3B::WHITE, 255, 0) });

			this->runAction(ActionTween::create(this->_duration, "col1", _pl->m_pColorChannels.at(_targetColorId)._color.r, _color.r));
			this->runAction(ActionTween::create(this->_duration, "col2", _pl->m_pColorChannels.at(_targetColorId)._color.g, _color.g));
			this->runAction(ActionTween::create(this->_duration, "col3", _pl->m_pColorChannels.at(_targetColorId)._color.b, _color.b));
			break;
		}
		case 22: _pl->_enterEffectID = 1; break;
		case 23: _pl->_enterEffectID = 5; break;
		case 24: _pl->_enterEffectID = 4; break;
		case 25: _pl->_enterEffectID = 6; break;
		case 26: _pl->_enterEffectID = 7; break;
		case 27: _pl->_enterEffectID = 2; break;
		case 28: _pl->_enterEffectID = 3; break;
	}
}

void EffectGameObject::updateTweenAction(float value, std::string_view key)
{
	if (!_pl) return;

	if (key == "col1") {
		_pl->m_pColorChannels.at(_targetColorId)._color.r = value;
	}
	else if (key == "col2") {
		_pl->m_pColorChannels.at(_targetColorId)._color.g = value;
	}
	else if (key == "col3") {
		_pl->m_pColorChannels.at(_targetColorId)._color.b = value;
	}
}

EffectGameObject* EffectGameObject::create(std::string_view frame)
{
	auto pRet = new (std::nothrow) EffectGameObject();

	if (pRet && pRet->init(frame))
	{
		pRet->autorelease();
		return pRet;
	}
	AX_SAFE_DELETE(pRet);
	return nullptr;
}