#pragma once
#include <axmol.h>
#include "SimpleProgressBar.h"

class LoadingLayer : public ax::Layer {
public:
	const char* getSplash();
	static ax::Scene* scene();
	bool init();
	void loadAssets();
	void assetLoaded(ax::Ref*);
	CREATE_FUNC(LoadingLayer);
	SimpleProgressBar* _pBar;
	
	float m_nAssetsLoaded = 0;
	float m_nTotalAssets = 0;

};
