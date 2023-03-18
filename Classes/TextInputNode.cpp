#include "TextInputNode.h"
#include <ui/CocosGUI.h>
#include <iostream>

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
		m_pTextField->attachWithIME();

		return true;
	}
	else
	{
		if (m_pTextField->getDetachWithIME())
			m_pTextField->detachWithIME();

		return false;
	}
};

bool TextInputNode::init(float width, float height, const char* placeholder, const char* font, int scale)
{
	if (!Layer::init()) return false;
	m_pAllowedChars = " abcdefghijkmlnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789";

	this->setContentSize({ width, height });
	
	m_pTextField = ui::TextField::create("", font, std::round(scale * 1.2));
	m_pTextField->setContentSize({ width, height });
	m_pTextField->setPositionY(height / 2);
	
	m_pPlaceholder = Label::createWithBMFont(font, placeholder);
	m_pPlaceholder->setPositionX(width / 2);
	m_pPlaceholder->setPositionY(height / 2);
	
	float newFontSize = m_pPlaceholder->getBMFontSize() * width / m_pPlaceholder->getContentSize().width;
	m_pPlaceholder->setBMFontSize(newFontSize);

	this->addChild(m_pTextField);
	this->addChild(m_pPlaceholder);

	auto touchListener = EventListenerTouchOneByOne::create();
	m_pTextField->addEventListener([&](Ref* re, ui::TextField::EventType event) {
		switch (event) {
		case ui::TextField::EventType::DETACH_WITH_IME:
			ax::log("DETACH_WITH_IME");

			if (m_pTextField->getString().empty())
				m_pPlaceholder->setVisible(true);

			return;
		case ui::TextField::EventType::ATTACH_WITH_IME:
			ax::log("ATTACH_WITH_IME");

			m_pPlaceholder->setVisible(false);

			return;
		default: // INSERT_TEXT / DELETE_BACKWARD

			if (m_pTextField->getString().empty())
			{
				m_pPlaceholder->setVisible(true);
				return;
			}
			else {
				m_pPlaceholder->setVisible(false);
			}

			// Filter (todo: find a better way to do this)
			std::string new_str("");

			for (char character : this->m_pTextField->getString())
			{
				bool is_allowed = false;

				for (char allowed_char : this->m_pAllowedChars)
					if (character == allowed_char)
						is_allowed = true;

				if (is_allowed)
					new_str += character;
			}

			m_pTextField->setString(new_str);

			// Scale down text
			float newScale = this->getContentSize().width / m_pTextField->getContentSize().width;

			if (newScale < 1.f)
				m_pTextField->setScale(newScale);
		}
	});

	touchListener->onTouchBegan = AX_CALLBACK_2(TextInputNode::onTouchBegan, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	return true;
}