/* Title Scene
 * © Zach Moore 2015
 * */

#include "Title.h"
#include "background.h"
#include "maze.h"
#include "loading.h"
#include "diffSelector.h"
#include "sound.h"
#include "AboutScreen.h"

Title::Title()
:dataMan(nullptr)/*,mp(nullptr)*/,logo(nullptr),bkLogo(nullptr),
 playBtn(nullptr),howToBtn(nullptr),menu(nullptr)
{

}

Title::~Title() {

	if(logo)
	{
		logo->release();
	}
	if(bkLogo)
	{
		bkLogo->release();
	}
	if(playBtn)
		playBtn->release();

	if(howToBtn)
		howToBtn->release();

	if(menu)
		menu->release();
}

bool Title::init()
{
	if(!Node::init())
	{
		return false;
	}

	dataMan = &DataMan::instance();

	//create title scene
	Vec2 logoPos = Vec2(dataMan->screenCenter().x,dataMan->screenCenter().y+250* CC_CONTENT_SCALE_FACTOR());
	logo = dataMan->getSprite("zabrynth.png");
	logo->setPosition(logoPos);
	logo->retain();
	logo->setOpacity(0);
	logo->setBlendFunc(BlendFunc::ADDITIVE);
	logo->setColor(Color3B(255,255,230));
	logo->setScale(0.8f);
	addChild(logo,2);

	bkLogo = dataMan->getSprite("zabrynth.png");
	bkLogo->setPosition(logoPos);
	bkLogo->setOpacity(0);
	bkLogo->setScale(0.8f);
	bkLogo->retain();

	playBtn = MenuItemImage::create(
									   "titlePlayBtn.png",
									   "titlePlayBtn.png",
									   CC_CALLBACK_1(Title::startCallBack, this));
	playBtn->getSelectedImage()->setColor(Color3B(230,230,230));
	float btnYOff = (playBtn->getNormalImage()->getContentSize().height/2)+
			60*CC_CONTENT_SCALE_FACTOR();

	playBtn->setPosition(Vec2(
	dataMan->screenCenter().x+dataMan->screenSize().width,
	dataMan->screenOrigin().y+btnYOff
	));
	playBtn->retain();

	howToBtn = MenuItemImage::create(
									   "aboutBtn.png",
									   "aboutBtn.png",
									   CC_CALLBACK_1(Title::howTo, this));
	howToBtn->getSelectedImage()->setColor(Color3B(230,230,230));
	howToBtn->setPosition(Vec2(
	dataMan->screenCenter().x-dataMan->screenSize().width,
	dataMan->screenOrigin().y+btnYOff
	));
	howToBtn->retain();

	// create menu, it's an autorelease object
	menu = Menu::create(playBtn,howToBtn, nullptr);
	menu->setPosition(Vec2::ZERO);
	menu->retain();
	addChild(menu, 1);

	setonEnterTransitionDidFinishCallback(CC_CALLBACK_0(Title::intro,this));

	return true;
}

void Title::intro()
{
	SAE->playEffect("audio/drum.wav");
	CallFunc* func = CallFunc::create(
	[this]()
	{
		//logo->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED);
		addChild(bkLogo,0);
		bkLogo->runAction(FadeIn::create(0.6f));

	}
	);

	CallFunc* btnFunc = CallFunc::create(
	[this]()
	{
		Vec2 center = dataMan->screenCenter();
		float spacing = 100 * CC_CONTENT_SCALE_FACTOR();
		playBtn->runAction(MoveTo::create(0.5f,Vec2((center.x+playBtn->getContentSize().width/2)+spacing,playBtn->getPosition().y)));
		howToBtn->runAction(MoveTo::create(0.5f,Vec2((center.x-playBtn->getContentSize().width/2)-spacing,playBtn->getPosition().y)));
	}
	);

	Sequence* seq = Sequence::create(FadeIn::create(1.3f),func,TintTo::create(0.6f,0,0,0),btnFunc,nullptr);
	logo->runAction(seq);
}

void Title::howTo(Ref* pSender)
{
	SAE->playEffect(BTNSOUND);
	auto as = AboutScreen::create();
	addChild(as,99);

	DataMan::pauseNodeRecursive(this);

	as->backFunc =
	[this]()
	{
		DataMan::resumeNodeRecursive(this);
	};

	as->exec();

}

void Title::startCallBack(Ref* pSender)
{
	SAE->playEffect(BTNSOUND);
	auto ds =DiffSelector::create();
	addChild(ds,99);

	DataMan::pauseNodeRecursive(this);

	ds->backFunc =
	[this]()
	{
		DataMan::resumeNodeRecursive(this);
	};

	ds->exec();

}

/////////////////////////////////////

TitleScene::TitleScene()
{
	auto layer = Title::create();
	auto bk = bkLayer::create(true);
	addChild(bk,0);
	addChild(layer,1);
}

void TitleScene::exec()
{

	auto trans = TransitionFade::create(2.0f, this);
	Director::getInstance()->replaceScene(trans);

}
