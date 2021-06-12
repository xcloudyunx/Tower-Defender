#include "LaserBeamer.h"

int LaserBeamer::getCost() {
	return LaserBeamer::Cost;
}

int LaserBeamer::getUpgradeCost() {
	return LaserBeamer::UpgradeCost;
}

int LaserBeamer::getSellCost() {
	return LaserBeamer::SellCost;
}

int LaserBeamer::getUpgradedSellCost() {
	return LaserBeamer::UpgradedSellCost;
}

LaserBeamer* LaserBeamer::create() {
	LaserBeamer *beamer = new (std::nothrow) LaserBeamer();
	if (beamer && beamer->init()) {
		beamer->autorelease();
		return beamer;
	}
	CC_SAFE_DELETE(beamer);
	return nullptr;
}

bool LaserBeamer::init() {
	if (!this->initWithFile("Turrets/laserBeamerBase.png")) {
		return false;
	}
	
	this->setLocalZOrder(10);
	
	this->setHead(Sprite::create("Turrets/laserBeamerHead.png"));
	this->getHead()->setAnchorPoint(Vec2(0.5f, 0.28f));
	
	this->setOffset(Vec2(64, 64)); //will need to change
	this->setBulletOffset(Vec2(0, 60));
	
	this->setRange(480.0f);
	
	_laser = Sprite::create("Bullets/laser.png");
	_laser->setScaleX(0.4f);
	_laser->setAnchorPoint(Vec2(0.5f, 0.0f));
	this->setLaserDamage(50.0f);
	this->setLaserSlow(0.5f);
	
	this->setScale(0.1f);
	this->runAction(EaseBackOut::create(ScaleTo::create(0.1f, 1.0f)));
	
	this->schedule(schedule_selector(LaserBeamer::updateTarget), 0.1f);
	this->scheduleUpdate();
	
	return true;
}

void LaserBeamer::upgrade() {
	this->setHead(Sprite::create("Turrets/laserBeamerHeadUpgraded.png"));
	this->getHead()->setAnchorPoint(Vec2(0.5f, 0.28f));
	
	_laser->removeFromParent();
	_laser = Sprite::create("Bullets/laserUpgraded.png");
	_laser->setScaleX(0.4f);
	_laser->setAnchorPoint(Vec2(0.5f, 0.0f));
	this->setLaserDamage(2*this->getLaserDamage());
	this->setLaserSlow(0.5f);
	
	this->setScale(0.1f);
	this->runAction(EaseBackOut::create(ScaleTo::create(0.1f, 1.0f)));
	
	this->setUpgraded(true);
}

void LaserBeamer::sell() {
	_laser->removeFromParent();
	Turret::sell();
}

void LaserBeamer::update(float dt) {
	if (Player::getInstance()->isPaused()) return;
	
	this->getLaser()->setVisible(false);
	if (this->getTarget() == nullptr) return;
	if (this->getTarget()->getTruePosition() < Vec2(0, 0)) updateTarget(0);
	if (this->getTarget() == nullptr) return;
	
	this->lockTarget(dt);
	
	this->getLaser()->setVisible(true);
	this->getLaser()->setPosition(this->getBulletSpawnPosition());
	this->getLaser()->setScaleY(this->getLaser()->getPosition().distance(this->getTarget()->getTruePosition())/100);
	this->getLaser()->setRotation(this->getHead()->getRotation());
	this->getTarget()->damage(this->getLaserDamage()*dt);
	this->getTarget()->slow(this->getLaserSlow());
}

Sprite* LaserBeamer::getLaser() {
	if (!_laser->getParent()) this->getParent()->addChild(_laser);
	return _laser;
}

void LaserBeamer::setLaserDamage(const float d) {
	_laserDamage = d;
}

float LaserBeamer::getLaserDamage() {
	return _laserDamage;
}

void LaserBeamer::setLaserSlow(const float s) {
	_laserSlow = s;
}

float LaserBeamer::getLaserSlow() {
	return _laserSlow;
}
