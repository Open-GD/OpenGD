#pragma once
#include "GameObject.h"
#include <axmol.h>

class EffectGameObject : public GameObject
{
public:
    float m_fDuration;
    ax::Color3B m_cColor;
    float m_fOpacity;
    int m_nTargetId;

    void triggerActivated(float idk);

    static EffectGameObject *create(std::string_view frame);
};