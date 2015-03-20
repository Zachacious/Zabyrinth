/* Contact Listener:
 * Subclass of box2ds contact listner
 * -handle collision in the box2d world
 * © Zach Moore 2015
 * */

#ifndef CONTACTLISTENER_H_
#define CONTACTLISTENER_H_

#include "PhyEntities.h"

class ContactListener: public b2ContactListener
{
public:

	void BeginContact(b2Contact* contact);

	void EndContact(b2Contact* contact){}
};


#endif /* CONTACTLISTENER_H_ */
