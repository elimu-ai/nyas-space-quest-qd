//
//  Meteor.h
//  akua
//
//  Created by German Torres on 8/3/15.
//
//

#ifndef __akua__Meteor__
#define __akua__Meteor__

#include "BaseObject.h"
#include "SimpleAudioEngine.h"

#include "androidFix.h"

class Meteor : public BaseObject
{
public:
	static Meteor * create(Vec2 pos);
    
    void regen(float playerX);
	void update(Vec2 playerPosition, float dt);

private:
	~Meteor();
	bool isAvailable;
	void initMeteor(Vec2 pos);
	void setupBoundary();
	void playMeteorAudio(float playerX);

	Sprite * meteorSprite;
	Sequence * moveSeq;
	ParticleMeteor * tail;

	float speed;
	float angle;
	float speedX;
	float speedY;
	Vec2 origin;
};

#endif /* defined(__akua__Meteoro__) */