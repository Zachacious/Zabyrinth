/* Random Maze Generator:
 * randomly generate and draw maze data and create and position entities
 * © Zach Moore 2015
 * */

#include "Maze.h"
#include "background.h"
#include "phyworld.h"
#include "randomnumgen.h"
#include "EndScene.h"

bool Maze::finished = false;


//each maze level is init with a diff level
Maze::Maze(unsigned char diff)
:/*b2DebugDraw(0),*/ballRadius(0),
 ballRadSquared(0),
 dataMan(0),
 ball(nullptr),
 epSteps(0),
cam(nullptr),
holesClip(nullptr),
holesStencil(nullptr),
osPerc(0),
holePerc(0),
obsticalPerc(0)
{

	//maze size and number of holes and blk balls set based on difficulty

	RandomIntGenerator rig;

	if(diff == 0) //very easy
	{
		int s = rig.ri(3,4);
		size.width = s; size.height = s;
		osPerc = 0;
		holePerc = 0.2f;
		obsticalPerc = 0;
	}
	else if(diff == 1)
	{
		int s = rig.ri(5,6);
		size.width = s; size.height = s;
		osPerc = 0.3f;//(float)rig.ri(40,50)/(float)100;
		holePerc = 0.3f;
		obsticalPerc = 0.1f;
	}
	else if(diff == 2)
	{
		int s = rig.ri(7,8);
		size.width = s; size.height = s;
		osPerc = (float)rig.ri(30,40)/(float)100.f;
		holePerc = 0.4f;
		obsticalPerc = 0.3f;
	}
	else if(diff == 3)
	{
		int s = rig.ri(9,10);
		size.width = s; size.height = s;
		osPerc = 0.2f;
		holePerc = (float)rig.ri(40,50)/(float)100.f;
		obsticalPerc = 0.4f;
	}
	else if(diff == 4) //insane
	{
		int s = 11;
		size.width = s; size.height = s;
		osPerc = 0;
		holePerc = rig.ri(50,60)/(float)100.f;
		obsticalPerc = 0.5f;
	}

	finished = false;

	//set mazedata size to accomidate size of maze
	mazeData.resize((int)size.width);
		for(int i = 0; i < (int)size.width; i++)
			mazeData[i].resize((int)size.height);

}

Maze::~Maze() {

	Device::setAccelerometerEnabled(false);
	if(cam){ cam->release(); }
	//delete b2DebugDraw;

	//release rogue entities: prevents mem leak
	//rogue entities are entities that couldn't be released earlier
	for(Entity* e : rogueEntities)
	{
		if(e)
		{
			e->release();
		}
	}

	if(holesClip)
	{
		holesClip->release();
	}
	if(holesStencil)
	{
		holesStencil->release();
	}
	if(ball)
	{
		ball->release();
	}

}

Maze* Maze::create(unsigned char diff)
{
	Maze* ret = new Maze(diff);
	if(ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		delete ret;
		ret = 0;
		return ret;
	}
}

