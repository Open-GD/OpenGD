#include "GameToolbox.h"
#include <fmt/format.h>
#include <fstream>

USING_NS_AX;

//general purpose class for helper functions that can be useful anywhere

int GameToolbox::randomInt(int min, int max)
{
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> distribution(min, max);
    
    return distribution(generator);
}
int GameToolbox::randomInt(int max) {
    return GameToolbox::randomInt(0, max);
}
float GameToolbox::randomFloat(int min, int max) {
    return static_cast<float>(GameToolbox::randomInt(min, max));
}
float GameToolbox::randomFloat(int max) {
   return static_cast<float>(GameToolbox::randomInt(max));
}

ax::Color3B GameToolbox::randomColor3B()
{
    uint8_t r = GameToolbox::randomInt(255);
    uint8_t g = GameToolbox::randomInt(255);
    uint8_t b = GameToolbox::randomInt(255);
    
    return {r, g, b};
}

void GameToolbox::createCorners(ax::Node* self, bool topLeft, bool topRight, bool botLeft, bool botRight)
{
    const std::array<bool, 4> corners { botLeft, topLeft, topRight, botRight };
  
    const auto winSize = Director::getInstance()->getWinSize();
    for(int i = 0; 4 > i; i++) {
        if(!corners[i]) continue;
        
        std::tuple<constexpr ax::Vec2, constexpr ax::Vec2, constexpr bool, constexpr bool> settings;
        
        switch(i) {
            case 0: settings = { {0,0}, {0,0}, false, false };                  break;
            case 1: settings = { {0, winSize.height}, {0, 1}, false, true };    break;
            case 2: settings = { {winSize}, {1, 1}, true, true };               break;
            case 3: settings = { {winSize.width, 0}, {1, 0}, true, false };     break;
            //more cases
        }
        
        const auto [pos, anchor, flipX, flipY] = settings;
        auto corner = ax::Sprite::createWithSpriteFrameName("GJ_sideArt_001.png");
        corner->setStretchEnabled(false);
        corner->setPosition(pos);
        corner->setAnchorPoint(anchor);
        corner->setFlippedX(flipX);
        corner->setFlippedY(flipY);
        self->addChild(corner);
    }
}
//if low or no "." found, return existing
//if high, add -uhd otherwise add -hd (medium default)
//take string by copy, reference or string_view is not possible because we actually modify the string

static inline std::string getTextureString_AppendResources(std::string texture) {
    std::string nTexture = "Resources/";
    nTexture += texture;
    bool low = false;
    bool medium = true;
    bool high = false;

    size_t pos = nTexture.find(".");
    if(!low && pos != std::string::npos) {
        nTexture.insert(pos, high ? "-uhd" : "-hd");
    }
    GameToolbox::log("texture: {}", nTexture);
    return nTexture;
}

static inline std::string getTextureString_WithoutResources(std::string texture)
{
    bool low = false;
    bool medium = true;
    bool high = false;

    size_t pos = texture.find(".");
    if(!low && pos != std::string::npos) {
        texture.insert(pos, high ? "-uhd" : "-hd");
    }
    GameToolbox::log("texture: {}", texture);
    return texture;
}

std::string GameToolbox::getStringForMusicID(int id) {
    switch(id) {
        case 0: return "StereoMadness.mp3";
        case 1: return "BackOnTrack.mp3";
        case 2: return "Polargeist.mp3";
        case 3: return "DryOut.mp3";
        case 4: return "BaseAfterBase.mp3";
        case 5: return "CantLetGo.mp3";
        case 6: return "Jumper.mp3";
        default: return "StereoMadness.mp3";
    }
}

