/* Sprite Extended:
 * Subclass of cocos sprite to include a callback that runs when set position is called.
 * also to allow a shadow sprite.
 * © Zach Moore 2015
 * */

#ifndef SPRITEEXT_H_
#define SPRITEEXT_H_

#include "cocos2d.h"
#include "Box2d/Box2d.h"
#include <functional>

USING_NS_CC;

class SpriteExt: public Sprite {
public:
	SpriteExt();
	virtual ~SpriteExt();
	static SpriteExt* create(const std::string& filename);
	static SpriteExt* createWithSpriteFrameName(const std::string& spriteFrameName);
	static SpriteExt* createWithSpriteFrame(SpriteFrame *spriteFrame);
	virtual void setPosition(const Vec2 & pos);
	virtual void setRepositionCallback(std::function<void(Vec2)> func);
	virtual void scaleToSize(Size desired);
	virtual void scaleXtoSize(float desired);
	virtual void scaleYtoSize(float desired);
	virtual void draw(Renderer *renderer, const Mat4& parentTransform, uint32_t flags);
	virtual void setShadowSprite(SpriteExt* s);

	//=================

	//function ptr for setPosition method
	std::function<void(Vec2)> setPosFunc;

	b2Body* body;
	SpriteExt* shadow;
};

#endif /* SPRITEEXT_H_ */
