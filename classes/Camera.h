/* Node Camera:
 * Handle cocos lack of a camera.
 * --the Illusion of the camera is created by moving the
 * target layer relative to the virtual camera position
 * © Zach Moore 2015
 * */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "cocos2d.h"

USING_NS_CC;

class NodeCamera: public Node {
public:
	NodeCamera(Node* target); //target = the parent node which will actuall move
	virtual ~NodeCamera();
	static NodeCamera* create(Node* target);
	bool init();
	void setPosition(Vec2 pos);
	void setZoom(float factor = 1.0f);
	void setZoomAtPosition(float factor,Vec2 pos);
	inline void setOffset(Vec2 offset){ camOffset = offset; }
	inline void setBBox(Rect b){ bBox = b; }
	Rect getBBox();
	void smoothMove(Vec2 pos);

	void enableTouchMove();
	virtual bool onTouchBegan(Touch *pTouches, Event *pEvent){ return true; }
	virtual void onTouchMoved(Touch *pTouches, Event *pEvent);

private:
	Node* targetNode;
	Vec2 camPosition;
	Vec2 camOffset;
	float zoomFactor;
	bool touchMoveEnabled;
	Rect bBox;
	Action* moveAction;
};

#endif /* CAMERA_H_ */
