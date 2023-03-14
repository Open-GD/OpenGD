#pragma once
#include "GameObject.h"
#include <axmol.h>

class EffectGameObject : public GameObject, public ax::ActionTweenDelegate
{
private:
    virtual void updateTweenAction(float value, std::string_view key) override;
public:
    float m_fDuration;
    ax::Color3B m_cColor;
    float m_fOpacity;
    int m_nTargetColorId;

    void triggerActivated(float idk);
    static EffectGameObject *create(std::string_view frame);
};