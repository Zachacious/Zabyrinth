/*Difficulty Selector Scene
 * handle diff selector screen with animations
 * © Zach Moore 2015
 * */

#ifndef DIFFSELECTOR_H_
#define DIFFSELECTOR_H_

#include "cocos2d.h"
#include "loading.h"
#include "maze.h"
#include "sound.h"

USING_NS_CC;

class DiffSelector: public Node
{
public:
	DiffSelector():
		heading(nullptr),bk(nullptr),
		menuSpacing(10),btnOffset(10),
		veasyBtn(nullptr),easyBtn(nullptr),normalBtn(nullptr),
		hardBtn(nullptr),insaneBtn(nullptr),backBtn(nullptr),
		difficulty(0),menu(nullptr),backFunc(nullptr),listenerKeyboard(nullptr)
	{}

	virtual ~DiffSelector()
	{
		if(heading)
			heading->release();
		if(bk)
			bk->release();
		if(menu)
			menu->release();
		//btns
		if(veasyBtn)
			veasyBtn->release();
		if(easyBtn)
			easyBtn->release();
		if(normalBtn)
			normalBtn->release();
		if(hardBtn)
			hardBtn->release();
		if(insaneBtn)
			insaneBtn->release();
		if(backBtn)
			backBtn->release();


		if(listenerKeyboard)
				getEventDispatcher()->removeEventListener(listenerKeyboard);
	}

	CREATE_FUNC(DiffSelector);

