/*Data Manager:
 * Singleton to provide data management and utility
 * note: some of this may not be used
 * © Zach Moore 2015
 * */

#ifndef DATA_H_
#define DATA_H_

#include "cocos2d.h"
#include "spriteExt.h"
#include "GameData.h"
#include <cstring>
#include <vector>

using namespace std;
USING_NS_CC;

//singleton class
class DataMan {
public:
	static DataMan& instance();

	void init();// call after setting resource dirs
	void addResourceDir(string dir);
	void loadSpriteSheet(string pList, string texture);
	SpriteExt* getSprite(string name);

	inline Size screenSize(){ return screensSize; }
	inline Vec2 screenOrigin(){ return screensOrigin; }
	inline Vec2 screenCenter(){return screensCenter; }
	void updateScreenData();
	inline GameData* Data(){ return &gameData; }

	inline unsigned char getDifficulty(){return difficulty;}
	inline void setDifficulty(unsigned char d){difficulty = d;}

	static void fixVec(Vec2 &v);
	static void fixSize(Size &s);

	static void pauseNodeRecursive(Node* n);
	static void resumeNodeRecursive(Node* n);

private:
	Size screensSize;
	Vec2 screensOrigin;
	Vec2 screensCenter;
	bool pathsSet;
	vector<string> searchPaths;
	GameData gameData;
	unsigned char difficulty;
	DataMan();
};

#endif /* DATA_H_ */
