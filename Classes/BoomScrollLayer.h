#pragma once

#include <axmol.h>

class BoomScrollLayer : public ax::Layer, public ax::ActionTweenDelegate {
private:
	std::vector<ax::Layer*> _layers;
	virtual void updateTweenAction(float value, std::string_view key) override {};
	ax::Layer* _internalLayer;

public:

	int _currentPage;
	int _totalPages;

	bool init(std::vector<ax::Layer*>, int);
	static BoomScrollLayer* create(std::vector<ax::Layer*> layers, int currentPage);
	void selectPage(int current);
	void changePageRight();
	void changePageLeft();
	bool onTouchBegan(ax::Touch *touch, ax::Event *event);
	void onTouchEnded(ax::Touch *touch, ax::Event *event);
	void onTouchMoved(ax::Touch *touch, ax::Event *event);
	void onExit() override;
};