	bool init()
	{
		if(!Node::init())
			return false;

		listenerKeyboard = EventListenerKeyboard::create();
		listenerKeyboard->onKeyReleased = CC_CALLBACK_2(DiffSelector::onKeyReleased, this);
		getEventDispatcher()->addEventListenerWithFixedPriority(listenerKeyboard, 1);

		bk = BlackBk::create();
		bk->retain();
		addChild(bk,0);

		btnOffset = 100*CC_CONTENT_SCALE_FACTOR();

		heading = Label::createWithTTF("Please Select A Difficulty","fonts/TradeWinds-Regular.ttf",140);//*CC_CONTENT_SCALE_FACTOR());
		heading->retain();
		heading->setAlignment(TextHAlignment::CENTER);
		addChild(heading,1);

		Vec2 centerpos = DataMan::instance().screenCenter();
		menuSpacing = 75;//*CC_CONTENT_SCALE_FACTOR();
		heading->setPosition(Vec2(centerpos.x,centerpos.y+heading->getHeight()+btnOffset+menuSpacing));

		setCascadeOpacityEnabled(true);

		setOpacity(0);

		//btns

		//veasy------------------------
		veasyBtn = MenuItemImage::create("greyCircle.png",
				   "greyCircle.png",
				   CC_CALLBACK_1(DiffSelector::veasy, this));
		veasyBtn->getNormalImage()->setColor(Color3B(0,200,0));
		veasyBtn->getSelectedImage()->setColor(Color3B(50,225,50));
		auto ring = Sprite::create("emptyBtn.png");
		veasyBtn->addChild(ring);
//		veasyBtn->getNormalImage()->addChild(ring);
//		veasyBtn->getSelectedImage()->addChild(ring);
		Size btnSize = veasyBtn->getNormalImage()->getContentSize();
		ring->setPosition(Vec2(btnSize.width/2.f,btnSize.height/2.f));
		veasyBtn->retain();
		//retryBtn->setPosition(Vec2(centerpos.x,centerpos.y-retryBtn->getContentSize().height));
		veasyBtn->setPosition(Vec2(centerpos.x-DataMan::instance().screenSize().width,(centerpos.y-veasyBtn->getContentSize().height)+btnOffset));
		auto veasyText = Label::createWithTTF("Very Easy","fonts/Nobile-Regular.ttf",50);//*CC_CONTENT_SCALE_FACTOR());
		veasyText->setAlignment(TextHAlignment::CENTER);
		veasyBtn->addChild(veasyText,0);
		veasyText->setPosition(Vec2(veasyBtn->getContentSize().width/2.f,-veasyBtn->getContentSize().height/6.f));
		log("veasy");
		//easy------------------------
		easyBtn = MenuItemImage::create("greyCircle.png",
				   "greyCircle.png",
				   CC_CALLBACK_1(DiffSelector::easy, this));
		easyBtn->getNormalImage()->setColor(Color3B(100,230,0));
		easyBtn->getSelectedImage()->setColor(Color3B(150,255,50));
		auto ering = Sprite::create("emptyBtn.png");
		easyBtn->addChild(ering);
		ering->setPosition(Vec2(btnSize.width/2.f,btnSize.height/2.f));
		easyBtn->retain();
		easyBtn->setPosition(Vec2(centerpos.x-DataMan::instance().screenSize().width,(centerpos.y-veasyBtn->getContentSize().height)+btnOffset));
		auto easyText = Label::createWithTTF("Easy","fonts/Nobile-Regular.ttf",50);//*CC_CONTENT_SCALE_FACTOR());
		easyText->setAlignment(TextHAlignment::CENTER);
		easyBtn->addChild(easyText,0);
		easyText->setPosition(Vec2(veasyBtn->getContentSize().width/2.f,-veasyBtn->getContentSize().height/6.f));
		log("eaxy");
		//normal------------------------
		normalBtn = MenuItemImage::create("greyCircle.png",
				   "greyCircle.png",
				   CC_CALLBACK_1(DiffSelector::normal, this));
		normalBtn->getNormalImage()->setColor(Color3B(230,230,0));
		normalBtn->getSelectedImage()->setColor(Color3B(255,255,50));
		auto nring = Sprite::create("emptyBtn.png");
		normalBtn->addChild(nring);
		nring->setPosition(Vec2(btnSize.width/2.f,btnSize.height/2.f));
		normalBtn->retain();
		normalBtn->setPosition(Vec2(centerpos.x,(centerpos.y-DataMan::instance().screenSize().height)+btnOffset));
		auto normalText = Label::createWithTTF("Normal","fonts/Nobile-Regular.ttf",50);//*CC_CONTENT_SCALE_FACTOR());
		normalText->setAlignment(TextHAlignment::CENTER);
		normalBtn->addChild(normalText,0);
		normalText->setPosition(Vec2(veasyBtn->getContentSize().width/2.f,-veasyBtn->getContentSize().height/6.f));
		log("norm");
		//hard------------------------
		hardBtn = MenuItemImage::create("greyCircle.png",
				   "greyCircle.png",
				   CC_CALLBACK_1(DiffSelector::hard, this));
		hardBtn->getNormalImage()->setColor(Color3B(230,100,0));
		hardBtn->getSelectedImage()->setColor(Color3B(255,150,50));
		auto hring = Sprite::create("emptyBtn.png");
		hardBtn->addChild(hring);
		hring->setPosition(Vec2(btnSize.width/2.f,btnSize.height/2.f));
		hardBtn->retain();
		hardBtn->setPosition(Vec2(centerpos.x+DataMan::instance().screenSize().width,(centerpos.y-veasyBtn->getContentSize().height)+btnOffset));
		auto hardText = Label::createWithTTF("Hard","fonts/Nobile-Regular.ttf",50);//*CC_CONTENT_SCALE_FACTOR());
		hardText->setAlignment(TextHAlignment::CENTER);
		hardBtn->addChild(hardText,0);
		hardText->setPosition(Vec2(veasyBtn->getContentSize().width/2.f,-veasyBtn->getContentSize().height/6.f));
		log("hard");
		//insane------------------------
		insaneBtn = MenuItemImage::create("greyCircle.png",
				   "greyCircle.png",
				   CC_CALLBACK_1(DiffSelector::insane, this));
		insaneBtn->getNormalImage()->setColor(Color3B(200,0,0));
		insaneBtn->getSelectedImage()->setColor(Color3B(225,50,50));
		auto iring = Sprite::create("emptyBtn.png");
		insaneBtn->addChild(iring);
		iring->setPosition(Vec2(btnSize.width/2.f,btnSize.height/2.f));
		insaneBtn->retain();
		insaneBtn->setPosition(Vec2(centerpos.x+DataMan::instance().screenSize().width,(centerpos.y-veasyBtn->getContentSize().height)+btnOffset));
		auto insaneText = Label::createWithTTF("Insane","fonts/Nobile-Regular.ttf",50);//*CC_CONTENT_SCALE_FACTOR());
		insaneText->setAlignment(TextHAlignment::CENTER);
		insaneBtn->addChild(insaneText,0);
		insaneText->setPosition(Vec2(veasyBtn->getContentSize().width/2.f,-veasyBtn->getContentSize().height/6.f));
		log("insane");
		//back------------------------
		backBtn = MenuItemImage::create("backBtn.png",
				   "backBtn.png",
				   CC_CALLBACK_1(DiffSelector::back, this));
		backBtn->getNormalImage()->setScale(0.5f);
		backBtn->getSelectedImage()->setScale(0.5f);
		backBtn->retain();
		backBtn->setPosition(Vec2(centerpos.x+DataMan::instance().screenSize().width,(DataMan::instance().screenOrigin().y+backBtn->getContentSize().height*0.5f)+btnOffset));
		auto backText = Label::createWithTTF("Back","fonts/Nobile-Regular.ttf",50);//*CC_CONTENT_SCALE_FACTOR());
		backText->setAlignment(TextHAlignment::CENTER);
		backBtn->addChild(backText,0);
		backText->setPosition(Vec2(backBtn->getContentSize().width/4.f,-backBtn->getContentSize().height/6.f));
		log("back");
		//menu
		menu = Menu::create(veasyBtn,easyBtn,normalBtn,hardBtn,insaneBtn,backBtn,nullptr);
		menu->setPosition(Vec2::ZERO);
		menu->retain();

		return true;
	}

