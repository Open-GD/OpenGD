#pragma once
#include <string_view>

#include "GameObject.h"
#include "ccTypes.h"
#include "math/Vec2.h"

class BaseGameLayer;
namespace ax 
{ 
	class Action;
	class ActionInterval;
}

class EffectGameObject : public GameObject
{
private:
	BaseGameLayer* _bgl;

public:
	float _duration;
	ax::Color3B _color;
	ax::HSV _hsv = {0.f, 1.f, 1.f, 1.f};
	float _opacity;
	int _targetColorId = 1, _targetGroupId = -1;
	bool _wasTriggerActivated;
	int _copiedColorId = -1;
	bool _saturationTicked, _brightnessTicked;

	float _fadeIn, _hold, _fadeOut;
	int _pulseMode, _pulseType;
	bool _mainOnly, _detailOnly;
	bool _blending;
	bool _activateGroup = false;

	ax::Vec2 _offset;

	int _easing;
	float _easeRate;

	bool _lockToPlayerX, _lockToPlayerY;

private:
	virtual void updateTweenAction(float value, std::string_view key) override;

public:
	void triggerActivated(float);
	static EffectGameObject* create(std::string_view frame);
	static ax::Action* actionEasing(ax::ActionInterval* ac, int ease, float rate);
};