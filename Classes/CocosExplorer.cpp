#include "CocosExplorer.h"

#include "ImGui/ImGuiPresenter.h"
#include "ImGui/imgui/imgui.h"
#include <sstream>
#include <queue>
#include <mutex>

USING_NS_AX;
USING_NS_AX_EXT;



static bool operator!=(const Size& a, const Size& b) { return a.width != b.width || a.height != b.height; }

static Node* selected_node = nullptr;
static bool reached_selected_node = false;
static Node* hovered_node = nullptr;

static std::queue<std::function<void()>> threadFunctions;
static std::mutex threadFunctionsMutex;


static const char* getNodeName(Node* node) {
    const char* name = typeid(*node).name() + 6;
    return name;
}

static void generateTree(Node* node, unsigned int i = 0) {
    std::stringstream stream;
    stream << "[" << i << "] " << getNodeName(node);
    if (node->getTag() != -1)
        stream << " (" << node->getTag() << ")";
    const auto childrenCount = node->getChildrenCount();
    if (childrenCount)
        stream << " {" << childrenCount << "}";
    if (ImGui::TreeNode(node, "%s", stream.str().c_str())) {
        if (ImGui::TreeNode(node + 1, "Attributes")) {
            if (ImGui::Button("Delete")) {
                node->removeFromParentAndCleanup(true);
                ImGui::TreePop();
                ImGui::TreePop();
                return;
            }
            ImGui::SameLine();
            if (ImGui::Button("Add Child")) {
                ImGui::OpenPopup("Add Child");
            }

            if (ImGui::BeginPopupModal("Add Child")) {
                static int item = 0;
                ImGui::Combo("Node", &item, "Node\0LabelBMFont\0LabelTTF\0Sprite\0MenuItemSpriteExtra\0");

                static int tag = -1;
                ImGui::InputInt("Tag", &tag);

                static char text[256];
                static char labelFont[256] = "bigFont.fnt";
                if (item == 1) {
                    ImGui::InputText("Text", text, 256);
                    ImGui::InputText("Font", labelFont, 256);
                }
                static int fontSize = 20;
                if (item == 2) {
                    ImGui::InputText("Text", text, 256);
                    ImGui::InputInt("Font Size", &fontSize);
                }
                static bool frame = false;
                if (item == 3 || item == 4) {
                    ImGui::InputText("Texture", text, 256);
                    ImGui::Checkbox("Frame", &frame);
                }

                ImGui::Separator();
                ImGui::SameLine();
                if (ImGui::Button("Cancel")) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
            ImGui::Text("Addr: 0x%p", node);
            if (node->getUserData()) {
                ImGui::SameLine();
                ImGui::Text("User data: 0x%p", node->getUserData());
            }

           float _scale[3] = { node->getScale(), node->getScaleX(), node->getScaleY() };
            ImGui::DragFloat3("Scale", _scale, 0.025f);
            // amazing
            if (node->getScale() != _scale[0])
                node->setScale(_scale[0]);
            else {
                node->setScaleX(_scale[1]);
                node->setScaleY(_scale[2]);
            }
            
            auto pos = node->getPosition();
            float _pos[2] = { pos.x, pos.y };
            ImGui::DragFloat2("Position", _pos);
            node->setPosition({ _pos[0], _pos[1] });



            int zOrder = node->getLocalZOrder();
            ImGui::InputInt("Z", &zOrder);
            if (node->getLocalZOrder() != zOrder)
                node->setLocalZOrder(zOrder);
            
            auto visible = node->isVisible();
            ImGui::Checkbox("Visible", &visible);
            if (visible != node->isVisible())
                node->setVisible(visible);


                auto rgbaNode = node;
                auto color = rgbaNode->getColor();
                float _color[4] = { color.r / 255.f, color.g / 255.f, color.b / 255.f, rgbaNode->getOpacity() / 255.f };
                ImGui::ColorEdit4("Color", _color);
                rgbaNode->setColor({
                    static_cast<GLubyte>(_color[0] * 255),
                    static_cast<GLubyte>(_color[1] * 255),
                    static_cast<GLubyte>(_color[2] * 255)
                });
                rgbaNode->setOpacity(static_cast<int>(_color[3]) * 255);
            
            if (dynamic_cast<LabelProtocol*>(node) != nullptr) {
                auto labelNode = dynamic_cast<LabelProtocol*>(node);
                auto labelStr = labelNode->getString();
                char text[256];
                auto clabel = std::string(labelStr).c_str();
                strcpy(text, clabel);
                ImGui::InputText("Text", text, 256);
                if (strcmp(text, clabel)) {
                    threadFunctionsMutex.lock();
                    threadFunctions.push([labelNode, text]() {
                        labelNode->setString(text);
                    });
                    threadFunctionsMutex.unlock();
                }
            }

            ImGui::TreePop();
        }
        
        auto children = node->getChildren();
        for (unsigned int i = 0; i < node->getChildrenCount(); ++i) {
            auto child = children.at(i);
            generateTree(dynamic_cast<Node*>(child), i);
        }
        ImGui::TreePop();
    }
}


static void draw() {
    auto current = Director::getInstance()->getRunningScene();
    ImGui::Begin("Cocos Explorer");
    generateTree(current);
    ImGui::End();
}

void CocosExplorer::openForever() {
    static bool openForever = false;
    if(openForever) return;
    
    openForever = true;
    auto e = Director::getInstance()->getEventDispatcher();
    e->addCustomEventListener(Director::EVENT_AFTER_SET_NEXT_SCENE, [&](EventCustom*){
        CocosExplorer::open();
    });
}

void CocosExplorer::open() {
    auto current = Director::getInstance()->getRunningScene();
    ImGuiPresenter::getInstance()->addRenderLoop("#cocosExplorer", draw, current);
}



void CocosExplorer::close() {
    ImGuiPresenter::getInstance()->removeRenderLoop("#cocosExplorer");
}
