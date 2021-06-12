#include "Missile.h"

Missile* Missile::create(Enemy* e, const bool u) {
	Missile *missile = new (std::nothrow) Missile();
	if (missile && missile->init(e, u)) {
		missile->autorelease();
		return missile;
	}
	CC_SAFE_DELETE(missile);
	return nullptr;
}

bool Missile::init(Enemy* e, const bool u) {
	if (!this->initWithFile(u ? "Bullets/missileUpgraded.png" : "Bullets/missile.png")) {
		return false;
	}
	
	this->setLocalZOrder(11);
	
	this->setTarget(e);
	
	this->setDamage(50.0f);
	
	this->setSpeed(800.0f);
	
	this->setRadius(200.0f);
	
	this->scheduleUpdate();
	
	return true;
}

void Missile::setRadius(const float r) {
	_radius = r;
}

float Missile::getRadius() {
	return _radius;
}

void Missile::missTarget() {
	auto miss = Sprite::create("Bullets/missileMiss.png");
	miss->setPosition(this->getPosition());
	miss->setLocalZOrder(7);
	this->getParent()->addChild(miss);
	miss->runAction(Sequence::create(
		FadeOut::create(10.0f),
		RemoveSelf::create(),
		nullptr
	));
	this->removeFromParent();
}

void Missile::hitTarget() {
	auto hit = Sprite::create("Bullets/missileHit.png");;
	hit->setPosition(this->getPosition());
	hit->setLocalZOrder(9);
	this->getParent()->addChild(hit);
	hit->runAction(Sequence::create(
		FadeOut::create(0.5f),
		RemoveSelf::create(),
		nullptr
	));
	
	std::queue<Enemy*> damaged;
	for (auto i : *EnemyManager::getInstance()->getEnemies()) {
		auto distanceToEnemy = this->getPosition().distance(i->getTruePosition());
		if (distanceToEnemy <= this->getRadius()) {
			damaged.push(i);
		}
	}
	while (!damaged.empty()) {
		damaged.front()->damage(this->getDamage());
		damaged.pop();
	}
	this->missTarget();
}
