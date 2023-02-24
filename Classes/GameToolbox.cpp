#include "GameToolbox.h"
#include <fmt/format.h>
#include <cstdlib>


//general purpose class for helper functions that can be useful anywhere

int GameToolbox::randomInt(int min, int max) {
	
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

ax::Color3B GameToolbox::randomColor3B() {
	
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
void GameToolbox::alignItemsInColumnsWithPadding(ax::Menu* menu, const int rows, const int x_padding, const int y_padding)
{
    // Get the menu items and calculate the total number of items
    const auto items = menu->getChildren();
    const int totalItems = static_cast<int>(items.size());

    // Calculate the number of columns and the maximum number of items per column
    const int columns = (totalItems + rows - 1) / rows;
    const int itemsPerColumn = (totalItems + columns - 1) / columns;

    // Calculate the starting position for the menu items
    const float startX = -(columns - 1) * (x_padding / 2.0f);
    const float startY = -(rows - 1) * (y_padding / 2.0f);

    // Loop through each column and position the items in it
    for (int column = 0; column < columns; column++)
    {
        // Calculate the x position for the items in this column
        const float xPos = startX + (column * x_padding);

        // Loop through each item in this column and position it
        for (int row = 0; row < rows; row++)
        {
            // Calculate the index of the item in the menu items array
            const int index = (row * columns) + column;

            // Check if this index is valid for the menu items array
            if (index < totalItems)
            {
                // Get the item and set its position
                auto item = dynamic_cast<ax::MenuItem*>(items.at(index));
                item->setPosition(ax::Vec2(xPos, startY + (row * y_padding)));
            }
            else
            {
                // We've reached the end of the items, so we break out of the loop
                break;
            }
        }
    }
}

void GameToolbox::log(const char* simpleStr) {
	fmt::print("[{:%H:%M:%S}] {}\n", fmt::gmtime(std::time(NULL)), simpleStr);
}