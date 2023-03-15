#pragma once
#include <axmol.h>

class PopupLayer : public ax::LayerColor {
public:
	ax::Layer* m_pMainLayer;

	virtual void setup() {};
	
	bool init();
	void keyBackClicked();

	virtual void show();
	virtual void close();
};