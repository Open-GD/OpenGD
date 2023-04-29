#pragma once

#include "2d/CCActionInterval.h"
#include "ccTypes.h"

class SpriteColor;

class ColorAction : public ax::ActionInterval
{
  private:
	ax::Color3B _from, _to;
	float _afrom, _ato;
	SpriteColor* _target;

	float _deltar, _deltag, _deltab, _deltaa;

  public:
	static ColorAction* create(float duration, SpriteColor* target, ax::Color3B from, ax::Color3B to, float afrom = -1, float ato = -1);

	void startWithTarget(ax::Node* target) override;
	void update(float dt) override;

	bool initWithDuration(float duration, SpriteColor* target, ax::Color3B from, ax::Color3B to, float afrom = -1, float ato = -1);
};