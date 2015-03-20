/* Game Entities:
 * Entites contain cocos sprite data and box2d data and collision callbacks
 * © Zach Moore 2015
 * */

#ifndef PHYENTITIES_H_
#define PHYENTITIES_H_

#include "Box2d/Box2d.h"
#include "DataMan.h"
#include "camera.h"

class Entity;

struct EntUserData
{
	Entity* e;
	void* extraData;
	EntUserData():e(nullptr),extraData(nullptr){}
	virtual ~EntUserData();
	void init(Entity* ent);
};

/////////////////////////////////////////////////////
class Entity : public Ref
{
public:
	b2Body* body;
	b2BodyDef bodyDef;
	b2Fixture* fixture;
	b2FixtureDef fixtureDef;
	//b2CircleShape shape;
	Node* parent;
	SpriteExt* sprite;
	//void* userData;
	unsigned char tag;
	EntUserData UData;
	Entity();
	virtual ~Entity();
	virtual bool init(){return true;}
	virtual void collision(void* target){}
	virtual void setPosition(Vec2 pos);
	virtual Vec2 getPosition();
	virtual void setRotation(float r);
	virtual Size getContentSize();
	virtual void setVisible(bool v = true);
	virtual void setEnabled(bool e = true);
	virtual Vec2 worldCoords(Vec2 pos);
	virtual Rect getBBox();
};
/////////////////////////////////////////////////////
class BallEntity: public Entity
{
public:
	static unsigned char id;
	//static SoundFx* sfx;
	b2CircleShape shape;
	SpriteExt* shadow;
	NodeCamera* cam;
	unsigned char camFrameCount;
	BallEntity():shadow(nullptr),cam(nullptr),camFrameCount(0){}
	virtual ~BallEntity()
	{if(shadow){shadow->release();}
	if(cam){cam->release();}}
	static BallEntity* create(Node* parent);
	virtual void setPosition(Vec2 pos);
	bool init();
	void setCamera(NodeCamera* c);
	void camCallback(Vec2 pos);
};
/////////////////////////////////////////////////////
class WallEntity: public Entity
{
public:
	static unsigned char id;
	b2PolygonShape shape;
	static WallEntity* create(Node* parent);
	//virtual void collision(void* target);
	bool init();
};
/////////////////////////////////////////////////////
class CornerEntity: public Entity
{
public:
	static unsigned char id;
	b2PolygonShape shape;
	static CornerEntity* create(Node* parent);
	bool init();
};

class HoleEntity: public Entity
{
public:
	static unsigned char id;
	b2CircleShape shape;
	SpriteExt* stencil;
	HoleEntity(): stencil(nullptr){}
	virtual ~HoleEntity(){ if(stencil){ stencil->release(); }}
	static HoleEntity* create(Node* parent);
	virtual bool init();
	virtual void collision(void* target);
	virtual void setPosition(Vec2 pos);
//	void screenCB(bool succeed,const std::string& outputFile);
};
///////////////////////////////////////////////////

class ExitEntity: public Entity
{
public:
	static unsigned char id;
	b2CircleShape shape;
	static ExitEntity* create(Node* parent);
	virtual bool init();
	virtual void collision(void* target);
};

///////////////////////////////////////////////////
class blkBallEntity: public Entity
{
public:
	static unsigned char id;
	b2CircleShape shape;
	SpriteExt* shadow;
	blkBallEntity():shadow(nullptr){}
	virtual ~blkBallEntity()
	{if(shadow){shadow->release();}}
	static blkBallEntity* create(Node* parent);
	virtual void setPosition(Vec2 pos);
	bool init();
	virtual void collision(void* target);
};

///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////
#endif /* PHYENTITIES_H_ */
