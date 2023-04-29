#pragma once

#include "2d/CCMotionStreak.h"

namespace ax 
{ 
	class Texture2D;
	struct Color3B; 
}


class MotionTrail : public ax::MotionStreak
{
public:
	MotionTrail();
	virtual ~MotionTrail();

	static MotionTrail* create(float timeToFade,
		float minSeg,
		float strokeWidth,
		const ax::Color3B& strokeColor,
		std::string_view imagePath);

	static MotionTrail* create(float timeToFade,
		float minSeg,
		float strokeWidth,
		const ax::Color3B& strokeColor,
		ax::Texture2D* texture);

	virtual void update(float delta) override;

	bool _appendNewPoints = true;
	void resumeStroke();
	void stopStroke();
};