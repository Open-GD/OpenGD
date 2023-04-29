#pragma once

#include "2d/CCNode.h"


class SongObject : ax::Node
{
public:
	int _songID;

	static SongObject* create(int id);
	bool init(int id);
};
