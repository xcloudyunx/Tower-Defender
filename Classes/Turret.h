#ifndef __TURRET_H__
#define __TURRET_H__

#define DEGREES (180/3.14159265)

#include "cocos2d.h"

#include "EnemyManager.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"

using namespace cocos2d;

class Turret : public Sprite {
public:
	const static int Cost = 200;
	const static int UpgradeCost = 300;
	const static int SellCost = 100;
	const static int UpgradedSellCost = 250;
	virtual int getCost();
	virtual int getUpgradeCost();
	virtual int getSellCost();
	virtual int getUpgradedSellCost();

	static Turret* create();
	
	virtual bool init();
	
	virtual void upgrade();
	void setUpgraded(const bool u);
	bool isUpgraded();
	virtual void sell();
	
	virtual void update(float dt);
	virtual void updateTarget(float dt);
	
	void lockTarget(float dt);
	virtual void shoot();
	virtual Bullet* getBullet();
	Vec2 getBulletSpawnPosition();
	
	void setHead(Sprite* h);
	Sprite* getHead();
	void setTarget(Enemy* e);
	Enemy* getTarget();
	void setRange(const float &r);
	float getRange();
	void setFireRate(const float &f);
	float getFireRate();
	void setFireCountdown(const float &f);
	float getFireCountdown();
	void setOffset(const Vec2 o);
	Vec2 getOffset();
	void setBulletOffset(const Vec2 o);
	Vec2 getBulletOffset();
	
	void setPosition(const Vec2 pos);
	Vec2 getPosition();
	void setTruePosition(const Vec2 pos);
	Vec2 getTruePosition();
	
private:
	Sprite* _head;
	Vec2 _offset;
	Vec2 _bulletOffset;
	
	Enemy* _target = nullptr;
	
	float _range;
	float _fireRate;
	float _fireCountdown = 0.0f;
	
	bool _isUpgraded;
};

#endif // __TURRET_H__
