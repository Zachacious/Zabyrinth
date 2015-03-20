/* Node Camera:
 * Handle cocos lack of a camera.
 * --the Illusion of the camera is created by moving the
 * target layer relative to the virtual camera position
 * © Zach Moore 2015
 * */

#include "Camera.h"
#include "DataMan.h"
#include "Maze.h"

NodeCamera::NodeCamera(Node* target)
:zoomFactor(1.0f),
 touchMoveEnabled(false),
 moveAction(nullptr)
{
	if(target)
	{

		targetNode = target;
		targetNode->retain();
	}

}

NodeCamera::~NodeCamera()
{
	if(targetNode)
	{
		targetNode->release();
	}
}

NodeCamera* NodeCamera::create(Node* target)
{
	NodeCamera* ret =new NodeCamera(target);
	if(ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		delete ret;
		ret = 0;
	}
	return ret;
}

bool NodeCamera::init()
{
	if(!Node::init())
	{
		return false;
	}

	Size ssize = DataMan::instance().screenSize();
	Vec2 scenter = DataMan::instance().screenCenter();
	Vec2 pos = Vec2(scenter.x-ssize.width/4.f,scenter.y-ssize.height/4.f);
	Size size = ssize/2.f;
	bBox.setRect(pos.x,pos.y,size.width,size.height);

	return true;
}

void NodeCamera::enableTouchMove()
{
	if(touchMoveEnabled)
	{
		return;
	}

	auto touchListner = EventListenerTouchOneByOne::create();
	touchListner->onTouchBegan = CC_CALLBACK_2(NodeCamera::onTouchBegan, this);
	touchListner->onTouchMoved = CC_CALLBACK_2(NodeCamera::onTouchMoved, this);
	touchListner->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithFixedPriority(touchListner, -1);

	touchMoveEnabled = true;

}

void NodeCamera::setPosition(Vec2 pos)
{
	//DataMan::fixVec(pos);
	camPosition = pos;
	if(!targetNode)
	{
		//log("no target");
		return;
	}

	Vec2 delta = DataMan::instance().screenCenter() - pos;
	Vec2 cpos = delta+targetNode->getPosition()+camOffset;
	//log("campos: %f, %f",cpos.x,cpos.y);
	targetNode->setPosition(cpos);
}

void NodeCamera::setZoom(float factor)
{
	zoomFactor = factor;
	if(!targetNode)
	{
		return;
	}

	targetNode->setScale(factor);

}

void NodeCamera::setZoomAtPosition(float factor, Vec2 pos)
{
	zoomFactor = factor;
	if(!targetNode)
	{
		return;
	}

	//DataMan::fixVec(pos);
	setPosition(pos);
	targetNode->setScale(factor);
}

void NodeCamera::onTouchMoved(Touch *pTouches, Event *pEvent)
{
	if(touchMoveEnabled && targetNode)
	{
		auto pos = pTouches->getLocation();
		//DataMan::fixVec(pos);
		auto prevPos = pTouches->getPreviousLocation();
		//DataMan::fixVec(prevPos);
		auto delta =  pos-prevPos;
		//DataMan::fixVec(delta);
		auto position = targetNode->getPosition();
		//DataMan::fixVec(position);
		//setPosition(position-delta);
		Vec2 newPos = position-delta;

		Vec2 tPos = targetNode->getPosition();
		Vec2 pdelta = Vec2(tPos.x-newPos.x,tPos.y-newPos.y);
		targetNode->setPosition(tPos+pdelta+camOffset);
	}

}

void NodeCamera::smoothMove(Vec2 pos)
{
	camPosition = pos;
	if(!targetNode)
	{
		//log("no target");
		return;
	}
	if(targetNode->getActionByTag(9999))
	{
		return;

	}
	Vec2 delta = DataMan::instance().screenCenter() - pos;
	Vec2 cpos = delta+targetNode->getPosition()+camOffset;
	//targetNode->setPosition(cpos);
	Action* move = EaseInOut::create(MoveTo::create(0.8f,cpos),2.f);
	move->setTag(9999);
	//move->retain();
	//moveAction = move;
	targetNode->runAction(move);
//	runAction(move);
}

Rect NodeCamera::getBBox()
{
	Size ssize = DataMan::instance().screenSize();
	Vec2 scenter = DataMan::instance().screenCenter();
	Vec2 pos = Vec2(scenter.x-ssize.width/3.f,scenter.y-ssize.height/3.f);
	bBox.origin = pos;
	return bBox;
}
