#include "GameToolbox.h"
#include <fmt/format.h>


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
//if low or no "." found, return existing
//if high, add -uhd otherwise add -hd (medium default)
//take string by copy, reference or string_view is not possible because we actually modify the string
std::string GameToolbox::getTextureString(std::string texture)
{
    std::string nTexture = "Resources/"; // Just leave it empty if this crap crashes the game
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