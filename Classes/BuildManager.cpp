#include "BuildManager.h"

static BuildManager* _instance = nullptr;

void BuildManager::Reset() {
	_instance = nullptr;
}

BuildManager* BuildManager::getInstance() {
	if (!_instance) {
		_instance = new (std::nothrow) BuildManager;
		_instance->init();
    }
	return _instance;
}

bool BuildManager::init() {
	_player = Player::getInstance();
	
	_turretToBuild = 0;
	_turretSelected = nullptr;
	
	return true;
}

void BuildManager::setTurretToBuild(const int t) {
	_turretToBuild = t;
}

int BuildManager::getTurretToBuild() {
	return _turretToBuild;
}

void BuildManager::setTurret(Turret* t) {
	_turretSelected = t;
}

Turret* BuildManager::getTurret() {
	return _turretSelected;
}

Turret* BuildManager::buildTurret() {
	if (!_turretToBuild) return nullptr;
	Turret* turret;
	if (_turretToBuild == 1) {
		turret = Turret::create();
	} else if (_turretToBuild == 2) {
		turret = LaserBeamer::create();
	} else if (_turretToBuild == 3) {
		turret = MissileLauncher::create();
	}
	_player->setMoney(_player->getMoney() - turret->getCost());
	return turret;
}

void BuildManager::upgradeTurret(const int cost) {
	_player->setMoney(_player->getMoney() - cost);
	_turretSelected->upgrade();
}

void BuildManager::sellTurret(const int cost) {
	_player->setMoney(_player->getMoney() + cost);
	_turretSelected->sell();
}