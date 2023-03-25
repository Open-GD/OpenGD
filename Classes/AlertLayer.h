#pragma once
#include "PopupLayer.h"
#include "TextButton.h"
class AlertLayer : public PopupLayer {
private:
	bool init(std::string title, std::string desc, std::string btn1, std::string btn2, float width, std::function<void(TextButton*)> btn1Callback, std::function<void(TextButton*)> btn2Callback);

	TextButton* _btn1;
	TextButton* _btn2;

public:
	static AlertLayer* create(std::string title, std::string desc, std::string btn1, std::string btn2, float width, std::function<void(TextButton*)> btn1Callback, std::function<void(TextButton*)> btn2Callback);
	static AlertLayer* create(std::string title, std::string desc, std::string btn1, std::string btn2, std::function<void(TextButton*)> btn1Callback, std::function<void(TextButton*)> btn2Callback);
	static AlertLayer* create(std::string title, std::string desc, std::string btn1, float width, std::function<void(TextButton*)> btn1Callback);
	static AlertLayer* create(std::string title, std::string desc, std::string btn1, std::function<void(TextButton*)> btn1Callback);
	static AlertLayer* create(std::string title, std::string desc, float width);
	static AlertLayer* create(std::string title, std::string desc);

	void setBtn1Callback(std::function<void(TextButton*)> callback);
	void setBtn2Callback(std::function<void(TextButton*)> callback);
};