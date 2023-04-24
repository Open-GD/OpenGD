#pragma once
#include "PopupLayer.h"
#include "MenuItemSpriteExtra.h"

class AlertLayer : public PopupLayer {
private:
	bool init(std::string_view title, std::string_view desc, std::string_view btn1, std::string_view btn2, float width, std::function<void(Node*)> btn1Callback, std::function<void(Node*)> btn2Callback);

	MenuItemSpriteExtra* _btn1;
	MenuItemSpriteExtra* _btn2;

public:
	static AlertLayer* create(std::string_view title, std::string_view desc, std::string_view btn1, std::string_view btn2, float width, std::function<void(Node*)> btn1Callback, std::function<void(Node*)> btn2Callback);
	static AlertLayer* create(std::string_view title, std::string_view desc, std::string_view btn1, std::string_view btn2, std::function<void(Node*)> btn1Callback, std::function<void(Node*)> btn2Callback);
	static AlertLayer* create(std::string_view title, std::string_view desc, std::string_view btn1, float width, std::function<void(Node*)> btn1Callback);
	static AlertLayer* create(std::string_view title, std::string_view desc, std::string_view btn1, std::function<void(Node*)> btn1Callback);
	static AlertLayer* create(std::string_view title, std::string_view desc, float width);
	static AlertLayer* create(std::string_view title, std::string_view desc);

	void setBtn1Callback(std::function<void(Node*)> btn1Callback);
	void setBtn2Callback(std::function<void(Node*)> btn2Callback);
};