#include "Bullet.h"

Bullet* Bullet::create(Enemy* e) {
	Bullet *bullet = new (std::nothrow) Bullet();
	if (bullet && bullet->init( e)) {
		bullet->autorelease();
		return bullet;
	}
	CC_SAFE_DELETE(bullet);
	return nullptr;
}

bool Bullet::init(Enemy* e) {
	if (!this->initWithFile("Bullets/bullet.png")) {
		return false;
	}
	
	this->setLocalZOrder(11);
	
	this->setTarget(e);
	
	this->setDamage(50.0f);
	
	this->setSpeed(2400.0f);
	
	this->scheduleUpdate();
	
	return true;
}

void Bullet::update(float dt) {
	if (Player::getInstance()->isPaused()) return;
	
	if (this->getTarget() == nullptr || this->getTarget()->getTruePosition() < Vec2(0, 0)) {
		this->missTarget();
		return;
	}
	
	auto dir = this->getTarget()->getTruePosition() - this->getPosition();
	auto dis = this->getSpeed()*dt;
	if (dir.length() <= dis) {
		this->hitTarget();
		return;
	}
	
	auto angle = atan2(dir.x, dir.y) * DEGREES;
	this->setRotation(angle);
	
	this->runAction(MoveBy::create(dt, dir.getNormalized()*dis));
}

void Bullet::missTarget() {
	auto miss = Sprite::create("Bullets/bulletMiss.png");
	miss->setPosition(this->getPosition());
	miss->setLocalZOrder(7);
	this->getParent()->addChild(miss);
	miss->runAction(Sequence::create(
		FadeOut::create(5.0f),
		RemoveSelf::create(),
		nullptr
	));
	this->removeFromParent();
}

void Bullet::hitTarget() {
	auto hit = Sprite::create("Bullets/bulletHit.png");;
	hit->setPosition(this->getPosition());
	hit->setLocalZOrder(9);
	this->getParent()->addChild(hit);
	hit->runAction(Sequence::create(
		FadeOut::create(0.5f),
		RemoveSelf::create(),
		nullptr
	));
	this->getTarget()->damage(this->getDamage());
	this->removeFromParent();
}

void Bullet::setTarget(Enemy* t) {
	_target = t;
}

Enemy* Bullet::getTarget() {
	return _target;
}

void Bullet::setDamage(const float d) {
	_damage = d;
}

float Bullet::getDamage() {
	return _damage;
}

void Bullet::setSpeed(const float &s) {
	_speed = s;
}

float Bullet::getSpeed() {
	return _speed;
}
