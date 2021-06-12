#ifndef __ENEMY_MANAGER_H__
#define __ENEMY_MANAGER_H__

#include "cocos2d.h"

#include "Player.h"
#include "Enemy.h"
#include "Wave.h"

using namespace cocos2d;

class EnemyManager : public Node {
public:
	static void Reset();
	static EnemyManager* getInstance();
	
	bool init();
	
	std::list<Enemy*>* getEnemies();
	std::vector<Sprite*>* getPath();
	
	void update(float dt);
	void updateEnemies(float dt);
	
	void spawnWave();
	void spawnEnemy(const int e);
	
	void addWaves();
	void generateWave(const int round);
	
private:
	Player* _player;

	std::list<Enemy*>* _enemies;
	std::vector<Sprite*>* _path;
	
	std::vector<Wave*> _waves;
	unsigned int _waveIndex;
	unsigned int _counter;
	bool _finishedRound = true;
	
	float _waitTime;
	float _countdown;
};

#endif // __ENEMY_MANAGER_H__
