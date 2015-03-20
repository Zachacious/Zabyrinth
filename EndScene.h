/*End Game Scene:
 * Handle level success or failure with scene
 * © Zach Moore 2015
 * */

#ifndef ENDSCENE_H_
#define ENDSCENE_H_

#include "cocos2d.h"
#include "blackBk.h"

USING_NS_CC;

class EndLayer: public Node
{
public:
	EndLayer(bool didFail);
	virtual ~EndLayer();
	static EndLayer* create(bool didFail);
	bool init();
	void exec();
	bool failed;
	int level;
	Label* levCleared;
	BlackBk* bk;
	int menuSpacing;
	float btnOffset;
	//bool fadeOutDone;
	//btns
	MenuItemImage* retryBtn;
	void retry(Ref* pSender);
	MenuItemImage* regenBtn;
	void regen(Ref* pSender);
	MenuItemImage* titleBtn;
	void backTitle(Ref* pSender);
	MenuItemImage* playNextBtn;
	void playNext(Ref* pSender);
	MenuItemImage* diffBtn;
	void changeDiff(Ref* pSender);
	static int EndSceneNode;

	Color3B diffNormCol;
	Color3B diffSelCol;
	//menu
	Menu* menu;
};


#endif /* ENDSCENE_H_ */
