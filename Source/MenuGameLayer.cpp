#include "MenuGameLayer.h"
#include "GameToolbox.h"

USING_NS_AX;

Scene* MenuGameLayer::scene() {
	auto scene = Scene::create();
	scene->addChild(MenuGameLayer::create());
	return scene;
}

bool MenuGameLayer::init(){
	if (!Layer::init()) return false;

	this->startPos = Vec2(0, 105);

	auto dir = Director::getInstance();
	auto winSize = dir->getWinSize();

	groundLayer = GroundLayer::create(1);
	
	addChild(groundLayer, 2);
	
	//should work but doesnt?
	auto playerTest = PlayerObject::create(GameToolbox::randomInt(1,14), this);
	playerTest->setMainColor(GameToolbox::randomColor3B());
	playerTest->setSecondaryColor(GameToolbox::randomColor3B());
	playerTest->setPosition({-100, 93.f});
	addChild(playerTest, 16);
	this->player = playerTest;

	const Texture2D::TexParams texParams = {
		backend::SamplerFilter::LINEAR, 
		backend::SamplerFilter::LINEAR, 
		backend::SamplerAddressMode::REPEAT, 
		backend::SamplerAddressMode::REPEAT
	};

	bgSprites = Menu::create();

	// this->bgSpr = bg;
	// this->bgSpr->getTexture()->setTexParameters(texParams);
	// this->bgSpr->setTextureRect(Rect(0, 0, 2048, 1024));
	// this->bgSpr->setPosition(winSize / 2);

	for(int i = 0; i < 4; i++) {
		auto gr = Sprite::create(GameToolbox::getTextureString("game_bg_01_001.png"));
		gr->setStretchEnabled(false);
		gr->getTexture()->setTexParameters(texParams);
		gr->setTextureRect(Rect(0, 0, 2048, 1024));
		gr->setPosition(winSize / 2);
		gr->setColor({ 0, 102, 255 });
		// bg scale
		gr->setScale(1.185f); // epic hardcore (please fix lmao)
		bsizeX = gr->getContentSize().width;

		this->bgSprites->addChild(gr);
	}
	this->bgSprites->alignItemsHorizontallyWithPadding(0);
	this->bgStartPos = bgSprites->getPositionX();
	this->addChild(bgSprites, -3);
	sep = 0.3f;
	bgSprites->setScale(winSize.width / bgSprites->getContentSize().width);
	bgSprites->setPositionY(0);

	//this is not how it works lol
	// bgSprites->setColor(GameToolbox::randomColor3B());

	// // this->addChild(this->bgSpr, -1);
	
	// bgSprites->runAction(
	// 	 RepeatForever::create(
	// 		 Sequence::create(
	// 			 TintTo::create(4.0f, {255, 0, 0}),
	// 			 TintTo::create(4.0f, {255, 255, 0}),
	// 			 TintTo::create(4.0f, {0, 255, 0}),
	// 			 TintTo::create(4.0f, {0, 255, 255}),
	// 			 TintTo::create(4.0f, {0, 0, 255}),
	// 			 TintTo::create(4.0f, {255, 0, 255}),
	// 			 TintTo::create(4.0f, {255, 0, 0}),
	// 			 nullptr
	// 		 )
	// 	 )
	// );
	
	scheduleUpdate();
	
	return true;
}
void MenuGameLayer::processPlayerMovement(float delta) {
	float step = std::min(2.0f, delta * 60.0f);
	auto winSize = Director::getInstance()->getWinSize();
	step /= 4.0f;

	this->player->setPositionX(this->player->getPositionX() + (step * 18.f));

	if(this->player->getPositionX() >= winSize.width + player->getContentSize().width) {
		player->setPosition({-300, 93.f});
		auto mainColor = GameToolbox::randomColor3B();
		auto secondaryColor = GameToolbox::randomColor3B();
		player->setMainColor(mainColor);
		player->setSecondaryColor(secondaryColor);
	}
}
void MenuGameLayer::processBackground(float delta) {
	if(this->bgStartPos - bgSprites->getPositionX() < this->bsizeX) 
	{
		bgSprites->setPositionX(bgSprites->getPositionX() - this->sep);
	}
	else 
	{
		bgSprites->setPositionX(bgSprites->getPositionX() + this->bsizeX);
	}
}

void MenuGameLayer::update(float delta) {
	processBackground(delta);
	processPlayerMovement(delta);
	groundLayer->update(delta * 60.0f);
}
