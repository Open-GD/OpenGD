#pragma once
#include <axmol.h>

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