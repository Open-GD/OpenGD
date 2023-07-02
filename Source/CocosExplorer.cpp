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

#include "CocosExplorer.h"
#include "BaseGameLayer.h"
#include "GameObject.h"
#include "ImGui/ImGuiPresenter.h"
#include "ImGui/imgui/imgui.h"
#include "NodeSerializer.h"
#include "math/Mat4.h"

#include <fmt/format.h>
#include <mutex>
#include <queue>

#include <2d/Sprite.h>

USING_NS_AX;
USING_NS_AX_EXT;

static bool operator!=(const Size& a, const Size& b)
{
	return a.width != b.width || a.height != b.height;
}

static Node* selected_node = nullptr;
static bool reached_selected_node = false;
static Node* hovered_node = nullptr;

static std::queue<std::function<void()>> threadFunctions;
static std::mutex threadFunctionsMutex;

static const char* getNodeName(Node* node)
{
	const char* name = typeid(*node).name() + 6;
	return name;
}

ImVec2 cocos_to_vec2(const ax::Point& a)
{
	const auto size = ImGui::GetMainViewport()->Size;
	const auto win_size = ax::Director::getInstance()->getWinSize();
	return {a.x / win_size.width * size.x, (1.f - a.y / win_size.height) * size.y};
}

void highlight(ax::Node* node, bool selected)
{
	ax::Point bb_min(node->getBoundingBox().getMinX(), node->getBoundingBox().getMinY());
	ax::Point bb_max(node->getBoundingBox().getMaxX(), node->getBoundingBox().getMaxY());
	auto& foreground = *ImGui::GetForegroundDrawList();
	auto parent = node->getParent();

	auto camera_parent = node;
	while (camera_parent)
	{
		auto camera = Camera::getDefaultCamera();

		const ax::Mat4& viewMatrix = camera->getViewMatrix();
		const ax::Point offset(viewMatrix.m[3], viewMatrix.m[7]);
		bb_min -= offset;
		bb_max -= offset;

		camera_parent = camera_parent->getParent();
	}

	auto min = cocos_to_vec2(parent ? parent->convertToWorldSpace(bb_min) : bb_min);
	auto max = cocos_to_vec2(parent ? parent->convertToWorldSpace(bb_max) : bb_max);

	foreground.AddRectFilled(min, max, selected ? IM_COL32(200, 200, 255, 60) : IM_COL32(255, 255, 255, 70));
}

