//
//  MoonScene.h
//  akua
//
//  Created by German Torres on 8/3/15.
//
//

#ifndef __akua__MoonScene__
#define __akua__MoonScene__

#include "GameMapScene.h"
#include "Meteor.h"
#include "LoadingScene.h"
#include "SimpleAudioEngine.h"

class Moon : public GameMap
{
public:
    static Scene* createScene();
    
    virtual bool init();
	void setupAudio();
	void setupBackGround();
	void setupParallaxImages();
	void setupMeteorites();
	void loadMap();
    
    CREATE_FUNC(Moon);
private:
    void update(float dt);

	//meteors
	Meteor * met;
	Vector<Meteor*> meteorVector; 
};

#endif /* defined(__akua__MoonScene__) */
