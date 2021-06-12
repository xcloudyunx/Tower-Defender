#ifndef __MISSILELAUNCHER_H__
#define __MISSILELAUNCHER_H__

#include "cocos2d.h"

#include "Turret.h"

#include "Missile.h"

#include "Player.h"

using namespace cocos2d;

class MissileLauncher : public Turret {
public:
	const static int Cost = 1000;
	const static int UpgradeCost = 1400;
	const static int SellCost = 500;
	const static int UpgradedSellCost = 1200;
	virtual int getCost();
	virtual int getUpgradeCost();
	virtual int getSellCost();
	virtual int getUpgradedSellCost();

	static MissileLauncher* create();
	
	virtual bool init();
	
	virtual void upgrade();
	
	virtual void update(float dt);
	
	virtual void shoot();
	
	virtual Bullet* getBullet();
	
	Sprite* getPlaceholderMissile();
	
private:
	Sprite* _missile;
	Sprite* _missile2;
	
	std::queue<Sprite*> _coolDown;
};

#endif // __MISSILELAUNCHER_H__
