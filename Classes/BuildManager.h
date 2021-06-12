#ifndef __BUILD_MANAGER_H__
#define __BUILD_MANAGER_H__

#include "cocos2d.h"

#include "Player.h"

#include "Turret.h"
#include "MissileLauncher.h"
#include "LaserBeamer.h"

using namespace cocos2d;

class BuildManager : public Ref {
public:
	static void Reset();
	static BuildManager* getInstance();
	
	bool init();
	
	void setTurretToBuild(const int t);
	int getTurretToBuild();
	
	void setTurret(Turret* t);
	Turret* getTurret();
	
	Turret* buildTurret();
	void upgradeTurret(const int cost);
	void sellTurret(const int cost);
	
private:
	Player* _player;
	
	int _turretToBuild;
	Turret* _turretSelected;
};

#endif // __BUILD_MANAGER_H__
