#pragma once

#include <axmol.h>
#include "GJGameLevel.h"

class LevelPage : public ax::Layer
{
  private:
	GJGameLevel* _level;

  public:
	bool init(GJGameLevel* level);
	static LevelPage* create(GJGameLevel* level);
};