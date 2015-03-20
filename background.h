/* Background:
 * Node subclass to handle bk image layers
 * initialize Cocos app and handle resolution independence
 * © Zach Moore 2015
 * */

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include "cocos2d.h"
#include "dataman.h"
USING_NS_CC;

class bkLayer : public Node{
public:

	bool alternate;
	bkLayer(bool alt){alternate = alt;}
	virtual bool init()
	{
		if(!Node::init())
			return false;

		SpriteExt* bk;

		if(alternate)
		{
			bk = DataMan::instance().getSprite("woodBk2.jpg");
		}
		else
		{
			bk = DataMan::instance().getSprite("woodBk.jpg");
		}

		bk->setPosition(DataMan::instance().screenCenter());
		addChild(bk,0);

		return true;

	}

	static bkLayer* create(bool alt = false)
	{
		bkLayer* ret = new bkLayer(alt);
		if(ret && ret->init())
		{
			ret->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(ret);
			ret = nullptr;
		}

		return ret;
	}
};


#endif /* BACKGROUND_H_ */
