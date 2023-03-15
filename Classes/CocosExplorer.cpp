#include "CocosExplorer.h"

#include "ImGui/ImGuiPresenter.h"
#include "ImGui/imgui/imgui.h"
#include <sstream>
#include <queue>
#include <mutex>

USING_NS_AX;
USING_NS_AX_EXT;

static bool operator!=(const Size &a, const Size &b) { return a.width != b.width || a.height != b.height; }

static Node *selected_node = nullptr;
static bool reached_selected_node = false;
static Node *hovered_node = nullptr;

static std::queue<std::function<void()>> threadFunctions;
static std::mutex threadFunctionsMutex;

static const char *getNodeName(Node *node)
{
	const char *name = typeid(*node).name() + 6;
	return name;
}

static void drawProperties()
{

	if (selected_node == nullptr)
	{
		ImGui::Text("Select a node to edit its properties :-)");
		return;
	}

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
	rgbaNode->setColor({static_cast<GLubyte>(_color[0] * 255),
						static_cast<GLubyte>(_color[1] * 255),
						static_cast<GLubyte>(_color[2] * 255)});
	rgbaNode->setOpacity(static_cast<int>(_color[3]) * 255);

	if (dynamic_cast<LabelProtocol *>(selected_node) != nullptr)
	{
		auto labelNode = dynamic_cast<LabelProtocol *>(selected_node);
		auto labelStr = labelNode->getString();
		char text[256];
		auto clabel = std::string(labelStr).c_str();
		strcpy(text, clabel);
		ImGui::InputText("Text", text, 256);
		if (strcmp(text, clabel))
		{
			threadFunctionsMutex.lock();
			threadFunctions.push([labelNode, text]()
								 { labelNode->setString(text); });
			threadFunctionsMutex.unlock();
		}
	}
}

static void generateTree(Node *node, unsigned int i = 0)
{
	std::stringstream stream;
	stream << "[" << i << "] " << getNodeName(node);
	if (node->getTag() != -1)
		stream << " (" << node->getTag() << ")";
	const auto childrenCount = node->getChildrenCount();
	if (childrenCount)
		stream << " {" << childrenCount << "}";

	auto flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
	if (selected_node == node)
	{
		flags |= ImGuiTreeNodeFlags_Selected;
		reached_selected_node = true;
	}
	if (node->getChildrenCount() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	const bool is_open = ImGui::TreeNodeEx(node, flags, stream.str().c_str());

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

	if (is_open)
	{
		auto children = node->getChildren();
		for (unsigned int i = 0; i < children_count; ++i)
		{
			auto child = children.at(i);
			generateTree(static_cast<ax::Node *>(child), i);
		}
		ImGui::TreePop();
	}
}

static void draw()
{
	extern bool _showDebugImgui;
	if(!_showDebugImgui) return;
	
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
}

void CocosExplorer::openForever()
{
	static bool openForever = false;
	if (openForever)
		return;

	openForever = true;
	auto e = Director::getInstance()->getEventDispatcher();
	e->addCustomEventListener(Director::EVENT_AFTER_SET_NEXT_SCENE, [&](EventCustom *)
							  { CocosExplorer::open(); });
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
