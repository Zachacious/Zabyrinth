/* App Delegate:
 * initialize Cocos app and handle resolution independence
 * © Zach Moore 2015
 * */

#include "AppDelegate.h"
#include "Title.h"
#include "DataMan.h"

//Design Size
#define DWIDTH 2048 //target width
#define DHEIGHT 1536//target height
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director

    auto director = Director::getInstance(); //cocos director

    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("Zabyrinth");
        director->setOpenGLView(glview);
    }

    glview->setDesignResolutionSize(DWIDTH, DHEIGHT, ResolutionPolicy::NO_BORDER);
	Size frameSize = glview->getFrameSize();

        //multi res support !!

    //get Opengl maximum supported texture size
    int maxTexSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);

	if(frameSize.width > DWIDTH)
	{

		//if device supports hd graphics
		if(maxTexSize>=2048)
		{
			DataMan::instance().addResourceDir("hd");
		}
		else //cant handle hd
		{
			director->setContentScaleFactor(0.5f);
		}

	}
	else if(frameSize.width < DWIDTH)
	{
		if(frameSize.width <= 1024)
		{
			director->setContentScaleFactor(0.5f);
		}
		else if(maxTexSize>=2048)
		{
			DataMan::instance().addResourceDir("hd");
		}
		else //cant handle hd
		{
			director->setContentScaleFactor(0.5f);
		}

	}
	else //1:1 design resolution
	{
		if(maxTexSize>=2048)
		{
			DataMan::instance().addResourceDir("hd");
		}
		else //cant handle hd
		{
			director->setContentScaleFactor(0.5f);
		}
	}

	DataMan::instance().init(); // init data manager

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = TitleScene::create();

    // run
    //begin program with cocos
    auto trans = TransitionFade::create(1.0f, scene);
    director->runWithScene(trans);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
