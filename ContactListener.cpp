/* Contact Listener:
 * Subclass of box2ds contact listner
 * -handle collision in the box2d world
 * © Zach Moore 2015
 * */

#include "contactlistener.h"
#include "phyworld.h"

void ContactListener::BeginContact(b2Contact* contact)
{
	//figure out which objects collided and what type of entity

	EntUserData* adata = (EntUserData*)contact->GetFixtureA()->GetBody()->GetUserData();
	EntUserData* bdata = (EntUserData*)contact->GetFixtureB()->GetBody()->GetUserData();
	if(!adata || !bdata)
	{
		//log("no data");
		return;
	}
	Entity* a = adata->e;
	Entity* b = bdata->e;

	Entity* ballEnt;
	Entity* target = nullptr;

	if(a->tag == BallEntity::id)
	{
		ballEnt = a;
		target = b;
	}
	else if(b->tag == BallEntity::id)
	{
		ballEnt = b;
		target = a;
	}
	else
	{
		return;
	}

	//add the entity's collision function to the callbacks list for the next iteration
	PhyWorld::instance()->addPreStepFunction(CC_CALLBACK_1(Entity::collision, target),ballEnt);

}


