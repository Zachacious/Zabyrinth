/* Random Maze Generator:
 * randomly generate and draw maze data and create and position entities
 * © Zach Moore 2015
 * */

#ifndef MAZE_H_
#define MAZE_H_

#include "cocos2d.h"
#include "Dataman.h"
#include "cell.h"
#include "phyEntities.h"
//#include "gles-render.h"
#include "OpenSpaceObj.h"
#include "Camera.h"
#include <vector>
#include <cmath>
#include <string>

#define NORTH 0
#define SOUTH 2
#define EAST 1
#define WEST 3

USING_NS_CC;

class Maze : public Node{
public:
	Maze(unsigned char diff);
	virtual ~Maze();
	static Maze* create(unsigned char diff);
	virtual bool init();
	void genMaze();
	void dig(Cell* c, Vec2 pos);
	void drawMaze();
	void update(float dt);
	void onAcceleration(Acceleration* acc, Event* event);
	Vec2 getRealCellCoords(Vec2 pos);
	Vec2 getCellMazeCoords(Vec2 pos);
	inline BallEntity* getBallEntity(){ return ball; }
	inline void setCamera(NodeCamera* c){ cam = c; }
	NodeCamera* getCamera(){ return cam; }
	void initEntities();
	Vec2 getNextHolePos(const Size &holeSize);
	void resetMaze();
	void posBlkBall(Entity* ent);
	//////////////////////////////////////////////////////////////////
	//DEBUG STUFF for box2d
	////////////////////////////
	//	GLESDebugDraw* b2DebugDraw;
	//	Mat4 mvMat;
//	CustomCommand _customCommand;
//	void draw(Renderer *renderer, const Mat4 &transform, bool transformUpdated)
//	{
//		Node::draw(renderer,transform,transformUpdated);
//		 GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
//		Director* director = Director::getInstance();
//		CCASSERT(nullptr != director, "Director is null when seting matrix stack");
//		director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//
//		mvMat = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//		_customCommand.init(_globalZOrder+100);
//		_customCommand.func = CC_CALLBACK_0(Maze::onDraw, this, transform, transformUpdated);
//		renderer->addCommand(&_customCommand);
//
//		director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//	}
//	void onDraw(const Mat4 &transform, bool transformUpdated)
//	{
//		Director* director = Director::getInstance();
//		CCASSERT(nullptr != director, "Director is null when seting matrix stack");
//		director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//		director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);
////			GameWorld* gw = GameWorld::instance();
////			for(int i= 0; i < gw->staticObjectCount(); ++i)
////			{
////				Rect r = gw->getStaticObject(i)->getBBox();
////				//DrawPrimitives::setDrawColor4B(255,0,0,255);
////				//glLineWidth( 2.0f );
////				if(gw->getStaticObject(i)->isCollided)
////					DrawPrimitives::drawSolidRect(r.origin,r.origin+r.size,Color4F(1.0f,0,0,0.5f));
////				else
////					DrawPrimitives::drawSolidRect(r.origin,r.origin+r.size,Color4F(1.0f,0,1.0f,0.5f));
////
////			}
////			Rect r = ball->getBBox();
////			DrawPrimitives::drawSolidRect(r.origin,r.origin+r.size,Color4F(1.0f,0,0,0.5f));
////
//		Mat4 oldMV;
//		oldMV = director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//		director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, mvMat);
//		PWorld->DrawDebugData();
//		director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, oldMV);
//
//		glLineWidth(1);
//		DrawPrimitives::setDrawColor4B(255,255,255,255);
//		DrawPrimitives::setPointSize(1);
//		director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//	}

	vector<Cell*> endPoints;
	vector<float> endPointSteps;
	vector<Entity*> rogueEntities;
	ClippingNode* holesClip;
	Node* holesStencil;

	static bool finished;

private:

	vector<vector<Cell>> mazeData; //actual randomly created maze data
	Size size;
	Size actualSize;
	Size cellSize;
	Size halfCellSize;
	Size bkTileSize;
	Size halfBkTileSize;
	int ballRadius;
	int ballRadSquared;
	Size ballSize;
	Size halfBallSize;
	Vec2 startPoint,exitPoint;
	DataMan* dataMan;
	BallEntity* ball;
	int epSteps;//end point step count
	NodeCamera* cam;
	float osPerc;
	float holePerc;
	float obsticalPerc;
};

//load maze scene with cocos
class MazePlayer: public Scene
{
public:
	MazePlayer(unsigned char diff);
	~MazePlayer();
	void exec();
	static MazePlayer* create(unsigned char diff);
	Maze* maze;
};

#endif /* MAZE_H_ */
