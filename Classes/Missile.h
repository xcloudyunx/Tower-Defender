#ifndef __MISSILE_H__
#define __MISSILE_H__

#include "cocos2d.h"

#include "Bullet.h"

#include "EnemyManager.h"
#include "Enemy.h"

using namespace cocos2d;

class Missile : public Bullet {
public:
	static Missile* create(Enemy* e, const bool u);
	
	virtual bool init(Enemy* e, const bool u);
	
	void setRadius(const float r);
	float getRadius();
	
	virtual void missTarget();
	virtual void hitTarget();
	
private:
	float _radius;

};

#endif // __MISSILE_H__
