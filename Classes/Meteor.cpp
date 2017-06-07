//
//  Meteor.cpp
//  akua
//
//  Created by German Torres on 8/3/15.
//
//

#include "Meteor.h"


USING_NS_CC;
using namespace CocosDenshion;

Meteor * Meteor::create(Vec2 pos)
{
	Meteor * meteor = new Meteor();

	if (meteor && meteor->init())
	{
		meteor->autorelease();
		meteor->initMeteor(pos);
		return meteor;
	}
	CC_SAFE_DELETE(meteor);
	return NULL;
}

Meteor::~Meteor()
{
}

void Meteor::initMeteor(Vec2 pos)
{
	origin = pos;
	this->setPosition(pos);

	tail = ParticleMeteor::create();
	tail->setPosition(Vec2::ZERO);
	tail->setPositionType(ParticleSystem::PositionType::GROUPED);
	tail->setGravity(Vec2(0, 150));
	tail->setStartColor(Color4F(0.56, 0.21, 0.5, 1.0));
	tail->setPosVar(Vec2(30, 0));

	this->addChild(tail);


	meteorSprite = Sprite::createWithSpriteFrameName("meteor"+ std::to_string(RandomHelper::random_int(1, 3)) +".png");
	this->addChild(meteorSprite);

	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadEffect("sfx/meteor.wav");

	regen(0);
	
	isAvailable = true;
	setupBoundary();
	BaseObject::initObject();
}

void Meteor::setupBoundary()
{
	boundary.shape = SHAPE::circle;
	boundary.r = 80;
	boundary.active = true;
	boundary.offset = Vec2(0, -10);
}

void Meteor::update(Vec2 playerPosition, float dt)
{
	float x = this->getPositionX();
	float y = this->getPositionY();
	this->setPosition(Vec2(x - speedX*dt, y - speedY*dt));
	if (this->getPositionY() < -(300))
	{
		this->regen(playerPosition.x);
	}
}

void Meteor::regen(float playerX)
{
	this->setPosition(origin);
	this->setAnchorPoint(Vec2(0.5, 0.5));
	speed = (CCRANDOM_0_1() + 0.15) * 260.0f;
	angle = CCRANDOM_MINUS1_1() * 45.0f;

	speedX = speed*sin(CC_DEGREES_TO_RADIANS(angle));
	speedY = speed*cos(CC_DEGREES_TO_RADIANS(angle));

	this->setScale(1 / (float)RandomHelper::random_int(1, 5));

	this->setRotation(angle);
	playMeteorAudio(playerX);
}

void Meteor::playMeteorAudio(float playerX)
{
	if (speed > 230 && origin.x <= playerX + 600 && origin.x >= playerX - 600)
	{
		auto audio = SimpleAudioEngine::getInstance();
		audio->playEffect("sfx/meteor.wav");
	}
}