bool Maze::init()
{
	if(!Node::init())
	{
		return false;
	}

	dataMan = &DataMan::instance();//Short hand could be confusing

	PhyWorld::instance()->reset(); //reset physics world;

	//BOX2D DEBUG STUFF
//	b2DebugDraw = new GLESDebugDraw( PTMRatio );
//	PWorld->SetDebugDraw(b2DebugDraw);
//
//	uint32 flags = 0;
//	flags += b2Draw::e_shapeBit;
////	flags += b2Draw::e_jointBit;
////	flags += b2Draw::e_aabbBit;
////	flags += b2Draw::e_pairBit;
//	flags += b2Draw::e_centerOfMassBit;
//	b2DebugDraw->SetFlags(flags);

		dataMan->loadSpriteSheet("maze.plist","maze.png");
		auto wall = dataMan->getSprite("wall.png");
		cellSize = wall->getContentSize();
		halfCellSize = cellSize/2.f;
		actualSize = Size(cellSize.width*size.width,cellSize.height*size.height);

		ball = BallEntity::create(this);
		ball->retain();
		ballSize = ball->getContentSize();


	//set callback to handle accelerometer events
	Device::setAccelerometerEnabled(true);
	Device::setAccelerometerInterval(30.0f);
	auto accListner = EventListenerAcceleration::create(CC_CALLBACK_2(Maze::onAcceleration, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(accListner, this);

	//create camera
	cam = NodeCamera::create(this);
	cam->retain();
	cam->setZoom(2.0f);
	//cam->enableTouchMove();
	addChild(cam,-1);

	//generate maze data
	genMaze();

	scheduleUpdate(); //make cocos call update function each iteration
	return true;
}

void Maze::genMaze()
{
	//initOpenSpaceObjs();
	//random starting point
	RandomIntGenerator randW;
	//RandomIntGenerator randH(0,size.height-2);

	//pick random starting point
	startPoint = Vec2(randW.ri(0,size.width-1),randW.ri(0,size.height-1));

	//setup initial cell
	//log("start: x: %f, y: %f",startPoint.x,startPoint.y);
	mazeData[startPoint.x][startPoint.y].visited = true;

	//recursivley dig from this cell
	dig(&mazeData[startPoint.x][startPoint.y],startPoint);

	//after maze is dug ... draw maze
	drawMaze();
	//setup ball
	ball->setPosition(Vec2(startPoint.x*cellSize.width+halfCellSize.width,
							startPoint.y*cellSize.height+halfCellSize.height));

	initEntities();

	// center camera over ball
	if(cam)
	{
		cam->setPosition(ball->worldCoords(ball->getPosition()));
		ball->setCamera(cam);
	}

}

void Maze::dig(Cell* c, Vec2 pos) // the dirty work of making the maze
{

	if(!c) //if cell pointer, bad return //this will occur when dig has backtracked to the origional start point cell: DONE!
		return;

	c->pos = pos; //set the position of the cell //maze pos not world  coords

	//if the cell has at least one undug direction,
	//dig that direction then run dig on the new cell ensuring that the
	//direction to be dug is not past the mazes borders.

	/*
	 * to begin with, each cell has rwall and dwall set to true meaning that the maze
	 * is made up of n*n closed off cells. each cells top wall being the cell above it's
	 * dwall(bottom). each cells left wall being the cell left of it's rwall.
	 * each time a cell has dig run on it and has an un-traveled direction one of the walls for that cell
	 * or the cell relatively above or left of it is set to false(removed) depending on the direction.
	 * the happens recursivley starting from the start point until every cell has been touch once.
	 * -Dead ends (end points) are kept track of and then the finish tile of the maze is determined by
	 * the endpoint that is farthest from the start point.
	 * NOTE: the maze also contains a set of cells that run across the top and down the left side to seal in
	 * the maze's top and left border. These cells are not reflected in mazeData but are only drawn.
	 * */

	unsigned char dir;
	if(c->hasDirections())
	{
		//log("past hasdirs");
		dir = c->directions.back();
		if(dir == NORTH && pos.y + 1 < size.height)
		{
			Cell* n = &mazeData[pos.x][pos.y+1]; //get the next cell
			if(!n->visited)
			{
				n->prevCell = c;
				n->prev = pos;
				n->visited = true;
				n->dWall = false;
				c->directions.pop_back(); //remove direction from the list
				epSteps++;
				//log("n pos: %f, %f",pos.x,pos.y+1);
				dig(n,Vec2(pos.x,pos.y+1)); //dig the north cell
			}
			else
			{
				//if north cell has already been visited remove that direction from this cells
				//list and try again.
				c->directions.pop_back();
				dig(c,pos);
			}
		}
		else if(dir == SOUTH && pos.y - 1 >=0)
		{
			Cell* n = &mazeData[pos.x][pos.y-1];
			if(!n->visited)
			{
				n->prevCell = c;
				n->prev = pos;
				n->visited = true;
				c->dWall = false;
				c->directions.pop_back();
				epSteps++;
				//log("s pos: %f, %f",pos.x,pos.y-1);
				dig(n,Vec2(pos.x,pos.y-1));
			}
			else
			{
				c->directions.pop_back();
				dig(c,pos);
			}
		}
		else if(dir == EAST && pos.x + 1 < size.width)
		{
			Cell* n = &mazeData[pos.x+1][pos.y];
			if(!n->visited)
			{
				n->prevCell = c;
				n->prev = pos;
				n->visited = true;
				c->rWall = false;
				c->directions.pop_back();
				epSteps++;
				//log("e pos: %f, %f",pos.x+1,pos.y);
				dig(n,Vec2(pos.x+1,pos.y));
			}
			else
			{
				c->directions.pop_back();
				dig(c,pos);
			}
		}
		else if(dir == WEST && pos.x - 1 >=0)
		{
			Cell* n = &mazeData[pos.x-1][pos.y];
			if(!n->visited)
			{
				n->prevCell = c;
				n->prev = pos;
				n->visited = true;
				n->rWall = false;
				c->directions.pop_back();
				epSteps++;
				//log("w pos: %f, %f",pos.x-1,pos.y);
				dig(n,Vec2(pos.x-1,pos.y));
			}
			else
			{
				c->directions.pop_back();
				dig(c,pos);
			}
		}
		else
		{
			//dig location is out of bounds
			c->directions.pop_back();
			dig(c,pos);
		}
	}
	else
	{

		//determine that cell is endpoint
		int wallcount = 0;
		wallcount += mazeData[pos.x-1][pos.y].rWall ? 1:0;
		wallcount += mazeData[pos.x][pos.y].rWall ? 1:0;
		wallcount += mazeData[pos.x][pos.y+1].dWall ? 1:0;
		wallcount += mazeData[pos.x][pos.y].dWall ? 1:0;
		if(wallcount==3 && pos != startPoint)
		{
			//found end point
			endPoints.push_back(c);
			endPointSteps.push_back(epSteps); //keep track of how far the end point is from the start

		}
		epSteps--;
//		log("prev cell");
		dig(c->prevCell,c->prev);
	}

}

void Maze::drawMaze()
{
	//create and scale bk image
	SpriteExt* bk = DataMan::instance().getSprite("woodBk2.jpg");
	Size mazeSize = Size((size.width)*cellSize.width,(size.height)*cellSize.height);
	Vec2 mazeCenter = (Vec2)(mazeSize/2.f);
	bk->scaleToSize(mazeSize);
	bk->setPosition(mazeCenter);
	addChild(bk,0);

	SpriteExt* boardBk = DataMan::instance().getSprite("bk.jpg");
	boardBk->scaleToSize(mazeSize);
	boardBk->setPosition(mazeCenter);
	//addChild(boardBk,0);

	//holes clipping node -- use stencil node to create actual holes
	holesClip = ClippingNode::create();
	holesStencil = Node::create();
	holesClip->setStencil(holesStencil);
	holesClip->setInverted(true);
	holesClip->setAlphaThreshold( 0.05f );
	holesClip->addChild(boardBk,0);
	addChild(holesClip,1);

	holesClip->retain();
	holesStencil->retain();

	//iterate through mazeData and draw each cells walls + the top and left border walls
	Vec2 realPosOffset = Vec2(halfCellSize.width,halfCellSize.height);

	for(int x = 0; x < (int)size.width; x++)
	{
		for(int y = 0; y < (int)size.height; y++)
		{

			//handle walls
			Cell curCell = mazeData[x][y];
			if(curCell.rWall)
			{
				WallEntity* g = WallEntity::create(this);
				g->setPosition(realPosOffset);

			}
			if(curCell.dWall)
			{
				WallEntity* g = WallEntity::create(this);
				g->setPosition(realPosOffset);
				g->setRotation(90.0f);
			}
			if(!curCell.rWall && !curCell.dWall && !curCell.noCorner)
			{
				CornerEntity* g = CornerEntity::create(this);
				g->setPosition(realPosOffset);
			}
			////////////////////
			realPosOffset.y += cellSize.height;
		}

		realPosOffset.x += cellSize.width;
		realPosOffset.y = halfCellSize.height;
	}

	//west and east border wall
	float westOffset = -1.f*(cellSize.height/2.0f);
	float eastOffset = size.width - 1.f;
	float yOffset = halfCellSize.height;
	for(int wey = 0; wey < (int)size.height; wey++)
	{
		//west
		WallEntity* w = WallEntity::create(this);
		w->setPosition(Vec2(westOffset,yOffset));

		yOffset += cellSize.height;
	}

	//north border wall
	float hOffset = halfCellSize.height+((size.height)*cellSize.height);
	float xOffset = halfCellSize.width;
	for(int nx = 0; nx < (int)size.width; nx++)
	{
		WallEntity* n = WallEntity::create(this);
		n->setPosition(Vec2(xOffset,hOffset));
		n->setRotation(90.0f);
		xOffset += cellSize.width;
	}
	//put a corner piece in north west corner border
	CornerEntity* nwc = CornerEntity::create(this);
	nwc->setPosition(Vec2(-halfCellSize.width,hOffset));
}

void Maze::initEntities()
{
	RandomIntGenerator rig;
	//get exit point

	Cell* epc;
	float maxSteps = 0;
	for(int epi =0; epi < endPoints.size(); epi++)
	{
		if(endPointSteps[epi]>maxSteps)
		{
			epc = endPoints[epi];
			maxSteps = endPointSteps[epi];
		}
	}

	exitPoint = epc->pos;

	ExitEntity * ee = ExitEntity::create(this);
	ee->setPosition(getRealCellCoords(exitPoint));

	//holes
	int l = size.width;
	float constFactor = holePerc;// 0.35f;
	float factor = (l/100.f)+constFactor;
	float perc = factor*l;
	int numHoles = (int)(perc*(l*2.2f));

	for(int i = 0; i < numHoles; i++)
	{
		HoleEntity* h = HoleEntity::create(this);
		//get free pos
		Vec2 p = getNextHolePos(h->getContentSize());
		//set pos
		h->setPosition(p);
		h->retain();
		rogueEntities.push_back(h);
	}

	//black balls
	float obstPerc = obsticalPerc;
	float obstFactor = (l*l)*obstPerc;
	//25% obsticals = blackballs
	float numBBalls = obstFactor * 0.25f;
	for(int bb = 0; bb < (int)numBBalls; bb++)
	{
		blkBallEntity* blkball = blkBallEntity::create(this);
		blkball->retain();
		rogueEntities.push_back(blkball);
		posBlkBall(blkball);

	}

void Maze::posBlkBall(Entity* ent)
{
	RandomIntGenerator ran;
	Vec2 tpos = Vec2(ran.ri(0,size.width-1),ran.ri(0,size.height-1));
	if(tpos != startPoint && tpos != exitPoint)
	{
		ent->setPosition(getRealCellCoords(tpos));
		return;
	}

	posBlkBall(ent);
}

Vec2 Maze::getNextHolePos(const Size &holeSize)
{
	RandomIntGenerator ran;
	Vec2 tpos = Vec2(ran.ri(0,size.width-1),ran.ri(0,size.height-1));
	if(tpos != startPoint && tpos != exitPoint)
	{
		Cell* c = &mazeData[tpos.x][tpos.y];
		char holeNum = c->getRandomFreeHole();
		if(holeNum != -1)
		{
			if(holeNum == Cell::centerHole)
			{
				return getRealCellCoords(c->pos);
			}

			Vec2 normal = Vec2::ZERO;
			c->hole[holeNum] = true;
			if(holeNum == Cell::topLeftHole)
			{normal = Vec2(-1,1);}
			else if(holeNum == Cell::topRightHole)
			{normal = Vec2(1,1);}
			else if(holeNum == Cell::bottomRightHole)
			{normal = Vec2(1,-1);}
			else if(holeNum == Cell::bottomLeftHole)
			{normal = Vec2(-1,-1);}

			Size halfHoleSize = holeSize/1.5f;
			Vec2 offset = Vec2(normal.x*halfHoleSize.width,normal.y*halfHoleSize.height);
			return getRealCellCoords(c->pos) + offset;
		}
	}

	return getNextHolePos(holeSize);
}

void Maze::resetMaze() //remove ball from hole and reposition at start-- restart maze
{
	if(!ball)
		return;
	reorderChild(ball->shadow,4);
	reorderChild(ball->sprite,4);
	ball->sprite->setScale(1.f);
	ball->shadow->setScale(1.f);
	ball->sprite->setVisible(true);
	ball->shadow->setVisible(true);

	auto func = CallFunc::create(
	[this]()
	{
		ball->setPosition(getRealCellCoords(startPoint));
		ball->setEnabled(true);
	}
	);

	auto seq = Sequence::create(MoveTo::create(1.25f,getRealCellCoords(startPoint)),func,nullptr);

	ball->sprite->runAction(MoveTo::create(1.f,getRealCellCoords(startPoint)));
	ball->shadow->runAction(seq);

	Maze::finished = false;

}


void Maze::update(float dt)
{
	PhyWorld::instance()->update(dt);

}

//When device is tilted use accelerometer data to change box2d gravity to move dynamic entities
void Maze::onAcceleration(Acceleration* acc, Event* event)
{
	if(!ball)
		return;
	b2Vec2 gravity( acc->x * 20.f,
				    acc->y * 20.f );

	PWorld->SetGravity( gravity );

}

Vec2 Maze::getRealCellCoords(Vec2 pos)
{
	float x = halfCellSize.width + (pos.x * cellSize.width);
	float y = halfCellSize.height + (pos.y * cellSize.height);
	return Vec2(x,y);
}

Vec2 Maze::getCellMazeCoords(Vec2 pos)
{
	float x = (pos.x - halfCellSize.width) / cellSize.width;
	float y = (pos.y - halfCellSize.height) / cellSize.height;
	return Vec2(x,y);
}

//////////////////////////////////////////
//mazePlayer class to load and start the maze level in cocos
MazePlayer::MazePlayer(unsigned char diff)
:maze(nullptr)/*,cam(nullptr)*/
{
	maze =Maze::create(diff);
	auto bk = bkLayer::create();
	maze->retain();
	addChild(bk,1);
	addChild(maze,2);

}

MazePlayer::~MazePlayer()
{
	maze->release();
}

MazePlayer* MazePlayer::create(unsigned char diff)
{
	MazePlayer* ret = new MazePlayer(diff);
	if(ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		delete ret;
		ret =0;
		return ret;
	}
}

void MazePlayer::exec()
{
	auto trans = TransitionFade::create(2.0f, this);
	Director::getInstance()->replaceScene(trans);
}
