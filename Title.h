/* Title Scene
 * © Zach Moore 2015
 * */

#ifndef TITLE_H_
#define TITLE_H_

#include "cocos2d.h"
#include "dataman.h"
#include "maze.h"

USING_NS_CC;

class Title: public Node {
public:
	Title();
	virtual ~Title();
	CREATE_FUNC(Title);
	virtual bool init();
	void startCallBack(Ref* pSender);
	void howTo(Ref* pSender);
	void intro();
private:
	DataMan * dataMan;
	SpriteExt* logo;
	SpriteExt* bkLogo;
	MenuItemImage* playBtn;
	MenuItemImage* howToBtn;
	Menu* menu;

};

class TitleScene: public Scene
{
public:
	TitleScene();
	void exec();
	CREATE_FUNC(TitleScene);
};

#endif /* TITLE_H_ */
