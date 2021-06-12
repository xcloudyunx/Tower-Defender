#ifndef __ENEMY_H__
#define __ENEMY_H__

#define DEGREES (180/3.14159265)

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "Player.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class Enemy : public Sprite {
public:
	static Enemy* create(std::list<Enemy*>* &enemies, std::vector<Sprite*>* &path, const int n);
	
	virtual bool init(std::list<Enemy*>* &enemies, std::vector<Sprite*>* &path, const int n);
	
	virtual void update(float dt);
	
	void setPosition(const Vec2 pos);
	Vec2 getPosition();
	Vec2 getTruePosition();
	
	void getNextTarget();
	int getIndex();
	
	void setStartingHealth(const float h);
	float getStartingHealth();
	void setHealth(const float h);
	float getHealth();
	void setValue(const int v);
	int getValue();
	void setSpeed(const float s);
	float getSpeed();
	void setSlow(const float s);
	float getSlow();
	
	void slow(const float s);
	void damage(const float d);
	void die();
	
private:
	Player* _player;
	
	std::list<Enemy*>* _enemies;
	std::vector<Sprite*>* _path;
	
	unsigned int _currentPos;
	Sprite* _target;
	Vec2 _offset;
	
	float _startingHealth;
	float _health;
	int _value;
	float _speed;
	float _slow;
	
	Sprite* _healthBarBase;
	LoadingBar* _healthBar;
	
	bool _dead;
};

#endif // __ENEMY_H__
