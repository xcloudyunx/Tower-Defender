#ifndef __LASERBEAMER_H__
#define __LASERBEAMER_H__

#include "cocos2d.h"

#include "Turret.h"
#include "Player.h"

using namespace cocos2d;

class LaserBeamer : public Turret {
public:
	const static int Cost = 500;
	const static int UpgradeCost = 750;
	const static int SellCost = 250;
	const static int UpgradedSellCost = 625;
	virtual int getCost();
	virtual int getUpgradeCost();
	virtual int getSellCost();
	virtual int getUpgradedSellCost();

	static LaserBeamer* create();
	
	virtual bool init();
	
	virtual void upgrade();
	virtual void sell();
	
	virtual void update(float dt);
	
	virtual Sprite* getLaser();
	void setLaserDamage(const float d);
	float getLaserDamage();
	void setLaserSlow(const float s);
	float getLaserSlow();
	
private:
	Sprite* _laser;
	float _laserSlow;
	float _laserDamage;
};

#endif // __LASERBEAMER_H__
