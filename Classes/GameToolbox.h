#pragma once
#include <axmol.h>
#include <string>
#include <fmt/chrono.h>

//general purpose class for helper functions that can be useful with any class at any moment
namespace GameToolbox
{

    enum GTRes {
        Low = 0,
        Medium,
        High
    };
    
    int randomInt(int min, int max);
    int randomInt(int max);
    float randomFloat(int min, int max);
    float randomFloat(int max);
    ax::Color3B randomColor3B();
    std::string getTextureString(std::string texture);
    void alignItemsInColumnsWithPadding(ax::Menu* menu, const int rows, const int x_padding, const int y_padding);
    void alignItemsVerticallyWithPadding(ax::Vector<ax::Node*> children, float padding);
    void alignItemsHorizontallyWithPadding(ax::Vector<ax::Node*> children, float padding);
    void createCorners(ax::Node* layer, bool topRight, bool topLeft, bool botLeft, bool botRight);
    inline void createAllCorners(ax::Node* self) { return createCorners(self, true, true, true, true); }
    
    inline float SquareDistance(float xa, float ya, float xb, float yb);
    float SquareDistance(ax::Vec2 a, ax::Vec2 b);

    inline float slerp(float a, float b, float ratio);
    inline float repeat(float a, float length);

    template <typename T>
    inline T inRange(T value, T min, T max) {
        return MIN(MAX(value, min), max);
    }
    
    template <typename... T>
        void log(fmt::format_string<T...> fmt, T&&... args) {
        return fmt::print("[{:%H:%M:%S}] {}\n", fmt::gmtime(std::time(NULL)), fmt::format(fmt, std::forward<T>(args)...));
    }
};
