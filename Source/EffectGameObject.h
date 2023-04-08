#pragma once
#include "GameObject.h"
#include <axmol.h>

class PlayLayer;

class EffectGameObject : public GameObject, public ax::ActionTweenDelegate
{
private:
	PlayLayer* _pl;

public:
	float _duration;
	ax::Color3B _color;
	float _opacity;
	int _targetColorId = 1;
	bool _wasTriggerActivated;

private:
	virtual void updateTweenAction(float value, std::string_view key) override;

public:
	void triggerActivated(float idk);
	static EffectGameObject *create(std::string_view frame);
};