//
//  InfoStop.h
//  nya
//
//  Created by German Torres on 7/06/17.
//  object for displaying tips 
//

#ifndef __akua__InfoStop__
#define __akua__Tip__

#include "BaseObject.h"

class InfoStop : public BaseObject
{
public:
	static InfoStop * create();
	void update(bool hit);
	void playAudio();
   
	bool consumed;

private:
	~InfoStop();
	void initTip();
	void setupBoundary();
	void setupAudio();
	void setupSprite();
	
	
	std::string name;
	bool isMessagevisible;
	Sequence * popUpSeq;
	Sequence * popDownSeq;
};

#endif /* defined(__akua__BitacoraScene__) */