#include "InfoStop.h"
#include "SimpleAudioEngine.h"

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
}

void InfoStop::setupBoundary()
{
	boundary.shape = SHAPE::circle;
	boundary.active = true;
	boundary.r = 20;
}


void InfoStop::setupSprite()
{
	auto sprite = Sprite::createWithSpriteFrameName("tip.png");
	auto moveUp = EaseInOut::create(MoveBy::create(2, Vec2(0, 5.0f)), 2);
	auto moveBack = EaseInOut::create(MoveBy::create(2, Vec2(0, -5.0f)), 2);
	auto seq1 = Sequence::create(moveUp, moveBack, nullptr);
	sprite->runAction(RepeatForever::create(seq1));
	this->addChild(sprite);
}

void InfoStop::setupAudio()
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadEffect("sfx/bot.wav");
	audio->preloadEffect(("sfx/tips/" + name + ".mp3").c_str());
}

void InfoStop::update(bool hit)
{
	if (hit && !isMessagevisible)
	{
		auto audio = SimpleAudioEngine::getInstance();
		//set this var to true for the name of the Tip, save it to file
		auto ud = UserDefault::getInstance();
		ud->setBoolForKey(name.c_str(), true);
		ud->flush();
		isMessagevisible = true;
		audio->playEffect("sfx/bot.wav");
	}
	else if (!hit && isMessagevisible)
	{
		isMessagevisible = false;
	}
}

void InfoStop::playAudio()
{
	auto audio = SimpleAudioEngine::getInstance();
	audio->playEffect(("sfx/tips/" + name + ".mp3").c_str());	
}
