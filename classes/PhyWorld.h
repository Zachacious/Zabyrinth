/* b2d physics world:
 * handle creating and updating physics as well as running collision callbacks
 * © Zach Moore 2015
 * */

#ifndef PHYWORLD_H_
#define PHYWORLD_H_

#include "Box2D/Box2D.h"
#include "contactListener.h"
#include <vector>
#include <functional>

typedef std::function<void(void*)> voidFunction;

#define PWorld PhyWorld::instance()->w
#define PTMRatio 32
#define DEG_TO_RAD 0.0174532925f
#define RAD_TO_DEG 57.2957795f

class PhyWorld
{

	ContactListener* contactHandler;
	std::vector<voidFunction> preStepFunctions;
	std::vector<void*>preStepFuncData;

	PhyWorld()
	:w(0),contactHandler(0)
	{
		reset();
	}

public:
	~PhyWorld()
	{
		if(w)
			delete w;

		if(contactHandler)
			delete contactHandler;
	}

	static PhyWorld* instance()
	{
		static PhyWorld * singleton = new PhyWorld();
		return singleton;
	}

	void update(float dt) //update the box2d physics world -- run once per iteration
	{
		//performance vs accuracy
		int velocityIterations = 8;
		int positionIterations = 1;

		w->Step(0.016666667f, velocityIterations, positionIterations);

		//run stored up collision functions
		if(preStepFunctions.size()>0)
		{
			for(voidFunction f : preStepFunctions)
			{
				voidFunction func = preStepFunctions.back();
				func(preStepFuncData.back());
				preStepFunctions.pop_back();
				preStepFuncData.pop_back();
			}
		}

	}

	void reset()
	{
		if(w)
			delete w;

		w = new b2World(b2Vec2(0,0));
		w->SetAllowSleeping(true);

		if(contactHandler)
			delete contactHandler;

		contactHandler = new ContactListener;
		w->SetContactListener(contactHandler);
	}

	void addPreStepFunction(voidFunction func,void* data)
	{
		preStepFunctions.push_back(func);
		preStepFuncData.push_back(data);
	}

	b2World* w;
};



#endif /* PHYWORLD_H_ */
