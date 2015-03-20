/*End Game Scene:
 * Handle level success or failure with scene
 * © Zach Moore 2015
 * */

#include "EndScene.h"
#include "dataman.h"
#include "maze.h"
#include "loading.h"
#include "title.h"
#include "DiffSelector.h"
#include "sound.h"
#include <string>

int EndLayer::EndSceneNode = 43567;

EndLayer::EndLayer(bool didFail)
:failed(false),
 level(0),
 levCleared(nullptr),
 bk(nullptr),
 menuSpacing(10),
 btnOffset(10),
 retryBtn(nullptr),
 regenBtn(nullptr),
 titleBtn(nullptr),
 playNextBtn(nullptr),
 diffBtn(nullptr),
 menu(nullptr)
{
	failed = didFail;

}

EndLayer::~EndLayer()
{
	if(levCleared)
		levCleared->release();

	if(bk)
		bk->release();

	if(retryBtn)
		retryBtn->release();

	if(regenBtn)
		regenBtn->release();

	if(titleBtn)
		titleBtn->release();

	if(playNextBtn)
		playNextBtn->release();

	if(playNextBtn)
		playNextBtn->release();

	if(menu)
		menu->release();
}

EndLayer* EndLayer::create(bool didFail)
{
	EndLayer* ret = new EndLayer(didFail);
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

bool EndLayer::init()
{
	if(!Node::init())
		return false;

	setTag(EndSceneNode);

	std::string levString = !failed ? "Congratulations!" :"Sorry...";

	bk = BlackBk::create();
	bk->retain();
	addChild(bk,0);

	btnOffset = 100;//*CC_CONTENT_SCALE_FACTOR();

	levCleared = Label::createWithTTF(levString,"fonts/TradeWinds-Regular.ttf",160);//*CC_CONTENT_SCALE_FACTOR());
	levCleared->retain();
	if(failed)
	{
		levCleared->setTextColor(Color4B(220,0,0,255));
	}
	levCleared->setAlignment(TextHAlignment::CENTER);
	//levCleared->setPosition(Vec2(DataMan::instance().screenOrigin().x-levCleared->getWidth(),DataMan::instance().screenCenter().y));
	addChild(levCleared,1);
	Vec2 centerpos = DataMan::instance().screenCenter();
	menuSpacing = 75;//*CC_CONTENT_SCALE_FACTOR();
	levCleared->setPosition(Vec2(centerpos.x,centerpos.y+levCleared->getHeight()+btnOffset+menuSpacing));

	setCascadeOpacityEnabled(true);

	setOpacity(0);

	unsigned char difficulty = DataMan::instance().getDifficulty();
	if(difficulty == 0)
	{
		diffNormCol = Color3B(0,200,0);
		diffSelCol = Color3B(50,255,50);
	}
	else if(difficulty == 1)
	{
		diffNormCol = Color3B(100,230,0);
		diffSelCol = Color3B(150,255,50);
	}
	else if(difficulty == 2)
	{
		diffNormCol = Color3B(230,230,0);
		diffSelCol = Color3B(255,255,50);
	}
	else if(difficulty == 3)
	{
		diffNormCol = Color3B(230,100,0);
		diffSelCol = Color3B(255,150,50);
	}
	else if(difficulty == 4)
	{
		diffNormCol = Color3B(200,0,0);
		diffSelCol = Color3B(255,50,50);
	}

	//btns
//	if(failed)
//	{
		//retry------------------------
		retryBtn = MenuItemImage::create("retryBtnOff.png",
				   "retryBtn.png",
				   CC_CALLBACK_1(EndLayer::retry, this));
		retryBtn->retain();
		retryBtn->setPosition(Vec2(centerpos.x-DataMan::instance().screenSize().width,(centerpos.y-retryBtn->getContentSize().height)+btnOffset));
		auto retryText = Label::createWithTTF("Retry","fonts/Nobile-Regular.ttf",50);//*CC_CONTENT_SCALE_FACTOR());
		retryText->setAlignment(TextHAlignment::CENTER);
		retryBtn->addChild(retryText,0);
		retryText->setPosition(Vec2(retryBtn->getContentSize().width/2.f,-retryBtn->getContentSize().height/6.f));

		//regen---------------------
		regenBtn = MenuItemImage::create("regenBtnOff.png",
				   "regenBtn.png",
				   CC_CALLBACK_1(EndLayer::regen, this));
		regenBtn->retain();
		regenBtn->setPosition(Vec2(centerpos.x-DataMan::instance().screenSize().width,(centerpos.y-retryBtn->getContentSize().height)+btnOffset));
		auto regenText = Label::createWithTTF("Regenerate","fonts/Nobile-Regular.ttf",50);//*CC_CONTENT_SCALE_FACTOR());
		regenText->setAlignment(TextHAlignment::CENTER);
		regenBtn->addChild(regenText,0);
		regenText->setPosition(Vec2(retryBtn->getContentSize().width/2.f,-retryBtn->getContentSize().height/6.f));

		//diff-----------------
		diffBtn = MenuItemImage::create("greyCircle.png",
				   "greyCircle.png",
				   CC_CALLBACK_1(EndLayer::changeDiff, this));
		auto ring = Sprite::create("emptyBtn.png");
		diffBtn->getNormalImage()->setColor(diffNormCol);
		diffBtn->getSelectedImage()->setColor(diffSelCol);
		diffBtn->addChild(ring);
		Size diffSize = diffBtn->getNormalImage()->getContentSize();
		ring->setPosition(Vec2(diffSize.width/2.f,diffSize.height/2.f));
		diffBtn->retain();
		diffBtn->setPosition(Vec2(centerpos.x+DataMan::instance().screenSize().width,(centerpos.y-retryBtn->getContentSize().height)+btnOffset));
		auto diffText = Label::createWithTTF("Difficulty","fonts/Nobile-Regular.ttf",50);//*CC_CONTENT_SCALE_FACTOR());
		diffText->setAlignment(TextHAlignment::CENTER);
		diffBtn->addChild(diffText,0);
		diffText->setPosition(Vec2(retryBtn->getContentSize().width/2.f,-retryBtn->getContentSize().height/6.f));

		//title-----------------
		titleBtn = MenuItemImage::create("titleBtnOff.png",
				   "titleBtn.png",
				   CC_CALLBACK_1(EndLayer::backTitle, this));
		titleBtn->retain();
		titleBtn->setPosition(Vec2(centerpos.x+DataMan::instance().screenSize().width,(centerpos.y-retryBtn->getContentSize().height)+btnOffset));
		auto titleText = Label::createWithTTF("Title Menu","fonts/Nobile-Regular.ttf",50);//*CC_CONTENT_SCALE_FACTOR());
		titleText->setAlignment(TextHAlignment::CENTER);
		titleBtn->addChild(titleText,0);
		titleText->setPosition(Vec2(retryBtn->getContentSize().width/2.f,-retryBtn->getContentSize().height/6.f));

		menu = Menu::create(retryBtn,regenBtn,diffBtn,titleBtn,nullptr);
		menu->setPosition(Vec2::ZERO);
		menu->retain();


	return true;
}

void EndLayer::exec()
{
		CallFunc* btns = CallFunc::create(
		[this]()
		{

			addChild(menu,1);
			Vec2 centerpos = DataMan::instance().screenCenter();

			retryBtn->runAction(
			EaseInOut::create(
			MoveTo::create(1.2f,Vec2(centerpos.x-((retryBtn->getContentSize().width+(retryBtn->getContentSize().width/2.f)+menuSpacing)+(menuSpacing/2.f)), retryBtn->getPosition().y+btnOffset))
			,2.f)
			);

			regenBtn->runAction(
			EaseInOut::create(
			MoveTo::create(0.8f,Vec2(centerpos.x-((retryBtn->getContentSize().width/2.f)+menuSpacing/2.f), retryBtn->getPosition().y+btnOffset))
			,2.f)
			);

			diffBtn->runAction(
			EaseInOut::create(
			MoveTo::create(0.8f,Vec2(centerpos.x+((retryBtn->getContentSize().width/2.0f)+menuSpacing/2.f), retryBtn->getPosition().y+btnOffset))
			,2.f)
			);

			titleBtn->runAction(
			EaseInOut::create(
			MoveTo::create(1.2f,Vec2((centerpos.x+(retryBtn->getContentSize().width+(retryBtn->getContentSize().width/2.f)+menuSpacing)+(menuSpacing/2.f)), retryBtn->getPosition().y+btnOffset))
			,2.f)
			);
		}
		);
		auto seq = Sequence::create(FadeIn::create(0.5f),btns,nullptr);
		runAction(seq);

}

void EndLayer::retry(Ref* pSender)
{

	SAE->playEffect(BTNSOUND);

	auto del = CallFunc::create(
	[this]()
	{
		MazePlayer* m = static_cast<MazePlayer*>(getParent());
		m->maze->resetMaze();
		removeFromParentAndCleanup(true);
	}
	);

	auto seq = Sequence::create(FadeTo::create(0.5f,0),del,nullptr);
	runAction(seq);

}

void EndLayer::regen(Ref* pSender)
{

	SAE->playEffect(BTNSOUND);

	Maze::finished = false;
	pause();
	LoadingScreen* ls = LoadingScreen::create();
	getParent()->addChild(ls,99);
	ls->callback = CallFunc::create(
	[](){
		MazePlayer* m = MazePlayer::create(DataMan::instance().getDifficulty());
		m->exec();
	}
	);
	ls->exec();
}

void EndLayer::backTitle(Ref* pSender)
{

	SAE->playEffect(BTNSOUND);

	TitleScene* ts = TitleScene::create();
	ts->exec();
}

void EndLayer::playNext(Ref* pSender)
{

	SAE->playEffect(BTNSOUND);

	Maze::finished = false;
	pause();
	LoadingScreen* ls = LoadingScreen::create();
	getParent()->addChild(ls,99);
	ls->callback = CallFunc::create(
	[](){
		MazePlayer* m = MazePlayer::create(DataMan::instance().getDifficulty());
		m->exec();
	}
	);
	ls->exec();
}

void EndLayer::changeDiff(Ref* pSender)
{
	SAE->playEffect(BTNSOUND);
	auto ds =DiffSelector::create();
	addChild(ds,99);
	ds->exec();
}

