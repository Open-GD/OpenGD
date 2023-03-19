#include "GJMoreGamesLayer.h"
#include "GameToolbox.h"

bool GJMoreGamesLayer::init()
{
    if(!ax::Layer::init()) return false;

    m_pL = DropDownLayer::create("RobTop Games");
    if(!m_pL)
        return false;

    auto nogames = ax::Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Nothing here yet... sorry :(");
    auto winSize = ax::Director::getInstance()->getWinSize();
    nogames->setPosition({winSize.width / 2, winSize.height / 2});
    nogames->setScale(0.6f);

    m_pL->droplayer->addChild(nogames);
    m_pL->showLayer();

    addChild(reinterpret_cast<ax::Layer*>(m_pL));

    return true;
}