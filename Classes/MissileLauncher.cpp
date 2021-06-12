#include "MissileLauncher.h"

int MissileLauncher::getCost() {
	return MissileLauncher::Cost;
}

int MissileLauncher::getUpgradeCost() {
	return MissileLauncher::UpgradeCost;
}

int MissileLauncher::getSellCost() {
	return MissileLauncher::SellCost;
}

int MissileLauncher::getUpgradedSellCost() {
	return MissileLauncher::UpgradedSellCost;
}

MissileLauncher* MissileLauncher::create() {
	MissileLauncher *launcher = new (std::nothrow) MissileLauncher();
	if (launcher && launcher->init()) {
		launcher->autorelease();
		return launcher;
	}
	CC_SAFE_DELETE(launcher);
	return nullptr;
}

bool MissileLauncher::init() {
	if (!this->initWithFile("Turrets/missileLauncherBase.png")) {
		return false;
	}
	
	this->setLocalZOrder(10);
	
	this->setHead(Sprite::create("Turrets/missileLauncherHead.png"));
	
	_missile = Sprite::create("Bullets/missile.png");
	this->getPlaceholderMissile()->setPosition(Vec2(this->getHead()->getBoundingBox().size.width/2, this->getHead()->getBoundingBox().size.height*3/5));
	this->getHead()->addChild(this->getPlaceholderMissile());
	
	this->setOffset(Vec2(64, 64)); //will need to change
	this->setBulletOffset(Vec2(0, 0));
	
	this->setRange(720.0f);
	this->setFireRate(0.5f);
	
	this->setScale(0.1f);
	this->runAction(EaseBackOut::create(ScaleTo::create(0.1f, 1.0f)));
	
	this->schedule(schedule_selector(MissileLauncher::updateTarget), 0.1f);
	this->scheduleUpdate();
	
	return true;
}

void MissileLauncher::upgrade() {
	this->setHead(Sprite::create("Turrets/missileLauncherHeadUpgraded.png"));
	
	_missile = Sprite::create("Bullets/missileUpgraded.png");
	_missile->setPosition(Vec2(this->getHead()->getBoundingBox().size.width*3/8, this->getHead()->getBoundingBox().size.height*3/5));
	this->getHead()->addChild(_missile);
	_missile2 = Sprite::create("Bullets/missileUpgraded.png");
	_missile2->setPosition(Vec2(this->getHead()->getBoundingBox().size.width*5/8, this->getHead()->getBoundingBox().size.height*3/5));
	this->getHead()->addChild(_missile2);
	while (!_coolDown.empty()) _coolDown.pop();
	
	this->setBulletOffset(Vec2(-this->getHead()->getBoundingBox().size.width/8, 0));
	
	this->setFireRate(2*this->getFireRate());
	
	this->setScale(0.1f);
	this->runAction(EaseBackOut::create(ScaleTo::create(0.1f, 1.0f)));
	
	this->setFireCountdown(0.0f);
	
	this->setUpgraded(true);
}

void MissileLauncher::update(float dt) {
	if (Player::getInstance()->isPaused()) return;
	
	Turret::update(dt);
	if (this->getFireCountdown() <= 0) {
		if (!_coolDown.empty()) {
			_coolDown.front()->setVisible(true);
			_coolDown.pop();
		}
	}
}

void MissileLauncher::shoot() {
	Turret::shoot();
	this->getPlaceholderMissile()->setVisible(false);
	_coolDown.push(this->getPlaceholderMissile());
	if (this->isUpgraded()) {
		std::swap(_missile, _missile2);
	}
}

Bullet* MissileLauncher::getBullet() {
	return Missile::create(this->getTarget(), this->isUpgraded());
}

Sprite* MissileLauncher::getPlaceholderMissile() {
	return _missile;
}