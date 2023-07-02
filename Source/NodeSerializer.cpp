/*************************************************************************
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
*************************************************************************/

#include "NodeSerializer.h"

#include <vector>
#include "external/json.hpp"

#include <2d/Node.h>
#include <2d/Sprite.h>
#include <platform/FileUtils.h>

using ordered_json = nlohmann::ordered_json;

USING_NS_AX;

static const char* getNodeName(const ax::Node* node)
{
	const char* name = typeid(*node).name() + 6;
	return name;
}

constexpr int indentation = 2;

void NodeSerializer::to_json(ordered_json& j, const ax::Node* p)
{
	j = ordered_json{
		{"type", getNodeName(p)},
		{"pos", {
			{"x", p->getPositionX()},
			{"y", p->getPositionY()}
		}},
		{"scale", {
			{"x", p->getScaleX()},
			{"y", p->getScaleY()}
		}},
		{"rot", {
			{"x", p->getRotationSkewX()},
			{"y", p->getRotationSkewY()}
		}},
		{"anchor", {
			{"x", p->getAnchorPoint().x},
			{"y", p->getAnchorPoint().y}
		}},
		{"contentSize", {
			{"width", p->getContentSize().width},
			{"height", p->getContentSize().height}
		}},
		{"color", {
			{"r", p->getColor().r},
			{"g", p->getColor().g},
			{"b", p->getColor().b}
		}},
		{"zOrder", p->getGlobalZOrder()},
		{"visible", p->isVisible()}
	};

	const auto& children = p->getChildren();
	if (!children.empty()) {
		ordered_json childrenJson;
		for (const auto& child : children) {
			ordered_json childJson;
			to_json(childJson, child);
			childrenJson.push_back(childJson);
		}
		j["children"] = childrenJson;
	}
}

//untested
void NodeSerializer::from_json(const ordered_json& j, ax::Node* p)
{
	p->setPosition({ j["pos"]["x"], j["pos"]["y"] });
	p->setScaleX(j["scale"]["x"]);
	p->setScaleY(j["scale"]["y"]);
	p->setRotationSkewX(j["rot"]["x"]);
	p->setRotationSkewY(j["rot"]["y"]);
	p->setAnchorPoint({ j["anchor"]["x"], j["anchor"]["y"] });
	p->setContentSize({ j["contentSize"]["width"], j["contentSize"]["height"] });
	p->setColor({ j["color"]["r"], j["color"]["g"], j["color"]["b"] });
	p->setGlobalZOrder(j["zOrder"]);
	p->setVisible(j["visible"]);

	const auto& childrenJson = j.find("children");
	if (childrenJson != j.end()) {
		const auto& childrenArray = *childrenJson;
		for (const auto& childJson : childrenArray) {
			ax::Node* child = nullptr;
			from_json(childJson, child);
			if (child) {
				p->addChild(child);
			}
		}
	}
}

void NodeSerializer::saveNodeToJsonFile(const std::string_view filename, ax::Node* rootNode)
{
	ordered_json j;
	NodeSerializer::to_json(j, rootNode);

	// Save the JSON object to a file
	ax::FileUtils::getInstance()->writeStringToFile(j.dump(indentation), filename);
}

#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32)
#include <windows.h>
#endif

#include <string_view>

static void CopyToClipboard(const std::string_view text) {
#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32)
	// Open the clipboard
	if (!OpenClipboard(nullptr)) {
		// Handle error if clipboard cannot be opened
		// You can add appropriate error handling here
		return;
	}

	// Empty the clipboard
	if (!EmptyClipboard()) {
		// Handle error if clipboard cannot be emptied
		// You can add appropriate error handling here
		CloseClipboard();
		return;
	}

	// Get the length of the text
	const size_t textLength = text.length();

	// Allocate global memory to hold the text
	HGLOBAL hClipboardData = GlobalAlloc(GMEM_DDESHARE, (textLength + 1) * sizeof(char));
	if (hClipboardData == nullptr) {
		// Handle error if memory allocation fails
		// You can add appropriate error handling here
		CloseClipboard();
		return;
	}

	// Lock the global memory handle and get a pointer to the memory
	char* pClipboardData = static_cast<char*>(GlobalLock(hClipboardData));
	if (pClipboardData == nullptr) {
		// Handle error if memory locking fails
		// You can add appropriate error handling here
		GlobalFree(hClipboardData);
		CloseClipboard();
		return;
	}

	// Copy the text to the global memory
	memcpy(pClipboardData, text.data(), textLength * sizeof(char));

	// Null-terminate the text
	pClipboardData[textLength] = '\0';

	// Unlock the global memory
	GlobalUnlock(hClipboardData);

	// Set the clipboard data
	if (!SetClipboardData(CF_TEXT, hClipboardData)) {
		// Handle error if clipboard data cannot be set
		// You can add appropriate error handling here
		GlobalFree(hClipboardData);
		CloseClipboard();
		return;
	}

	// Close the clipboard
	CloseClipboard();
#endif
}

void NodeSerializer::copyNodeJsonToClipboard(ax::Node* node)
{
	ordered_json j;
	NodeSerializer::to_json(j, node);
	CopyToClipboard(j.dump(indentation));
}