std::string GameToolbox::getTextureString(std::string texture)
{ 
    // // this code would try to access texture using six different ways. if last way fails, this function returns '-' for error handling

    // // attempt to create sprite without resources path (sprite frame name and without it)
    // ax::Sprite *spr = ax::Sprite::create(getTextureString_WithoutResources(texture));
    // if(!spr) {
    //     // not found, try with sprite frame name
    //     spr = ax::Sprite::createWithSpriteFrameName(getTextureString_WithoutResources(texture));
    //     if(!spr) {
    //         // not found, switch to resources path attempt
    //         goto withResourcesPath;
    //     } else {
    //         spr->release();
    //         return getTextureString_WithoutResources(texture);
    //     }
    // } else {
    //     return getTextureString_WithoutResources(texture);
    // }

    // withResourcesPath:
    // // attempt to create sprite with resources path (sprite frame name and without it)
    // spr = ax::Sprite::create(getTextureString_AppendResources(texture));
    // if(!spr) {
    //     // not found, try with sprite frame name
    //     spr = ax::Sprite::createWithSpriteFrameName(getTextureString_AppendResources(texture));
    //     if(!spr) {
    //         goto tryDifferentFormats;
    //     } else {
    //         spr->release();
    //         return getTextureString_AppendResources(texture);
    //     }
    // } else {
    //     spr->release();
    //     return getTextureString_AppendResources(texture);
    // }

    // tryDifferentFormats:
    // // every way was checked with no result, check if it plist file
    // if(texture.ends_with(".plist") || texture.ends_with(".fnt")) {
    //     #if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32) || (AX_TARGET_PLATFORM == AX_PLATFORM_LINUX)
    //         // try to find requested file
    //         std::string s1 = getTextureString_WithoutResources(texture);
    //         std::string s2 = getTextureString_AppendResources(texture);

    //         std::ifstream f1(s1);
    //         std::ifstream f2(s2);

    //         if(f1.good()) return s1;
    //         if(f2.good()) return s2;

    //         GameToolbox::log("File not found: -");
    //         return "-";
            
    //     #else
    //         return getTextureString_WithoutResources(texture);
    //     #endif
    // } else {
    //     // return '-' for error handling
    //     GameToolbox::log("Worst case: -");
    //     return "-";
    // }
    return getTextureString_WithoutResources(texture);
}


void GameToolbox::alignItemsInColumnsWithPadding(ax::Menu* menu, const int rows, const int x_padding, const int y_padding)
{
    // Get the menu items and calculate the total number of items
    const auto items = menu->getChildren();
    const int totalItems = static_cast<int>(items.size());

    // Calculate the number of rows and the maximum number of items per row
    const int columns = (totalItems + rows - 1) / rows;
    const int itemsPerRow = (totalItems + rows - 1) / rows;

    // Calculate the starting position for the menu items
    const float startX = -(columns - 1) * (x_padding / 2.0f);
    const float startY = (rows - 1) * (y_padding / 2.0f);

    // Loop through each row and position the items in it
    for (int row = 0; row < rows; row++)
    {
        // Calculate the y position for the items in this row
        const float yPos = startY - (row * y_padding);

        // Loop through each item in this row and position it
        for (int column = 0; column < columns; column++)
        {
            // Calculate the index of the item in the menu items array
            const int index = (row * columns) + column;
            GameToolbox::log("index: {}", index);

            // Check if this index is valid for the menu items array
            if (index < totalItems)
            {
                // Get the item and set its position
                auto item = dynamic_cast<ax::MenuItem*>(items.at(index));
                const float xPos = startX + (column * x_padding);
                item->setPosition(ax::Vec2(xPos, yPos));
            }
            else
            {
                // We've reached the end of the items, so we break out of the loop
                break;
            }
        }
    }
}

void GameToolbox::alignItemsHorizontallyWithPadding(ax::Vector<ax::Node*> _children, float padding)
{
    float width = -padding;
    for (const auto& child : _children)
        width += child->getContentSize().width * child->getScaleX() + padding;

    float x = -width / 2.0f;

    for (const auto& child : _children)
    {
        child->setPosition(x + child->getContentSize().width * child->getScaleX() / 2.0f, 0);
        x += child->getContentSize().width * child->getScaleX() + padding;
    }
}

void GameToolbox::alignItemsVerticallyWithPadding(ax::Vector<ax::Node*> _children, float padding)
{
    float height = -padding;

    for (const auto& child : _children)
        height += child->getContentSize().height * child->getScaleY() + padding;

    float y = height / 2.0f;

    for (const auto& child : _children)
    {
        child->setPosition(0, y - child->getContentSize().height * child->getScaleY() / 2.0f);
        y -= child->getContentSize().height * child->getScaleY() + padding;
    }
}