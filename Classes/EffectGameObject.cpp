#include "EffectGameObject.h"
#include "PlayLayer.h"
#include "GameToolbox.h"

USING_NS_AX;

PlayLayer *pl;

void EffectGameObject::triggerActivated(float idk)
{
    this->m_bHasBeenActivated = true;
    pl = PlayLayer::getInstance();

    if(!pl->m_pColorChannels.contains(m_nTargetColorId)) return;

    this->runAction(ActionTween::create(this->m_fDuration, "col1", pl->m_pColorChannels.at(m_nTargetColorId).r, m_cColor.r));
    this->runAction(ActionTween::create(this->m_fDuration, "col2", pl->m_pColorChannels.at(m_nTargetColorId).g, m_cColor.g));
    this->runAction(ActionTween::create(this->m_fDuration, "col3", pl->m_pColorChannels.at(m_nTargetColorId).b, m_cColor.b));
}

void EffectGameObject::updateTweenAction(float value, std::string_view key) 
{
    if(key == "col1")
    {
        pl->m_pColorChannels.at(m_nTargetColorId).r = value;
    }
    else if(key == "col2")
    {
        pl->m_pColorChannels.at(m_nTargetColorId).g = value;
    }
    else if(key == "col3")
    {
        pl->m_pColorChannels.at(m_nTargetColorId).b = value;
    }
}

EffectGameObject *EffectGameObject::create(std::string_view frame)
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