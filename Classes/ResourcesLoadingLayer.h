#pragma once

#include <axmol.h>

class ResourcesLoadingLayer : public ax::Layer
{
public:
	bool checkPath(std::string path);
	void loadLoadingLayer();
	bool init();
	CREATE_FUNC(ResourcesLoadingLayer);
};