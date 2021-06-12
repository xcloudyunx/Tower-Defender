#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

using namespace cocos2d;

class Player : public Ref {
public:
	static void Reset();
	static Player* getInstance();
	
	bool init();
	
	void setMoney(const int m);
	int getMoney();
	void setLives(const int l);
	int getLives();
	int getStartLives();
	void setRound(const int r);
	int getRound();
	
	void setPaused(const bool p);
	bool isPaused();
	
	void setGameOver(const bool g);
	bool isGameOver();
	
	void setEndless(const bool e);
	bool isEndless();
	
private:
	int _money;
	int _startLives;
	int _lives;
	int _rounds;
	bool _isPaused;
	bool _isGameOver;
	bool _isEndless;
};

#endif // __PLAYER_H__