void drawProperties()
{

	if (selected_node == nullptr)
	{
		ImGui::Text("Select a node to edit its properties :-)");
		return;
	}

	if (ImGui::Button("Copy JSON"))
	{
		NodeSerializer::copyNodeJsonToClipboard(selected_node);
	}
	ImGui::SameLine();

	if (ImGui::Button("Delete"))
	{
		selected_node->removeFromParentAndCleanup(true);
		ImGui::TreePop();
		ImGui::TreePop();
		return;
	}
	ImGui::SameLine();
	if (ImGui::Button("Add Child"))
	{
		ImGui::OpenPopup("Add Child");
	}

	if (ImGui::BeginPopupModal("Add Child"))
	{
		static int item = 0;
		ImGui::Combo("Node", &item, "Node\0LabelBMFont\0LabelTTF\0Sprite\0MenuItemSpriteExtra\0");

		static int tag = -1;
		ImGui::InputInt("Tag", &tag);

		static char text[256];
		static char labelFont[256] = "bigFont.fnt";
		if (item == 1)
		{
			ImGui::InputText("Text", text, 256);
			ImGui::InputText("Font", labelFont, 256);
		}
		static int fontSize = 20;
		if (item == 2)
		{
			ImGui::InputText("Text", text, 256);
			ImGui::InputInt("Font Size", &fontSize);
		}
		static bool frame = false;
		if (item == 3 || item == 4)
		{
			ImGui::InputText("Texture", text, 256);
			ImGui::Checkbox("Frame", &frame);
		}

		ImGui::Separator();
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::Text("Addr: 0x%p", selected_node);
	if (selected_node->getUserData())
	{
		ImGui::SameLine();
		ImGui::Text("User data: 0x%p", selected_node->getUserData());
	}

	float _scale[3] = {selected_node->getScale(), selected_node->getScaleX(), selected_node->getScaleY()};
	ImGui::DragFloat3("Scale", _scale, 0.025f);
	// amazing
	if (selected_node->getScale() != _scale[0])
		selected_node->setScale(_scale[0]);
	else
	{
		selected_node->setScaleX(_scale[1]);
		selected_node->setScaleY(_scale[2]);
	}

	auto pos = selected_node->getPosition();
	float _pos[2] = {pos.x, pos.y};
	ImGui::DragFloat2("Position", _pos);
	selected_node->setPosition({_pos[0], _pos[1]});

	auto contect = selected_node->getContentSize();
	float _cont[2] = {contect.x, contect.y};
	ImGui::DragFloat2("Content Size", _cont);
	selected_node->setContentSize({_cont[0], _cont[1]});

	auto anchor = selected_node->getAnchorPoint();
	float _anch[2] = {anchor.x, anchor.y};
	ImGui::DragFloat2("Anchor Point", _anch);
	selected_node->setAnchorPoint({_anch[0], _anch[1]});

	float rotation[3] = {selected_node->getRotation(), selected_node->getRotationSkewX(),
						 selected_node->getRotationSkewY()};
	if (ImGui::DragFloat3("Rotation", rotation, 1.0f))
	{
		if (selected_node->getRotation() != rotation[0])
			selected_node->setRotation(rotation[0]);
		else
		{
			selected_node->setRotationSkewX(rotation[1]);
			selected_node->setRotationSkewY(rotation[2]);
		}
	}

	int zOrder = selected_node->getLocalZOrder();
	ImGui::InputInt("Z", &zOrder);
	if (selected_node->getLocalZOrder() != zOrder)
		selected_node->setLocalZOrder(zOrder);

	auto visible = selected_node->isVisible();
	ImGui::Checkbox("Visible", &visible);
	if (visible != selected_node->isVisible())
		selected_node->setVisible(visible);

	auto rgbaNode = selected_node;
	auto color = rgbaNode->getColor();
	float _color[4] = {color.r / 255.f, color.g / 255.f, color.b / 255.f, rgbaNode->getOpacity() / 255.f};
	ImGui::ColorEdit4("Color", _color);
	rgbaNode->setColor({static_cast<GLubyte>(_color[0] * 255), static_cast<GLubyte>(_color[1] * 255),
						static_cast<GLubyte>(_color[2] * 255)});
	rgbaNode->setOpacity(static_cast<int>(_color[3]) * 255);

	if (dynamic_cast<LabelProtocol*>(selected_node) != nullptr)
	{
		auto labelNode = dynamic_cast<LabelProtocol*>(selected_node);
		auto labelStr = labelNode->getString();
		auto labelStr2 = std::string(labelStr);
		char text[256];
		auto clabel = labelStr2.c_str();
		strcpy(text, clabel);
		ImGui::InputText("Text", text, 256);
		if (strcmp(text, clabel))
		{
			threadFunctionsMutex.lock();
			threadFunctions.push([labelNode, text]() { labelNode->setString(text); });
			threadFunctionsMutex.unlock();
		}
	}

	if (dynamic_cast<ax::Sprite*>(selected_node) != nullptr)
	{
		auto gm = dynamic_cast<ax::Sprite*>(selected_node);
		bool flipx = gm->isFlippedX(), flipy = gm->isFlippedY();
		ImGui::Checkbox("FlipX", &flipx);
		ImGui::Checkbox("FlipY", &flipy);
	}


	if (dynamic_cast<GameObject*>(selected_node) != nullptr)
	{
		auto gm = dynamic_cast<GameObject*>(selected_node);

		ImGui::Text("Color channel 1: %d", gm->_mainColorChannel);
		ImGui::Text("Color channel 2: %d", gm->_secColorChannel);
		ImGui::Text("ID: %d", gm->getID());
		ImGui::Text("Z Layer: %d", gm->_zLayer);
		ImGui::Text("Editor Layer: %d", gm->_editorLayer);
		std::string groupText = "";
		float opacityMultiplier = 1.f;

		for (int i : gm->_groups)
		{
			groupText += fmt::format("{} ", i);
			opacityMultiplier *= BaseGameLayer::getInstance()->_groups[i]._alpha;
		}
		ImGui::Text(fmt::format("Groups: {}", groupText).c_str());
		ImGui::Text(fmt::format("Opacity Multiplier (groups): {}", opacityMultiplier).c_str());

		float hsv1[3], hsv2[3];
		bool check1, check2, check3, check4;
		hsv1[0] = gm->_mainHSV.h;
		hsv1[1] = gm->_mainHSV.s;
		hsv1[2] = gm->_mainHSV.v;
		check1 = gm->_mainHSV.sChecked;
		check2 = gm->_mainHSV.vChecked;
		hsv2[0] = gm->_secondaryHSV.h;
		hsv2[1] = gm->_secondaryHSV.s;
		hsv2[2] = gm->_secondaryHSV.v;
		check3 = gm->_secondaryHSV.sChecked;
		check4 = gm->_secondaryHSV.vChecked;

		ImGui::InputFloat3("HSV1", hsv1); 
		ImGui::SameLine(); 
		ImGui::Checkbox("Saturation Check##1", &check1);
		ImGui::SameLine();
		ImGui::Checkbox("Brightness Check##1", &check2);
		ImGui::InputFloat3("HSV2", hsv2);
		ImGui::Checkbox("Saturation Check##2", &check3);
		ImGui::SameLine();
		ImGui::Checkbox("Brightness Check##2", &check4);

		gm->_mainHSV.h = hsv1[0];
		gm->_mainHSV.s = hsv1[1];
		gm->_mainHSV.v = hsv1[2];
		gm->_mainHSV.sChecked = check1;
		gm->_mainHSV.vChecked = check2;
		gm->_secondaryHSV.h = hsv2[0];
		gm->_secondaryHSV.s = hsv2[1];
		gm->_secondaryHSV.v = hsv2[2];
		gm->_secondaryHSV.sChecked = check3;
		gm->_secondaryHSV.vChecked = check4;
	}
}

static void generateTree(Node* node, unsigned int i = 0)
{
	std::string str = fmt::format("[{}] {} : {}", i, getNodeName(node), node->getName());

	if (dynamic_cast<GameObject*>(node) != nullptr)
	{
		auto gm = dynamic_cast<GameObject*>(node);
		str += fmt::format(" id {} uid {}", gm->getID(), gm->_uniqueID);
	}

	if (node->getTag() != -1)
		str += fmt::format(" ({})", node->getTag());
	const auto childrenCount = node->getChildrenCount();
	if (childrenCount)
		str += fmt::format(" {{{}}}", childrenCount);

	auto flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
	if (selected_node == node)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
		reached_selected_node = true;
	}
	if (node->getChildrenCount() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	const bool is_open = ImGui::TreeNodeEx(node, flags, "%s", str.c_str());

	if (ImGui::IsItemClicked())
	{
		if (node == selected_node && ImGui::GetIO().KeyAlt)
		{
			selected_node = nullptr;
			reached_selected_node = false;
		}
		else
		{
			selected_node = node;
			reached_selected_node = true;
		}
	}

	const auto children_count = node->getChildrenCount();

	if (ImGui::IsItemHovered())
		hovered_node = node;

	if (is_open)
	{
		auto children = node->getChildren();
		for (unsigned int i = 0; i < children_count; ++i)
		{
			auto child = children.at(i);
			generateTree(static_cast<ax::Node*>(child), i);
		}
		ImGui::TreePop();
	}
}

void draw()
{
	extern bool _showDebugImgui;
	if (!_showDebugImgui)
		return;

	hovered_node = nullptr;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(300, 300));

	auto current = Director::getInstance()->getRunningScene();
	ImGui::Begin("Cocos Explorer");
	const auto avail = ImGui::GetContentRegionAvail();

	ImGui::BeginChild("explorer.tree", ImVec2(avail.x * 0.5f, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	reached_selected_node = false;
	generateTree(current);
	ImGui::EndChild();

	if (!reached_selected_node)
		selected_node = nullptr;

	ImGui::SameLine();

	ImGui::BeginChild("explorer.options");

	drawProperties();

	ImGui::EndChild();

	ImGui::End();
	ImGui::PopStyleVar();

	if (selected_node)
		highlight(selected_node, true);
	if (hovered_node)
		highlight(hovered_node, false);
}

void CocosExplorer::openForever()
{
	static bool openForever = false;
	if (openForever)
		return;

	openForever = true;
	auto e = Director::getInstance()->getEventDispatcher();
	e->addCustomEventListener(Director::EVENT_AFTER_SET_NEXT_SCENE, [&](EventCustom*) { CocosExplorer::open(); });
}

void CocosExplorer::open()
{
	auto current = Director::getInstance()->getRunningScene();
	ImGuiPresenter::getInstance()->addRenderLoop("#cocosExplorer", draw, current);
}

void CocosExplorer::close()
{
	ImGuiPresenter::getInstance()->removeRenderLoop("#cocosExplorer");
}
