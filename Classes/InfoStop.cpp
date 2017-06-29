#include "InfoStop.h"
#include "SimpleAudioEngine.h"
#include "Planet.h"
#include "LanguageManager.h"

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
    langCode = "en";
    if (CCApplication::getInstance()->getCurrentLanguage() == LanguageType::SWAHILI)
        langCode = "sw";
    for (int i = 0; i <= 10; i++)
    {
        audio->preloadEffect(("sfx/" + langCode + "/digit_" + std::to_string(i) + ".wav").c_str());
    }
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
        int planetNumber = RandomHelper::random_int(0, 3);
        int i=0;
        while (i < numberLeft)
        {
            auto planet = Planet::create(planetNumber);
            planet->setScale(0.65f);
            planet->setPosition(Vec2(100, 260));
            bgSprite->addChild(planet);
            auto playAudio = CallFunc::create([this, i]() {
                
                auto audio = SimpleAudioEngine::getInstance();
                audio->playEffect(("sfx/" + langCode + "/digit_" + std::to_string(i + 1) + ".wav").c_str());
                
            });
            allActions.pushBack(playAudio);
            auto move = MoveBy::create(0.75f, Vec2(0,-(160 - 15*i)));
            auto targetedAction = TargetedAction::create(planet, move);
            allActions.pushBack(targetedAction);
            i++;
        }
        
        auto * delayNumbers = DelayTime::create(1.0f);
        allActions.pushBack(delayNumbers);
        
        i=0;
        while (i < numberRight)
        {
            auto planet = Planet::create(planetNumber);
            planet->setScale(0.65f);
            planet->setPosition(Vec2(300, 260));
            bgSprite->addChild(planet);
            auto playAudio = CallFunc::create([this, i]() {
                
                auto audio = SimpleAudioEngine::getInstance();
                audio->playEffect(("sfx/" + langCode + "/digit_" + std::to_string(i+1) + ".wav").c_str());
            });
            allActions.pushBack(playAudio);
            auto move = MoveBy::create(0.75f, Vec2(0,-(160 - 15*i)));
            auto targetedAction = TargetedAction::create(planet, move);
            allActions.pushBack(targetedAction);
            i++;
        }
        string greaterOrSmaller = "/is_greater_than.wav";
        if (RandomHelper::random_int(0,1))
            greaterOrSmaller = "/is_larger_than.wav";
        auto symbolLabel = Label::createWithTTF(">", LanguageManager::getString("font"), 164);
        if (numberRight > numberLeft)
        {
            symbolLabel->setString("<");
            greaterOrSmaller = "/is_smaller_than.wav";
            if (RandomHelper::random_int(0,1))
                greaterOrSmaller = "/is_less_than.wav";
        }
        symbolLabel->setAnchorPoint(Vec2(0.5,0.5));
        bgSprite->addChild(symbolLabel);
        symbolLabel->setPosition(Vec2(200,170));
        symbolLabel->setScale(0);
        auto labelScaleUp = ScaleTo::create(0.5,1);
        auto targetedLabelAction = TargetedAction::create(symbolLabel, labelScaleUp);
        allActions.pushBack(targetedLabelAction);
        
        auto * delay = DelayTime::create(1.2);
        allActions.pushBack(delay);
        
        auto playLeft = CallFunc::create([this](){
            auto audio = SimpleAudioEngine::getInstance();
            audio->playEffect(("sfx/" + langCode + "/digit_" + std::to_string(numberLeft) + ".wav").c_str());
        });

        auto playGreater = CallFunc::create([this, greaterOrSmaller](){
            auto audio = SimpleAudioEngine::getInstance();
            audio->playEffect(("sfx/" + langCode + greaterOrSmaller).c_str());
        });
        
        
        auto playRight = CallFunc::create([this](){
            auto audio = SimpleAudioEngine::getInstance();
            audio->playEffect(("sfx/" + langCode + "/digit_" + std::to_string(numberRight) + ".wav").c_str());
        });
        allActions.pushBack(playLeft);
        allActions.pushBack(delay);
        allActions.pushBack(playGreater);
        allActions.pushBack(delay);
        allActions.pushBack(playRight);
        
        
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
