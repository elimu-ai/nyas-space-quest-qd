#include "InfoStop.h"
#include "SimpleAudioEngine.h"
#include "Planet.h"

USING_NS_CC;
using namespace CocosDenshion;

InfoStop * InfoStop::create()
{
	InfoStop * tip = new InfoStop();

	if (tip && tip->init())
	{
		tip->autorelease();
		tip->initTip();
		return tip;
	}
	CC_SAFE_DELETE(tip);
	return NULL;
}

InfoStop::~InfoStop()
{
//	popDownSeq->release();
//	popUpSeq->release();
}

void InfoStop::initTip()
{
	setupBoundary();
	setupSprite();
	setupAudio();
	
    consumed = false;	
    this->setScale(0.9);
	BaseObject::initObject();
    numberLeft = RandomHelper::random_int(1, 10);
    numberRight = RandomHelper::random_int(1, 10);
    while (numberLeft == numberRight)
    {
        numberRight = RandomHelper::random_int(1, 10);
    }
}

void InfoStop::setupBoundary()
{
	boundary.shape = SHAPE::circle;
	boundary.active = true;
	boundary.r = 20;
}


void InfoStop::setupSprite()
{
    //stop post
	auto sprite = Sprite::createWithSpriteFrameName("tip.png");
	auto moveUp = EaseInOut::create(MoveBy::create(2, Vec2(0, 5.0f)), 2);
	auto moveBack = EaseInOut::create(MoveBy::create(2, Vec2(0, -5.0f)), 2);
	auto seq1 = Sequence::create(moveUp, moveBack, nullptr);
	sprite->runAction(RepeatForever::create(seq1));
	this->addChild(sprite);
    //stop background
    bgSprite = Sprite::createWithSpriteFrameName("tipBg.png");
    bgSprite->setPosition(0, 200);
    bgSprite->setScale(0.0f, 0.0f);
    sprite->addChild(bgSprite);
}

void InfoStop::setupAudio()
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadEffect("sfx/bot.wav");
}

void InfoStop::update(bool hit)
{
	if (hit && !isMessagevisible && !consumed)
	{
		auto audio = SimpleAudioEngine::getInstance();
		audio->playEffect("sfx/bot.wav");
        
        Vector<FiniteTimeAction *>  allActions;
        
        auto popUp = ScaleTo::create(0.3f, 1);
        allActions.pushBack(popUp);
        
        int i=0;
        while (i < numberLeft)
        {
            auto planet = Planet::create(1);
            planet->setScale(0.5f);
            planet->setPosition(Vec2(100, 100));
            bgSprite->addChild(planet);
            auto move = MoveBy::create(0.5f, Vec2(0,15*i));
            auto targetedAction = TargetedAction::create(planet, move);
            allActions.pushBack(targetedAction);
            i++;
        }
        
        i=0;
        while (i < numberRight)
        {
            auto planet = Planet::create(1);
            planet->setScale(0.5f);
            planet->setPosition(Vec2(300, 90 + 10));
            bgSprite->addChild(planet);
            auto move = MoveBy::create(0.5f, Vec2(0,15*i));
            auto targetedAction = TargetedAction::create(planet, move);
            allActions.pushBack(targetedAction);
            i++;
        }
        
        
        auto * delay = DelayTime::create(1.5);
        allActions.pushBack(delay);
        
        auto consumeNumberDisplay = CallFunc::create([this]() {
            consumed = true;
        });
        allActions.pushBack(consumeNumberDisplay);
        
        auto seq = Sequence::create(allActions);
        bgSprite->stopAllActions();
        bgSprite->runAction(seq);
        isMessagevisible = true;
	}
	else if (!hit && isMessagevisible)
	{
        bgSprite->stopAllActions();
        auto popDown = ScaleTo::create(0.2f, 0);
        bgSprite->runAction(popDown);
		isMessagevisible = false;
	}
    if (hit && !isMessagevisible && consumed)
    {
        auto audio = SimpleAudioEngine::getInstance();
        audio->playEffect("sfx/bot.wav");
        
        auto popUp = ScaleTo::create(0.3f, 1);
        bgSprite->stopAllActions();
        bgSprite->runAction(popUp);
        isMessagevisible = true;
    }
}

void InfoStop::playAudio()
{
	auto audio = SimpleAudioEngine::getInstance();
}
