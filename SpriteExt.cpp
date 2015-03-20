/* Sprite Extended:
 * Subclass of cocos sprite to include a callback that runs when set position is called.
 * also to allow a shadow sprite.
 * © Zach Moore 2015
 * */

#include "SpriteExt.h"
#include "phyworld.h"

SpriteExt::SpriteExt()
:setPosFunc(nullptr), body(nullptr), shadow(nullptr)
{

}

SpriteExt::~SpriteExt() {
	if(shadow)
	{
		shadow->release();
	}
}

void SpriteExt::setPosition(const Vec2 & pos)
{
	Sprite::setPosition(pos);
	if(setPosFunc)
	{
		setPosFunc(pos);
	}
}

void SpriteExt::setRepositionCallback(std::function<void(Vec2)> func)
{
	setPosFunc = func;
}

SpriteExt* SpriteExt::create(const std::string& filename)
{
    SpriteExt *sprite = new (std::nothrow) SpriteExt();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

SpriteExt* SpriteExt::createWithSpriteFrameName(const std::string& spriteFrameName)
{
    SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);

//#if COCOS2D_DEBUG > 0
//    char msg[256] = {0};
//    sprintf(msg, "Invalid spriteFrameName: %s", spriteFrameName.c_str());
//    CCASSERT(frame != nullptr, msg);
//#endif

    return !frame ? nullptr : createWithSpriteFrame(frame);
}

SpriteExt* SpriteExt::createWithSpriteFrame(SpriteFrame *spriteFrame)
{
	SpriteExt *sprite = new (std::nothrow) SpriteExt();
    if (sprite && spriteFrame && sprite->initWithSpriteFrame(spriteFrame))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void SpriteExt::scaleToSize(Size desired)
{
	Size factor = Size(desired.width/Sprite::getContentSize().width,
			desired.height/Sprite::getContentSize().height);
	Sprite::setScale(factor.width,factor.height);
}

void SpriteExt::scaleXtoSize(float desired)
{
	float factor = desired/Sprite::getContentSize().width;
	Sprite::setScaleX(factor);
}

void SpriteExt::scaleYtoSize(float desired)
{
	float factor = desired/Sprite::getContentSize().height;
	Sprite::setScaleY(factor);
}

void SpriteExt::draw(Renderer *renderer, const Mat4& parentTransform, uint32_t flags)
{
	if(body && body->IsActive() && body->GetType() == b2_dynamicBody)
	{
		b2Vec2 pos = body->GetPosition();
		setPosition(Vec2(pos.x*PTMRatio,pos.y*PTMRatio));
		setRotation(CC_RADIANS_TO_DEGREES(body->GetAngle()));

		if(shadow)
		{
			shadow->setPosition(Vec2(pos.x*PTMRatio,pos.y*PTMRatio));
			//shadow->setRotation(CC_RADIANS_TO_DEGREES(body->GetAngle()));
		}

//		b2Vec2 pos  = body->GetPosition();
//
//		float x = pos.x * PTMRatio;
//		float y = pos.y * PTMRatio;
//
//		if (_ignoreAnchorPointForPosition)
//		{
//			x += _anchorPointInPoints.x;
//			y += _anchorPointInPoints.y;
//		}
//
//		// Make matrix
//		float radians = body->GetAngle();
//		float c = cosf(radians);
//		float s = sinf(radians);
//
//		if (!_anchorPointInPoints.equals(Vec2::ZERO))
//		{
//			x += ((c * -_anchorPointInPoints.x * _scaleX) + (-s * -_anchorPointInPoints.y * _scaleY));
//			y += ((s * -_anchorPointInPoints.x * _scaleX) + (c * -_anchorPointInPoints.y * _scaleY));
//		}
//
//		// Rot, Translate Matrix
//
//		float mat[] = {  (float)c * _scaleX, (float)s * _scaleX, 0,  0,
//			(float)-s * _scaleY, (float)c * _scaleY,  0,  0,
//			0,  0,  1,  0,
//			x,	y,  0,  1};
//
//		_transform.set(mat);

	}
	Sprite::draw(renderer,parentTransform,flags);
}

void SpriteExt::setShadowSprite(SpriteExt* s)
{
	if(s)
	{
		shadow = s;
		shadow->retain();
	}
}

