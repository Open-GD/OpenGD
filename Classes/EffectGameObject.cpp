#include "EffectGameObject.h"
#include "GameToolbox.h"
#include "PlayLayer.h"

USING_NS_AX;

PlayLayer* pl;

void EffectGameObject::triggerActivated(float idk)
{
	this->m_bHasBeenActivated = true;
	pl = PlayLayer::getInstance();

	if(getID() == 29)
		m_nTargetColorId = 1000;
	else if (getID() == 30)
		m_nTargetColorId = 1001;

	switch (getID())
	{
	case 30:
	case 29:
	case 899:
		if (!pl->m_pColorChannels.contains(m_nTargetColorId))
			pl->m_pColorChannels.insert({ m_nTargetColorId, SpriteColor(Color3B::WHITE, 255, 0) });

		this->runAction(
			ActionTween::create(this->m_fDuration, "col1", pl->m_pColorChannels.at(m_nTargetColorId)._color.r, m_cColor.r));
		this->runAction(
			ActionTween::create(this->m_fDuration, "col2", pl->m_pColorChannels.at(m_nTargetColorId)._color.g, m_cColor.g));
		this->runAction(
			ActionTween::create(this->m_fDuration, "col3", pl->m_pColorChannels.at(m_nTargetColorId)._color.b, m_cColor.b));
		break;
	case 22:
		pl->_enterEffectID = 1;
		break;
	case 23:
		pl->_enterEffectID = 5;
		break;
	case 24:
		pl->_enterEffectID = 4;
		break;
	case 25:
		pl->_enterEffectID = 6;
		break;
	case 26:
		pl->_enterEffectID = 7;
		break;
	case 27:
		pl->_enterEffectID = 2;
		break;
	case 28:
		pl->_enterEffectID = 3;
		break;
	}
}

void EffectGameObject::updateTweenAction(float value, std::string_view key)
{
	if (key == "col1")
	{
		pl->m_pColorChannels.at(m_nTargetColorId)._color.r = value;
	}
	else if (key == "col2")
	{
		pl->m_pColorChannels.at(m_nTargetColorId)._color.g = value;
	}
	else if (key == "col3")
	{
		pl->m_pColorChannels.at(m_nTargetColorId)._color.b = value;
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