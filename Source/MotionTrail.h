#pragma once
#include <axmol.h>
#include <map>
#include <string>

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