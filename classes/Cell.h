/*Maze Cell Structure:
 * This structure holds data for individual cells of the maze
 * -each cell has a right wall (rWall) and a down wall (dWall)
 * each cell can have both, either or neither.
 * -each cell contains a pointer to the previous cell
 * -each cell contains a list of possible directions to "dig" next.
 * as each direction is dig it is removed from the list.
 * -contains 5 possible areas to have a hole. can have 0-5 holes per cell.
 * © Zach Moore 2015
 * */

#ifndef CELL_H_
#define CELL_H_

#include "cocos2d.h"
#include "PhyEntities.h"
#include "randomNumgen.h"
#include <vector>
#include <algorithm>
USING_NS_CC;
using namespace std;

struct Cell
{
	enum holePos
	{
		topLeftHole,
		bottomLeftHole,
		topRightHole,
		bottomRightHole,
		centerHole
	};

	bool visited,rWall,dWall;
	unsigned char n,s,e,w;
	Vec2 prev;
	Cell* prevCell;
	vector<unsigned char> directions;
	void* userData;
	Vec2 pos;
	bool hole[5];
	bool noCorner;

	Cell()
	:visited(false),
	 rWall(true),
	 dWall(true),
	 prevCell(nullptr),
	 userData(nullptr),
	 noCorner(false)
	{
		//randomize the order of directions for the cell
		RandomIntGenerator randD;
		int factor = randD.ri(0,3);
		n = 0 + factor;
		s = 1+factor>3 ? 1 : 1+factor;
		e = 2+factor>3 ? (2+factor)-4 : 2+factor;
		w = 3+factor>3 ? (3+factor)-4 : 3+factor;
		directions.push_back(0);
		directions.push_back(1);
		directions.push_back(2);
		directions.push_back(3);
		random_shuffle(directions.begin(),directions.end());

		hole[centerHole] = false; hole[topLeftHole] = false;
		hole[topRightHole] = false; hole[bottomLeftHole]=false; hole[bottomRightHole] = false;
	}

	bool hasDirections()
	{
		//log("num dirs: %i",directions.size());
		return directions.size() > 0;
	}

	char getRandomFreeHole() //if cell has less than 5 holes find a free one at random
	{
		RandomIntGenerator ran;
		char i = (char)ran.ri(topLeftHole,centerHole);
		if(!hole[i])
		{
			return i;
		}
		else
		{
			for(char ii = 0; ii<5; ii++)
			{
				if((i+ii)>centerHole)
				{
					i = topLeftHole;
				}
				if(!hole[i])
				{
					return i;
				}
			}
		}

		return -1;
	}
};


#endif /* CELL_H_ */
