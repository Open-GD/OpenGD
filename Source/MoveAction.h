#pragma once

#include "2d/CCActionInterval.h"

struct GroupProperties;
namespace ax 
{ 
	class Node; 
	class Vec2;
}

class MoveAction : public ax::MoveBy
{
  private:
	GroupProperties* _group;
  std::vector<ax::Vec2> _prevPositions, _startPositions;
  ax::Vec2 _posDelta;

  public:
	virtual void update(float time) override;
  virtual void stop() override;
  virtual void startWithTarget(ax::Node* target) override;
  bool initWithDuration(float duration, const ax::Vec2& deltaPosition, GroupProperties* group);
	static MoveAction* create(float duration, const ax::Vec2& deltaPosition, GroupProperties* group);
};