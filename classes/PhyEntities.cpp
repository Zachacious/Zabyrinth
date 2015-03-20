/* Game Entities:
 * Entites contain cocos sprite data and box2d data and collision callbacks
 * © Zach Moore 2015
 * */

#include "PhyEntities.h"
#include "PhyWorld.h"
#include "maze.h"
#include "EndScene.h"
#include "sound.h"
#include <cmath>

//static ids
unsigned char BallEntity::id = 1;
unsigned char WallEntity::id = 2;
unsigned char CornerEntity::id = 3;
//unsigned char SwitchEntity::id = 4;
//unsigned char PortalEntity::id = 5;
unsigned char HoleEntity::id = 6;
unsigned char ExitEntity::id = 7;
unsigned char blkBallEntity::id = 8;
unsigned char SpinEntity::id = 9;

EntUserData::~EntUserData()
{
	if(e)
	{
		e->release();
	}
}

void EntUserData::init(Entity* ent)
{
	if(ent)
	{
		e = ent;
		e->retain();
	}
}

Entity::Entity()
:body(nullptr), fixture(nullptr),parent(nullptr),sprite(nullptr)/*,userData(0)*/,tag(0)
{
	retain();
	UData.init(this);
} //need to keep ref cause not add as child}

Entity::~Entity()
{
	release();
	if(sprite)
	{
		sprite->release();
	}
	if(parent)
		parent->release();
}

void Entity::setPosition(Vec2 pos)
{
	//set position must set both cocos sprite and physics body

	if(body && body->GetType() == b2_dynamicBody)
	{
		b2Vec2 b2Pos = b2Vec2(pos.x/PTMRatio,pos.y/PTMRatio);
		body->SetTransform(b2Pos,body->GetAngle());
	}
	else
	{
		if(sprite){sprite->setPosition(pos);}
		b2Vec2 b2Pos = b2Vec2(pos.x/PTMRatio,pos.y/PTMRatio);
		body->SetTransform(b2Pos,body->GetAngle());
	}

}

Vec2 Entity::getPosition()
{
	if(body)
	{
		b2Vec2 pos = body->GetPosition();
		return Vec2(pos.x*PTMRatio,pos.y*PTMRatio);
	}
	else if(sprite)
	{
		return sprite->getPosition();
	}
	return Vec2::ZERO;
}

void Entity::setRotation(float r)
{
	//rotate phy body and sprite
	if(body && body->GetType() == b2_dynamicBody)
	{
		body->SetTransform(body->GetPosition(),CC_DEGREES_TO_RADIANS(r)/*-(r*DEG_TO_RAD)*/);
	}
	else
	{
		if(sprite){sprite->setRotation(r);}
		body->SetTransform(body->GetPosition(),-CC_DEGREES_TO_RADIANS(r)/*-(r*DEG_TO_RAD)*/);
	}

}

Size Entity::getContentSize() { return sprite->getContentSize(); }

void Entity::setVisible(bool v){ sprite->setVisible(v); }

void Entity::setEnabled(bool e){ body->SetActive(e); }

Vec2 Entity::worldCoords(Vec2 pos)
{
	if(sprite)
	{
		return sprite->convertToWorldSpace(pos);
	}
	return pos;
}

Rect Entity::getBBox()
{
	if(sprite)
	{
		return sprite->getBoundingBox();
	}
	else
		return Rect::ZERO;
}
///////////////////////////////////////////////////////////////////////

BallEntity* BallEntity::create(Node* parent)
{
	BallEntity* ret = new BallEntity;
	if(ret && parent)
	{
		ret->autorelease();
		ret->parent = parent;
		ret->parent->retain();
		ret->sprite = DataMan::instance().getSprite("ball.png");
		ret->shadow = DataMan::instance().getSprite("ballShadow.png");
		ret->shadow->retain();
		ret->sprite->setShadowSprite(ret->shadow);
		ret->sprite->retain();
		parent->addChild(ret->shadow,4);
		parent->addChild(ret->sprite,4);

		if(ret->init())
			return ret;
	}

	CC_SAFE_DELETE(ret);
	ret= nullptr;
	return ret;
}

bool BallEntity::init()
{

	tag = id; // what type of entity is this? used for collision
	//setup physics body
	bodyDef.type = b2_dynamicBody;
	bodyDef.angularDamping = 0.9f;
	bodyDef.allowSleep = false;
	bodyDef.fixedRotation = false;
	body = PWorld->CreateBody(&bodyDef);
	sprite->body = body;
	shape.m_radius = ((sprite->getContentSize().width)/2.0f)/PTMRatio; //radius relative to center of sprite

	fixtureDef.shape = &shape;
	fixtureDef.density = 5.f*CC_CONTENT_SCALE_FACTOR();
	fixtureDef.friction = 0.0005f;
	fixtureDef.restitution = 0.4f;

	body->CreateFixture(&fixtureDef);
	body->SetUserData(&UData);

	//we need to know the camera in use so that we can update position with movement
	Maze* m = dynamic_cast<Maze*>(parent);
	if(m->getCamera())
	{
		setCamera(m->getCamera());
		m->getCamera()->retain();
	}

	return true;
}

