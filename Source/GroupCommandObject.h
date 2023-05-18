#pragma once

#include "2d/CCActionEase.h"
#include "2d/CCActionTween.h"

class GroupCommandObject : public ax::Node, public ax::ActionTweenDelegate
{
  public:
	ax::Point _oldPos;
	ax::Point _currentOffset;
	ax::Point _newPos;
  ax::Point _unkPoint;
  ax::Point _finalPoint;

  int _groupID;
  int _actionID;

  float _elapsed, _duration, _delta1, _delta2;

  bool _followPlayerX, _followPlayerY;
  bool _actionDone, _actionDoneForNextLoop;

  ax::Action *_action1, *_action2;

    void runMoveCommand(float duration, ax::Point offsetPos, int easeType, float easeAmt);
    void step(float dt);
    virtual void updateTweenAction(float value, std::string_view key) override;

    static GroupCommandObject *create();
};