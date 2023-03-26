#pragma once

#include <axmol.h>

class SongObject : ax::Node
{
public:
	int _songID;

	static SongObject* create(int id);
	bool init(int id);
};