void BallEntity::setPosition(Vec2 pos)
{
	Entity::setPosition(pos);
	if(!shadow)
		return;

	shadow->setPosition(sprite->getPosition());

}

void BallEntity::setCamera(NodeCamera* c)
{
	cam = c;
	cam->retain();
	//see spriteExt
	sprite->setRepositionCallback(CC_CALLBACK_1(BallEntity::camCallback,this));
}

void BallEntity::camCallback(Vec2 pos) //used by spriteExt to reposition camera upon sprite movement
{
	if(!cam)
		return;

	pos = worldCoords(pos/PTMRatio);

	cam->setPosition(pos);
}

////////////////////////////////////////////////////////////////////////

WallEntity* WallEntity::create(Node* parent)
{
	WallEntity* ret = new WallEntity;
	if(ret && parent)
	{
		ret->autorelease();
		ret->parent = parent;
		ret->parent->retain();
		ret->sprite = DataMan::instance().getSprite("wall.png");
		parent->addChild(ret->sprite,6);
		ret->sprite->retain();
		if(ret->init())
			return ret;
	}

	CC_SAFE_DELETE(ret);
	ret= nullptr;
	return ret;
}

bool WallEntity::init()
{

	tag = id;
	bodyDef.type = b2_staticBody;
	bodyDef.allowSleep = true;
	body = PWorld->CreateBody(&bodyDef);
	sprite->body = body;

	//set phy shape relative to center of sprite
	float perc = 0.1171875f;
	Size s = sprite->getContentSize();
	s = Size((s.width-1)/PTMRatio,(s.height-1)/PTMRatio);
	Size size = Size(((perc*s.width)/2.f), (s.height/2.f));
	b2Vec2 center = b2Vec2((s.width/2.f-((perc*s.width)/2.f)), 0/*(s.height/2.f)*/);
	shape.SetAsBox(size.width,size.height,center,0);

	fixtureDef.shape = &shape;
	fixtureDef.density = 3.0f;
	fixtureDef.friction = 0.5f;
	fixtureDef.restitution = 0.2f;

	body->CreateFixture(&fixtureDef);
	body->SetUserData(&UData);
	return true;
}

/////////////////////////////////////////////////////////////////////////

CornerEntity* CornerEntity::create(Node* parent)
{
	CornerEntity* ret = new CornerEntity;
	if(ret && parent)
	{
		ret->autorelease();
		ret->parent = parent;
		ret->parent->retain();
		ret->sprite = DataMan::instance().getSprite("corner1.png");
		parent->addChild(ret->sprite,8);
		ret->sprite->retain();
		if(ret->init())
			return ret;
	}

	CC_SAFE_DELETE(ret);
	ret= nullptr;
	return ret;
}

bool CornerEntity::init()
{

	tag = id;
	bodyDef.type = b2_staticBody;
	bodyDef.allowSleep = true;
	body = PWorld->CreateBody(&bodyDef);
	sprite->body = body;

	float perc = 0.1171875f;
	Size s = sprite->getContentSize();
	s = Size((s.width-1)/PTMRatio,(s.height-1)/PTMRatio);
	Size size = Size(((perc*s.width)/2.f), ((perc*s.width)/2.f));
	b2Vec2 center = b2Vec2((s.width/2.f-((perc*s.width)/2.f)), -(s.width/2.f-((perc*s.width)/2.f)));
	shape.SetAsBox(size.width,size.height,center,0);

	fixtureDef.shape = &shape;
	fixtureDef.density = 3.0f;
	fixtureDef.friction = 0.5f;
	fixtureDef.restitution = 0.2f;

	body->CreateFixture(&fixtureDef);
	body->SetUserData(&UData);
	return true;
}

////////////////////////////////////////////////////////////////////////

HoleEntity* HoleEntity::create(Node* parent)
{
	HoleEntity* ret = new HoleEntity;
	if(ret && parent)
	{
		ret->autorelease();
		ret->parent = parent;
		ret->parent->retain();
		ret->sprite = DataMan::instance().getSprite("hole.png");
		ret->stencil = DataMan::instance().getSprite("holeStencil.png");
		ret->stencil->retain();

		parent->addChild(ret->sprite,2);

		Maze* m = dynamic_cast<Maze*>(parent);//parent should be a maze
		m->holesStencil->addChild(ret->stencil,0);
		ret->sprite->retain();
		if(ret->init())
			return ret;
	}

	CC_SAFE_DELETE(ret);
	ret= nullptr;
	return ret;
}

bool HoleEntity::init()
{

	tag = id;
	bodyDef.type = b2_staticBody;

	bodyDef.allowSleep = true;
	body = PWorld->CreateBody(&bodyDef);
	sprite->body = body;
	shape.m_radius = ((sprite->getContentSize().width)/20.0f)/PTMRatio;

	fixtureDef.shape = &shape;
	fixtureDef.density = 0.f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.25f;
	fixtureDef.isSensor = true;

	body->CreateFixture(&fixtureDef);
	body->SetUserData(&UData);
	return true;
}

