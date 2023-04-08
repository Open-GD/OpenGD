#include "TextInputNode.h"
#include <ui/CocosGUI.h>
#include <iostream>
#include <GameToolbox.h>

USING_NS_AX;

bool TextInputNode::onTouchBegan(Touch* touch, Event* event)
{
	Director* director = Director::getInstance();
	auto glTouchPoint = touch->getLocation();

	auto nodeTouchPoint = this->convertTouchToNodeSpace(touch);

	auto anchorPoint = Point(0, 0);
	auto contentSize = this->getContentSize();
	auto boundingBox = Rect(
		-anchorPoint.x * contentSize.width,
		-anchorPoint.y * contentSize.height,
		contentSize.width,
		contentSize.height
	);

	bool touchInside = boundingBox.containsPoint(nodeTouchPoint);

	if (touchInside)
	{
		_pTextField->attachWithIME();

		return true;
	}
	else
	{
		if (_pTextField->getDetachWithIME())
			_pTextField->detachWithIME();

		return false;
	}
};

bool TextInputNode::init(float width, float height, const char* placeholder, const char* font, int scale)
{
	if (!Layer::init()) return false;
	_pAllowedChars = " abcdefghijkmlnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

	this->setContentSize({ width, height });
	
	_pTextField = ui::TextField::create("", font, std::round(scale * 1.2));
	_pTextField->setContentSize({ width, height });
	_pTextField->setPositionY(height / 2);
	
	_pPlaceholder = Label::createWithBMFont(font, placeholder);
	_pPlaceholder->setPositionX(width / 2);
	_pPlaceholder->setPositionY(height / 2);
	
	float newFontSize = _pPlaceholder->getBMFontSize() * width / _pPlaceholder->getContentSize().width;
	_pPlaceholder->setBMFontSize(newFontSize);

	this->addChild(_pTextField);
	this->addChild(_pPlaceholder);

	auto touchListener = EventListenerTouchOneByOne::create();
	_pTextField->addEventListener([&](Ref* re, ui::TextField::EventType event) {
		switch (event) {
		case ui::TextField::EventType::DETACH_WITH_IME:
			ax::log("DETACH_WITH_IME");

			if (_pTextField->getString().empty())
				_pPlaceholder->setVisible(true);

			return;
		case ui::TextField::EventType::ATTACH_WITH_IME:
			ax::log("ATTACH_WITH_IME");

			_pPlaceholder->setVisible(false);

			return;
		default: // INSERT_TEXT / DELETE_BACKWARD

			if (_pTextField->getString().empty())
			{
				_pPlaceholder->setVisible(true);
				return;
			}
			else {
				_pPlaceholder->setVisible(false);
			}

			// Filter (todo: find a better way to do this)
			std::string new_str("");

			for (char character : this->_pTextField->getString())
			{
				bool is_allowed = false;

				for (char allowed_char : this->_pAllowedChars)
					if (character == allowed_char)
						is_allowed = true;

				if (is_allowed)
					new_str += character;
			}

			_pTextField->setString(new_str);

			// Scale down text
			float newScale = this->getContentSize().width / _pTextField->getContentSize().width;

			if (newScale < 1.f)
				_pTextField->setScale(newScale);
		}
	});

	touchListener->onTouchBegan = AX_CALLBACK_2(TextInputNode::onTouchBegan, this);

#ifdef AX_PLATFORM_PC
	auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed = AX_CALLBACK_2(TextInputNode::onKeyPressed, this);
	listener->onKeyReleased = AX_CALLBACK_2(TextInputNode::onKeyReleased, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
#endif

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	return true;
}

void TextInputNode::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	// if (keyCode == EventKeyboard::KeyCode::KEY_CTRL) GameToolbox::_isCtrlPressed = true;
	// i don't think that ctrl flag should be placed in gametoolbox

	switch (keyCode)
	{
#ifdef AX_PLATFORM_PC
	case EventKeyboard::KeyCode::KEY_CTRL:
		_pCommandMode = true;
		break;
	case EventKeyboard::KeyCode::KEY_C:
		if (_pCommandMode) glfwSetClipboardString(static_cast<GLViewImpl*>(Director::getInstance()->getOpenGLView())->getWindow(), _pTextField->getString().data());
		_pCommandMode = false;
		break;
	case EventKeyboard::KeyCode::KEY_V:
		if (_pCommandMode) _pTextField->setString(fmt::format("{}{}", _pTextField->getString(), glfwGetClipboardString(static_cast<GLViewImpl*>(Director::getInstance()->getOpenGLView())->getWindow())));
		_pCommandMode = false;
		break;
#endif
	default:
		break;
	}
}
void TextInputNode::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_CTRL) _pCommandMode = false;
}