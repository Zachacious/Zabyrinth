/* Black Bk:
 * Convenience class to handle semi-transparent black background
 * © Zach Moore 2015
 * */

#ifndef BLACKBK_H_
#define BLACKBK_H_


#include "cocos2d.h"
#include "dataman.h"
USING_NS_CC;

class BlackBk : public Node{
public:

	virtual bool init()
	{
		if(!Node::init())
			return false;

		setCascadeOpacityEnabled(true);
		SpriteExt* bk = DataMan::instance().getSprite("black.png");
		bk->setPosition(DataMan::instance().screenCenter());
		bk->scaleToSize(DataMan::instance().screenSize());
		bk->setOpacity(200);
		addChild(bk,0);

		return true;

	}
	CREATE_FUNC(BlackBk);
};


#endif /* BLACKBK_H_ */
