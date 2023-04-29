#pragma once
#include <functional>
#include <string_view>

#include "MenuItemSpriteExtra.h"

namespace ax 
{ 
	class Node; 
}


using CheckboxCallback = const std::function<void(ax::Node*, bool)>&;

class Checkbox : public MenuItemSpriteExtra {
public:
	Checkbox(std::string_view off, std::string_view on, bool toggled, CheckboxCallback callback);
	 bool init(std::string_view text);
	 void activate() override;
	
	std::string_view _offSpr;
	std::string_view _onSpr;
	bool _toggled;
	std::function<void(ax::Node*, bool)> _callback;
	
	static Checkbox* create(std::string_view text, std::string_view off, std::string_view on, bool toggled, CheckboxCallback callback);
	static Checkbox* create(std::string_view text, bool toggled, CheckboxCallback);
	static Checkbox* create(std::string_view text, CheckboxCallback);
};