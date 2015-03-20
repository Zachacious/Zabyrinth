/*Data Manager:
 * Singleton to provide data management and utility
 * note: some of this may not be used
 * © Zach Moore 2015
 * */

#include "DataMan.h"
#include <vector>

DataMan::DataMan()
:pathsSet(false),difficulty(0)
{

}

DataMan& DataMan::instance()
{
	static DataMan * singleton = new DataMan;
	return *singleton;
}

void DataMan::init()
{
	if(!pathsSet && searchPaths.size()>0)
	{
		FileUtils::getInstance()->setSearchPaths(searchPaths);
		pathsSet = true;
	}

	screensSize = Director::getInstance()->getVisibleSize();
	screensOrigin = Director::getInstance()->getVisibleOrigin();
	screensCenter = Vec2(screensOrigin.x + screensSize.width/2.f, screensOrigin.y + screensSize.height/2.f);

	gameData.init();
}

void DataMan::addResourceDir(string dir)
{
	searchPaths.push_back(dir);
}

void DataMan::loadSpriteSheet(string pList, string texture)
{
	//all search paths must be set before first resource loaded;
	if(!pathsSet)
	{
		FileUtils::getInstance()->setSearchPaths(searchPaths);
		pathsSet = true;
	}

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(pList,texture);

}

SpriteExt* DataMan::getSprite(string name)
{
	SpriteExt * ret = SpriteExt::createWithSpriteFrameName(name);
	return !ret ? SpriteExt::create(name): ret;
}

void DataMan::fixVec(Vec2 &v)
{
	v = Vec2((int)v.x,(int)v.y);
}

void DataMan::fixSize(Size &s)
{
	s = Size((int)s.width,(int)s.height);
}

void DataMan::updateScreenData()
{
	screensOrigin = Director::getInstance()->getVisibleOrigin();
	screensCenter = Vec2(screensSize.width/2.f + screensOrigin.x, screensSize.height/2.f + screensOrigin.y);
	log("sOrgin: %f, %f",screensOrigin.x,screensOrigin.y);
}

void DataMan::pauseNodeRecursive(Node* n)
{
	n->pause();
	Vector<Node*> children = n->getChildren();
	for(Node* c : children)
	{
		pauseNodeRecursive(c);
	}
}

void DataMan::resumeNodeRecursive(Node* n)
{
	n->resume();
	Vector<Node*> children = n->getChildren();
	for(Node* c : children)
	{
		resumeNodeRecursive(c);
	}
}
