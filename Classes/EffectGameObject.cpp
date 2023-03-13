#include "EffectGameObject.h"
#include "PlayLayer.h"
#include "GameToolbox.h"

USING_NS_AX;

void EffectGameObject::triggerActivated(float idk)
{
    auto pl = PlayLayer::getInstance();

    this->runAction(ActionTween::create(this->m_fDuration, "color", pl->m_pColorChannels.at(m_nTargetId).r, m_cColor.r));
    this->runAction(ActionTween::create(this->m_fDuration, "color", pl->m_pColorChannels.at(m_nTargetId).g, m_cColor.g));
    this->runAction(ActionTween::create(this->m_fDuration, "color", pl->m_pColorChannels.at(m_nTargetId).b, m_cColor.b));
    this->m_bHasBeenActivated = true;
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