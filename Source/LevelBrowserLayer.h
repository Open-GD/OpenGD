#pragma once

#include <axmol.h>
#include "GJSearchObject.h"
#include "network/HttpClient.h"
#include "ListLayer.h"
#include "ui/UIListView.h"
#include "GJGameLevel.h"
#include "LoadingCircle.h"

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