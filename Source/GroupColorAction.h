#pragma once

#include "2d/ActionInterval.h"
#include "Types.h"
#include "BaseGameLayer.h"

class GroupColorAction : public ax::ActionInterval
{
  private:
	ax::Color3B _from, _to;
	GroupProperties* _target;

	float _deltar, _deltag, _deltab;

	bool _resetGroupState = false;

  public:
	static GroupColorAction* create(float duration, GroupProperties* target, ax::Color3B from, ax::Color3B to, bool resetGroupState);

	void startWithTarget(ax::Node* target) override;
	void update(float dt) override;

	bool initWithDuration(float duration, GroupProperties* target, ax::Color3B from, ax::Color3B to, bool resetGroupState);
};