	void exec()
	{
		CallFunc* btns = CallFunc::create(
		[this]()
		{

			addChild(menu,1);
			Vec2 centerpos = DataMan::instance().screenCenter();

			//animations
			veasyBtn->runAction(
			EaseInOut::create(
			MoveTo::create(1.2f,Vec2(centerpos.x-(veasyBtn->getContentSize().width+menuSpacing)*2, veasyBtn->getPosition().y+btnOffset))
			,2.f)
			);

			easyBtn->runAction(
			EaseInOut::create(
			MoveTo::create(0.8f,Vec2(centerpos.x-(veasyBtn->getContentSize().width+menuSpacing), veasyBtn->getPosition().y+btnOffset))
			,2.f)
			);

			normalBtn->runAction(
			EaseInOut::create(
			MoveTo::create(0.4f,Vec2(centerpos.x, veasyBtn->getPosition().y+btnOffset))
			,2.f)
			);

			hardBtn->runAction(
			EaseInOut::create(
			MoveTo::create(0.8f,Vec2(centerpos.x+(veasyBtn->getContentSize().width+menuSpacing), veasyBtn->getPosition().y+btnOffset))
			,2.f)
			);

			insaneBtn->runAction(
			EaseInOut::create(
			MoveTo::create(1.2f,Vec2(centerpos.x+(veasyBtn->getContentSize().width+menuSpacing)*2, veasyBtn->getPosition().y+btnOffset))
			,2.f)
			);

			Vec2 origin = DataMan::instance().screenOrigin();
			Size ssize = DataMan::instance().screenSize();

			backBtn->runAction(
			EaseElasticInOut::create(
			MoveTo::create(1.8f,Vec2(origin.x+ssize.width-(backBtn->getContentSize().width*0.5f+menuSpacing), backBtn->getPosition().y+btnOffset))
			,2.f)
			);
		}
		);
		auto seq = Sequence::create(FadeIn::create(0.5f),btns,nullptr);
		runAction(seq);
	}

	Label* heading;
	BlackBk* bk;
	int menuSpacing;
	float btnOffset;

	//btns clicked callbacks----------------------

	MenuItemImage* veasyBtn;
	void veasy(Ref* pSender)
	{

		loadMaze(0);
	}
	MenuItemImage* easyBtn;
	void easy(Ref* pSender)
	{
		loadMaze(1);
	}
	MenuItemImage* normalBtn;
	void normal(Ref* pSender)
	{
		loadMaze(2);
	}
	MenuItemImage* hardBtn;
	void hard(Ref* pSender)
	{
		loadMaze(3);
	}
	MenuItemImage* insaneBtn;
	void insane(Ref* pSender)
	{
		loadMaze(4);
	}

	MenuItemImage* backBtn;
	void back(Ref* pSender)
	{
		SAE->playEffect(BTNSOUND);
		removeFromParentAndCleanup(true);
		if(backFunc)
		{
			backFunc();
		}
	}

	void loadMaze(unsigned char diff)
	{
		SAE->playEffect(BTNSOUND);
		difficulty = diff;
		DataMan::instance().setDifficulty(diff);
		LoadingScreen* ls = LoadingScreen::create();
		addChild(ls,101);
		ls->callback = CallFunc::create(
		[this]()
		{
			auto mp = MazePlayer::create(difficulty);
			mp->exec();
		}
		);
		ls->exec();
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

	unsigned char difficulty;
	Menu* menu;
	std::function<void()> backFunc;
	EventListenerKeyboard* listenerKeyboard;
};

#endif /* DIFFSELECTOR_H_ */
