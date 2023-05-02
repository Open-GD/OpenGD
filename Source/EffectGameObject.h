#pragma once
#include "GameObject.h"
#include <axmol.h>

class BaseGameLayer;

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

	bool _scheduledRemoval;

private:
	virtual void updateTweenAction(float value, std::string_view key) override;
	ax::Action* actionEasing(ax::ActionInterval* ac);

public:
	void triggerActivated(float idk);
	static EffectGameObject *create(std::string_view frame);
	void update(float dt) override;
};