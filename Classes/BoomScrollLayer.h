#pragma once

#include <axmol.h>

class BoomScrollLayer : public ax::Layer {
private:
	std::vector<ax::Layer*> _layers;
	int _currentPage;
	int _totalPages;
	ax::Layer* _internalLayer;

public:
	bool init(std::vector<ax::Layer*>, int);
	static BoomScrollLayer* create(std::vector<ax::Layer*> layers, int currentPage);
	void selectPage(int current);
	bool onTouchBegan(ax::Touch *touch, ax::Event *event);
	void onTouchEnded(ax::Touch *touch, ax::Event *event);
	void onTouchMoved(ax::Touch *touch, ax::Event *event);
	void onExit() override;
};