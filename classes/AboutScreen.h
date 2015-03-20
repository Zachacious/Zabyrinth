/*
 * AboutScreen.h
 *
 *  Created on: Sep 11, 2014
 *      Author: Zach
 */

#ifndef ABOUTSCREEN_H_
#define ABOUTSCREEN_H_

#include "cocos2d.h"
#include "blackbk.h"
#include "sound.h"
#include "DataMan.h"

USING_NS_CC;

class AboutScreen : public Node
{
public:
	BlackBk* bk;
	Label* heading;
	Label* content;
	MenuItemImage* backBtn;
	Menu* menu;
	std::function<void()> backFunc;
	EventListenerKeyboard* listenerKeyboard;


	AboutScreen():bk(nullptr),
			heading(nullptr), content(nullptr),
			backBtn(nullptr),backFunc(nullptr),
			menu(nullptr), listenerKeyboard(nullptr){}

	virtual ~AboutScreen()
	{
		if(bk)
			bk->release();

		if(heading)
			heading->release();

		if(content)
			content->release();

		if(backBtn)
			backBtn->release();

		if(menu)
			menu->release();

		if(listenerKeyboard)
		getEventDispatcher()->removeEventListener(listenerKeyboard);
	}

	CREATE_FUNC(AboutScreen);
	bool init()
	{
		if(!Node::init())
			return false;

		listenerKeyboard = EventListenerKeyboard::create();
		listenerKeyboard->onKeyReleased = CC_CALLBACK_2(AboutScreen::onKeyReleased, this);
		getEventDispatcher()->addEventListenerWithFixedPriority(listenerKeyboard, 1);

		bk = BlackBk::create();
		bk->retain();
		addChild(bk,0);

		heading = Label::createWithTTF("About","fonts/TradeWinds-Regular.ttf",140);//CC_CONTENT_SCALE_FACTOR());
		heading->retain();
		heading->setAlignment(TextHAlignment::CENTER);
		addChild(heading,1);

		content = Label::createWithTTF("Zabyrinth is a simple game based on classic Labyrinth. \n "
				"To play, select a difficulty and use your\n device as the game board to control the balls journey "
				"through the maze,\n avoiding the holes, until it reaches the checkered tile.\n"
				"\nCreated by Zach Moore Copyright 2014","fonts/Nobile-Regular.ttf",50);//*CC_CONTENT_SCALE_FACTOR());
		content->retain();
		content->setAlignment(TextHAlignment::CENTER);
		addChild(content,1);

		content->setPosition(DataMan::instance().screenCenter());
		Size cSize = content->getContentSize();

		float spacing = 100;///CC_CONTENT_SCALE_FACTOR();

		heading->setPosition(Vec2(DataMan::instance().screenCenter().x,
				DataMan::instance().screenCenter().y+cSize.height/2.f+spacing));

		float btnOffset = 100;///CC_CONTENT_SCALE_FACTOR();

		backBtn = MenuItemImage::create("backBtn.png",
				   "backBtn.png",
				   CC_CALLBACK_1(AboutScreen::back, this));
		backBtn->getNormalImage()->setScale(0.5f);
		backBtn->getSelectedImage()->setScale(0.5f);
		backBtn->retain();
		Vec2 centerpos = DataMan::instance().screenCenter();
		backBtn->setPosition(Vec2(centerpos.x+DataMan::instance().screenSize().width,(DataMan::instance().screenOrigin().y+backBtn->getContentSize().height*0.5f)+btnOffset));
		auto backText = Label::createWithTTF("Back","fonts/Nobile-Regular.ttf",50);//CC_CONTENT_SCALE_FACTOR());
		backText->setAlignment(TextHAlignment::CENTER);
		backBtn->addChild(backText,0);
		backText->setPosition(Vec2(backBtn->getContentSize().width/4.f,-backBtn->getContentSize().height/6.f));

		menu = Menu::create(backBtn,nullptr);
		menu->setPosition(Vec2::ZERO);
		menu->retain();
		addChild(menu,1);

		setCascadeOpacityEnabled(true);

		setOpacity(0);

		return true;
	}

	void exec()
	{
		CallFunc* btns = CallFunc::create(
		[this]()
		{
			Vec2 origin = DataMan::instance().screenOrigin();
			Size ssize = DataMan::instance().screenSize();
			float menuSpacing = 75;///CC_CONTENT_SCALE_FACTOR();
			float btnOffset = 100;///CC_CONTENT_SCALE_FACTOR();

			backBtn->runAction(
			EaseElasticInOut::create(
			MoveTo::create(0.5f,Vec2(origin.x+ssize.width-(backBtn->getContentSize().width*0.5f+menuSpacing), backBtn->getPosition().y+btnOffset))
			,2.f)
			);
		}
		);

		Sequence* seq = Sequence::create(FadeIn::create(0.5f),btns,nullptr);
		runAction(seq);
	}

	void back(Ref* pSender)
	{
		SAE->playEffect(BTNSOUND);
		removeFromParentAndCleanup(true);
		if(backFunc)
		{
			backFunc();
		}
	}

	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
	{
		if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE )
		{
			SAE->playEffect(BTNSOUND);
			removeFromParentAndCleanup(true);
			if(backFunc)
			{
				backFunc();
			}
		}
	}

};


#endif /* ABOUTSCREEN_H_ */
