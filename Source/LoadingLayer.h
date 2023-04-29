#pragma once
#include "2d/CCLayer.h"

class SimpleProgressBar;

namespace ax 
{ 
	class Scene; 
	class SpriteFrameCache;
	class TextureCache;
}


class LoadingLayer : public ax::Layer {
public:
	const char* getSplash();
	static ax::Scene* scene();
	static LoadingLayer* create();
	bool init();
	void loadAssets();
	void assetLoaded(ax::Ref*);
	SimpleProgressBar* _pBar;
	
	float m_nAssetsLoaded = 0;
	float m_nTotalAssets = 0;
	
	//avoid always calling getInstance for these
	ax::SpriteFrameCache* _sprFrameCache;
	ax::TextureCache* _textureCache;

};