void HoleEntity::collision(void* target)
{
	if(target)
	{
		Entity* e = (Entity*)target;
		if(e->tag != BallEntity::id)
			return;

		BallEntity* b = static_cast<BallEntity*>(target);
		b2Vec2 vel = b->body->GetLinearVelocity();
		if(std::fabs(vel.x)<15.f && std::fabs(vel.y)<15.f)
		{
			b->body->SetLinearVelocity(b2Vec2(0,0));
			auto drop = CallFunc::create(
			[b]()
			{
				b->sprite->setScale(0.9f);
				b->shadow->setScale(0.9f);
				b->shadow->getParent()->reorderChild(b->shadow,0);
				b->sprite->getParent()->reorderChild(b->sprite,0);
				b->setEnabled(false);
				SAE->playEffect("audio/holedrop2.wav");
			}
			);
			auto end = CallFunc::create(
			[b]()
			{
				b->setVisible(false);
				b->shadow->setVisible(false);

				if(!Maze::finished)
				{
					Maze::finished = true;
					EndLayer* ending = EndLayer::create(true);
					b->sprite->getParent()->getParent()->addChild(ending,99);
					ending->exec();
				}

			}
			);
			auto seq = Sequence::create(drop,DelayTime::create(0.25f),end,nullptr);
			b->sprite->runAction(seq);

		}

	}
}

void HoleEntity::setPosition(Vec2 pos)
{
	Entity::setPosition(pos);
	stencil->setPosition(pos);
}

////////////////////////////////////////////////////////////////////////
ExitEntity* ExitEntity::create(Node* parent)
{
	ExitEntity* ret = new ExitEntity;
	if(ret && parent)
	{
		ret->autorelease();
		ret->parent = parent;
		ret->parent->retain();
		ret->sprite = DataMan::instance().getSprite("finish.jpg");
		//ret->sprite->setOpacity(127);
		parent->addChild(ret->sprite,2);
		ret->sprite->retain();
		if(ret->init())
			return ret;
	}

	CC_SAFE_DELETE(ret);
	ret= nullptr;
	return ret;
}

bool ExitEntity::init()
{

	tag = id;
	bodyDef.type = b2_staticBody;
	bodyDef.allowSleep = true;
	body = PWorld->CreateBody(&bodyDef);
	sprite->body = body;
	shape.m_radius = ((sprite->getContentSize().width)/3.f)/PTMRatio;

	fixtureDef.shape = &shape;
	fixtureDef.density = 0.f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.25f;
	fixtureDef.isSensor = true;

	body->CreateFixture(&fixtureDef);
	body->SetUserData(&UData);
	/////////
//	BlendFunc overlayBlend = {GL_SRC_COLOR,GL_DST_COLOR};
//	sprite->setBlendFunc(overlayBlend);
	return true;
}

void ExitEntity::collision(void* target)
{
	if(target)
	{
		Entity* e = (Entity*)target;
		if(e->tag != BallEntity::id)
			return;

		if(!Maze::finished)
		{
			SAE->playEffect("audio/dust.wav");
			Maze::finished = true;
			EndLayer* ending = EndLayer::create(false);
			e->sprite->getParent()->getParent()->addChild(ending,99);
			ending->exec();
		}

	}
}
////////////////////////////////////////////////////////////////////////
blkBallEntity* blkBallEntity::create(Node* parent)
{
	blkBallEntity* ret = new blkBallEntity;
	if(ret && parent)
	{
		ret->autorelease();
		ret->parent = parent;
		ret->parent->retain();
		ret->sprite = DataMan::instance().getSprite("blackBall.png");
		ret->shadow = DataMan::instance().getSprite("blackBallShadow.png");
		ret->shadow->retain();
		ret->sprite->setShadowSprite(ret->shadow);
		ret->sprite->retain();
		parent->addChild(ret->shadow,4);
		parent->addChild(ret->sprite,4);

		if(ret->init())
			return ret;
	}

	CC_SAFE_DELETE(ret);
	ret= nullptr;
	return ret;
}

bool blkBallEntity::init()
{

	tag = id;
	bodyDef.type = b2_dynamicBody;
	bodyDef.angularDamping = 0.9f;
	bodyDef.allowSleep = false;
	bodyDef.fixedRotation = false;
	body = PWorld->CreateBody(&bodyDef);
	sprite->body = body;
	shape.m_radius = ((sprite->getContentSize().width)/2.4f)/PTMRatio;

	fixtureDef.shape = &shape;
	fixtureDef.density = 6.f*CC_CONTENT_SCALE_FACTOR();
	fixtureDef.friction = 0.0005f;
	fixtureDef.restitution = 0.5f;

	body->CreateFixture(&fixtureDef);
	body->SetUserData(&UData);

	return true;
}

void blkBallEntity::setPosition(Vec2 pos)
{
	Entity::setPosition(pos);
	if(!shadow)
		return;

	shadow->setPosition(sprite->getPosition());

}

void blkBallEntity::collision(void* target)
{
	//SAE->playEffect("audio/ballclank.wav");
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
