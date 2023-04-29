#pragma once

#include <string_view>

#include "2d/CCLayer.h"
#include "2d/CCActionTween.h"
#include "2d/CCAction.h"

namespace ax 
{ 
	class Sprite; 
}


class GroundLayer : public ax::Layer, public ax::ActionTweenDelegate 
{
private:
	virtual void updateTweenAction(float value, std::string_view key) override;
	
	float m_fOneGroundSize;

	AX_SYNTHESIZE(float, m_fSpeed, Speed)

public:
	ax::Sprite* _sprite;
	void update(float dt);
	bool init(int groundID);
	static GroundLayer* create(int groundID);
};