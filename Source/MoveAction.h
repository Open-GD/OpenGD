#pragma once

#include "BaseGameLayer.h"
#include <axmol.h>

class MoveAction : public ax::MoveBy
{
  private:
	GroupProperties* _group;

  public:
	virtual void update(float time) override;
  virtual void startWithTarget(ax::Node* target) override;
  bool initWithDuration(float duration, const ax::Vec2& deltaPosition, GroupProperties* group);
	static MoveAction* create(float duration, const ax::Vec2& deltaPosition, GroupProperties* group);
};