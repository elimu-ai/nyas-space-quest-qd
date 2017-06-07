//
//  MoonScene.cpp
//  akua
//
//  Created by German Torres on 8/3/15.
//  Moon Map class
//

#include "MoonScene.h"

using namespace CocosDenshion;


Scene* Moon::createScene()
{
    auto scene = Scene::create();
    auto layer = Moon::create();
    
    layer->loadMap();
    
    scene->addChild(layer);
    return scene;
}

bool Moon::init()
{
    if (!GameMap::init())
        return false;
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("moon.plist");
    return true;
}

void Moon::loadMap()
{
	setupAudio();
	setupBackGround();
	setupParallaxImages();
	setupTilemap("moonLevel.tmx");
	setupMeteorites();
	GameMap::loadMap();
}

void Moon::setupAudio()
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->stopBackgroundMusic();
	audio->playBackgroundMusic("sfx/moonMusic.mp3", true);
}

void Moon::setupBackGround()
{
	auto bG = Sprite::createWithSpriteFrameName("bg.png");
	bG->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bG, -10);
	auto earthBG = Sprite::createWithSpriteFrameName("earth.png");
	earthBG->setPosition(Vec2(visibleSize.width/2, visibleSize.height - 450));
	this->addChild(earthBG, -9);
	auto starsFX = Sprite::createWithSpriteFrameName("stars.jpg");
	starsFX->setAnchorPoint(Vec2::ZERO);
	starsFX->setBlendFunc(BlendFunc::ADDITIVE);
	addChild(starsFX, -10);
	auto lander = Sprite::createWithSpriteFrameName("lunarLander.png");
	lander->setAnchorPoint(Vec2(0.5, 0));
	lander->setScale(0.8f);
	lander->setPosition(2500, 10);
	lander->setTag(66);
	parallaxNodeBack->addChild(lander,1);
}

void Moon::setupParallaxImages()
{
	for (int i = 1; i <= 3; ++i)
	{
		std::string name = "p" + std::to_string(i) + ".png";
		auto sprite = Sprite::createWithSpriteFrameName(name);
		sprite->setAnchorPoint(Vec2::ZERO);
		sprite->setPositionX((i - 1)*sprite->getContentSize().width);
		parallaxNodeBack->addChild(sprite);
	}

	for (int i = 3; i <= 4; ++i)
	{
		std::string name = "montana" + std::to_string(i) + ".png";
		auto mountainSprite = Sprite::createWithSpriteFrameName(name);
		mountainSprite->setAnchorPoint(Vec2(0, 0));
		mountainSprite->setPositionX(RandomHelper::random_int(-50, (int)visibleSize.width));
		if (i >= 3)//lol
		{
			parallaxNodeFront->addChild(mountainSprite);
		}
		else
		{
			//parallaxNodeBack->addChild(mountainSprite);
		}
	}
}

void Moon::setupMeteorites()
{
	auto meteors = tiledMap->getObjectGroup("meteors")->getObjects();
	for (auto meteor : meteors)
	{
		auto meteorMap = meteor.asValueMap();
		int x = meteorMap["x"].asInt();
		int y = meteorMap["y"].asInt();
		auto object = Meteor::create(Vec2(x, y));
		meteorVector.pushBack(object);
		gameplayNode->addChild(object);
	}
}

void Moon::update(float dt)
{
	//meteor
	for (Meteor * meteor : meteorVector)
	{
		meteor->update(player->getPosition(), dt);
		if (player->checkIntersect(meteor))
		{
			//meteor->regen(player->getPositionX());
			this->hurtPlayer();
		}
	}
    GameMap::update(dt);
}
