#pragma once

#include <axmol.h>
#include "GJSearchObject.h"
#include "network/HttpClient.h"
#include "ListLayer.h"
#include "ui/UIListView.h"

class LevelBrowserLayer : public ax::Layer {
public:
    static ax::Scene* scene(GJSearchObject* search);
    static LevelBrowserLayer* create(GJSearchObject* search);
    bool init(GJSearchObject* search);

    ax::ui::ListView* listView;

    void onHttpRequestCompleted(ax::network::HttpClient* sender, ax::network::HttpResponse* response);
};