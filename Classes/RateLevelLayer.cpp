#include "RateLevelLayer.h"

#include <MenuItemSpriteExtra.h>
#include "GameToolbox.h"
#include <ui/CocosGUI.h>

USING_NS_AX;

const Color3B SELECTED_COLOR = { 255, 255, 255 };
const Color3B UNSELECTED_COLOR = { 125, 125, 125 };

unsigned int RateLevelLayer::m_dSelectedDiff = 0;
TextButton* RateLevelLayer::m_pSubmitButton = nullptr;

RateLevelLayer* RateLevelLayer::create(int levelID)
{
	RateLevelLayer* pRet = new RateLevelLayer();

	if (pRet && pRet->init(levelID))
	{
		pRet->autorelease();
		return pRet;
	}

	AX_SAFE_DELETE(pRet);
	return nullptr;
}

void RateLevelLayer::selectRating(Node* btn)
{
	auto tag = btn->getTag();

	if (tag == m_dSelectedDiff)
		return;
	
	btn->setColor(SELECTED_COLOR);

	if (auto oldSelected = btn->getParent()->getChildByTag(m_dSelectedDiff))
		oldSelected->setColor(UNSELECTED_COLOR);
	else
	{
		m_pSubmitButton->setColor(SELECTED_COLOR);
		m_pSubmitButton->getLabel()->setColor(SELECTED_COLOR);
	}

	m_dSelectedDiff = tag;
}

bool RateLevelLayer::init(int)
{
	m_dSelectedDiff = 0;

	if (!PopupLayer::init()) return false;
	auto winSize = Director::getInstance()->getWinSize();

	auto bg = ui::Scale9Sprite::create("GJ_square01-hd.png");
	bg->setContentSize({ 360, 180 });
	bg->setPosition(winSize / 2);

	this->m_pMainLayer->addChild(bg);

	auto label = Label::createWithBMFont(GameToolbox::getTextureString("bigFont.fnt"), "Rate Difficulty");
	label->setPosition({ winSize.width / 2, (winSize.height / 2) + 70.0f });

	this->m_pMainLayer->addChild(label);

	auto menu = Menu::create();

	for (size_t i = 1; i < 6; i++)
	{
		auto textureName = StringUtils::format("difficulty_%02d_btn_001.png", i);
		auto diffSprite = Sprite::createWithSpriteFrameName(textureName);

		auto diffSpriteExtra = MenuItemSpriteExtra::create(diffSprite, RateLevelLayer::selectRating);
		diffSpriteExtra->setColor(UNSELECTED_COLOR);
		diffSpriteExtra->setTag(i);
		diffSpriteExtra->setScale(1.2f);

		menu->addChild(diffSpriteExtra);
	}

	menu->alignItemsHorizontallyWithPadding(38);
	menu->setPosition(winSize / 2);
	this->m_pMainLayer->addChild(menu);

	auto btnMenu = Menu::create();
	btnMenu->setPosition({ (winSize.width / 2), (winSize.height / 2) - 65 });
	this->m_pMainLayer->addChild(btnMenu);

	auto cancelBtn = TextButton::create("Cancel", [&](TextButton*){
		this->close();
	});
	btnMenu->addChild(cancelBtn);

	auto submitBtn = TextButton::create("Submit", [](TextButton*) {});

	submitBtn->setColor(UNSELECTED_COLOR);
	submitBtn->getLabel()->setColor(UNSELECTED_COLOR);

	btnMenu->addChild(submitBtn);
	m_pSubmitButton = submitBtn;

	btnMenu->alignItemsHorizontallyWithPadding(15);

	return true;
}
