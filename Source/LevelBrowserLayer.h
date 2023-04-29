#pragma once
#include <unordered_map>
#include <vector>

#include "2d/CCLayer.h"

class GJSearchObject;
class LoadingCircle;
class MenuItemSpriteExtra;
class GJGameLevel;

namespace ax 
{ 
    class Scene; 
    namespace ui
    {
        class ListView;
    }

    namespace network
    {
        class HttpClient;
        class HttpResponse;
    }
}



class LevelBrowserLayer : public ax::Layer {
public:
    static ax::Scene* scene(GJSearchObject* search);
    static LevelBrowserLayer* create(GJSearchObject* search);
    bool init(GJSearchObject* search);
    LoadingCircle* _loading;

    ax::ui::ListView* listView;
    GJSearchObject* _searchObj;
    MenuItemSpriteExtra* _leftBtn, *_rightBtn;

    std::unordered_map<int, std::vector<GJGameLevel*>> _cachedLevels;

    void fillList();
    void onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response);
};