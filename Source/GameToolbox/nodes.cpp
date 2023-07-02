/*
    OpenGD - Open source Geometry Dash.
    Copyright (C) 2023  OpenGD Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "nodes.h"

#include "2d/Transition.h"
#include "2d/Sprite.h"
#include "2d/Label.h"
#include "2d/Menu.h"
#include "AltDirector.h"

#include "getTextureString.h"
#include "log.h"

USING_NS_AX;


void GameToolbox::limitLabelWidth(ax::Label* label, float width, float normalScale, float minScale) {
	float val = width / label->getContentSize().width;
	label->setScale(val > normalScale ? normalScale : val < minScale ? minScale : val);
}

void GameToolbox::popSceneWithTransition(float duration, popTransition type) {
	auto dir = AltDirector::getInstance();

	if (dir->getRunningScene() != nullptr) {
		dir->popScene();

		unsigned int c = dir->getScenesStack().size();
		if (c == 0) dir->end();
		else {
			dir->setSendCleanupToScene(true);
			auto scene = dir->getScenesStack().back();

			switch (type) {
			case popTransition::kTransitionShop:
				dir->replaceScene(TransitionMoveInB::create(duration, scene)); // i will change this transition later / att: Sai
				break;
			default:
				dir->replaceScene(TransitionFade::create(duration, scene));
				break;
			}
		}
	}
}

// no more "GameToolbox::getTextureString" in Label::createWithBMFont
Label* GameToolbox::createBMFont(std::string text, std::string font, int width, TextHAlignment alignment) {
	return Label::createWithBMFont(GameToolbox::getTextureString(font), text, alignment, width);
}
Label* GameToolbox::createBMFont(std::string text, std::string font, int width) {
	return createBMFont(text, font, width, TextHAlignment::LEFT);
}
Label* GameToolbox::createBMFont(std::string text, std::string font) {
	return createBMFont(text, font, 0, TextHAlignment::LEFT);
}


void GameToolbox::createBG(ax::Node* self, const ax::Color3B color) {
	auto winSize = Director::getInstance()->getWinSize();
	auto bg = Sprite::create("GJ_gradientBG.png");
	bg->setStretchEnabled(false);
	bg->setScaleX(winSize.width / bg->getContentSize().width);
	bg->setScaleY(winSize.height / bg->getContentSize().height);
	bg->setAnchorPoint({ 0, 0 });
	bg->setColor(color);
	self->addChild(bg);
}

void GameToolbox::createBG(ax::Node* self) {
	return createBG(self, { 0, 102, 255 });
}



void GameToolbox::createCorners(ax::Node* self, bool topLeft, bool topRight, bool botLeft, bool botRight) {
	Sprite* corner = nullptr;
	auto winSize = Director::getInstance()->getWinSize();
	if (botLeft) {
		corner = Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
		corner->setStretchEnabled(false);
		corner->setPosition({ 0, 0 });
		corner->setAnchorPoint({ 0.0, 0.0 });
		self->addChild(corner);
	}

	if (topLeft) {
		corner = Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
		corner->setStretchEnabled(false);
		corner->setPosition({ 0, winSize.height });
		corner->setAnchorPoint({ 0, 1 });
		corner->setFlippedY(true);
		self->addChild(corner);
	}

	if (topRight) {
		corner = Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
		corner->setStretchEnabled(false);
		corner->setPosition(winSize);
		corner->setAnchorPoint({ 1, 1 });
		corner->setFlippedX(true);
		corner->setFlippedY(true);
		self->addChild(corner);
	}

	if (botRight) {
		corner = Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
		corner->setStretchEnabled(false);
		corner->setPosition({ winSize.width, 0 });
		corner->setAnchorPoint({ 1, 0 });
		corner->setFlippedX(true);
		self->addChild(corner);
	}
}



void GameToolbox::alignItemsInColumnsWithPadding(ax::Menu* menu,
												 const int rows,
												 const int x_padding,
												 const int y_padding) {
	// Get the menu items and calculate the total number of items
	const auto items	 = menu->getChildren();
	const int totalItems = static_cast<int>(items.size());

	// Calculate the number of rows and the maximum number of items per row
	const int columns	 = (totalItems + rows - 1) / rows;
	const int itemsPerRow = (totalItems + rows - 1) / rows;

	// Calculate the starting position for the menu items
	const float startX = -(columns - 1) * (x_padding / 2.0f);
	const float startY = (rows - 1) * (y_padding / 2.0f);

	// Loop through each row and position the items in it
	for (int row = 0; row < rows; row++) {
		// Calculate the y position for the items in this row
		const float yPos = startY - (row * y_padding);

		// Loop through each item in this row and position it
		for (int column = 0; column < columns; column++) {
			// Calculate the index of the item in the menu items array
			const int index = (row * columns) + column;
			GameToolbox::log("index: {}", index);

			// Check if this index is valid for the menu items array
			if (index < totalItems) {
				// Get the item and set its position
				auto item		= dynamic_cast<ax::MenuItem*>(items.at(index));
				const float xPos = startX + (column * x_padding);
				item->setPosition(ax::Vec2(xPos, yPos));
			} else {
				// We've reached the end of the items, so we break out of the loop
				break;
			}
		}
	}
}

void GameToolbox::alignItemsHorizontally(ax::Vector<Node*> children, float padding, Point location)
{
	auto totalWidth = -padding;

	if (children.size())
	{
		for (Node* child : children)
		{
			totalWidth += padding + (child->getContentSize().width * child->getScaleX());
		}

		float xPos = -(float)(totalWidth * 0.5);
		for (Node* child : children)
		{
			auto width = child->getContentSize().width;
			auto scale = child->getScaleX();

			child->setPosition({ xPos + (width * scale) * 0.5f, 0 });
			child->setPosition(child->getPosition() + location);

			xPos = xPos + (float)(padding + (float)(width * child->getScaleX()));
		}
	}
}

void GameToolbox::alignItemsHorizontallyWithPadding(ax::Vector<ax::Node*> _children, float padding) {
	float width = -padding;
	for (const auto& child : _children)
		width += child->getContentSize().width * child->getScaleX() + padding;

	float x = -width / 2.0f;

	for (const auto& child : _children) {
		child->setPosition(x + child->getContentSize().width * child->getScaleX() / 2.0f, 0);
		x += child->getContentSize().width * child->getScaleX() + padding;
	}
}

void GameToolbox::alignItemsVerticallyWithPadding(ax::Vector<ax::Node*> _children, float padding) {
	float height = -padding;

	for (const auto& child : _children)
		height += child->getContentSize().height * child->getScaleY() + padding;

	float y = height / 2.0f;

	for (const auto& child : _children) {
		child->setPosition(0, y - child->getContentSize().height * child->getScaleY() / 2.0f);
		y -= child->getContentSize().height * child->getScaleY() + padding;
	}
}

int GameToolbox::getHighestChildZ(ax::Node* node)
{
	const auto& children = node->getChildren();
	int highestZ = 0;
	for(const auto& child : children)
	{
		if(int z = child->getLocalZOrder(); z > highestZ) {
			highestZ = z;
		}
	}
	return highestZ;
}
