#ifndef __BULLET_H__
#define __BULLET_H__

#define DEGREES (180/3.14159265)

#include "cocos2d.h"

#include "Enemy.h"
#include "Player.h"

using namespace cocos2d;

class Bullet : public Sprite {
public:
	static Bullet* create(Enemy* e);
	
	virtual bool init(Enemy* e);
	
	virtual void update(float dt);
	
	
	virtual void missTarget();
	virtual void hitTarget();
	
	void setTarget(Enemy* t);
	Enemy* getTarget();
	void setDamage(const float d);
	float getDamage();
	void setSpeed(const float &s);
	float getSpeed();
	
private:
	Enemy* _target;
	
	float _damage;
	float _speed;
};

#endif // __BULLET_H__
