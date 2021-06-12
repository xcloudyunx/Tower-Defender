#include "Turret.h"

#define EMPTYGROUND 10

int Turret::getCost() {
	return Turret::Cost;
}

int Turret::getUpgradeCost() {
	return Turret::UpgradeCost;
}

int Turret::getSellCost() {
	return Turret::SellCost;
}

int Turret::getUpgradedSellCost() {
	return Turret::UpgradedSellCost;
}

Turret* Turret::create() {
	Turret *turret = new (std::nothrow) Turret();
	if (turret && turret->init()) {
		turret->autorelease();
		return turret;
	}
	CC_SAFE_DELETE(turret);
	return nullptr;
}

bool Turret::init() {
	if (!this->initWithFile("Turrets/turretBase.png")) {
		return false;
	}
	
	this->setLocalZOrder(10);
	
	this->setHead(Sprite::create("Turrets/turretHead.png"));
	this->getHead()->setAnchorPoint(Vec2(0.5f, 0.28f));
	
	this->setOffset(Vec2(64, 64)); //will need to change
	this->setBulletOffset(Vec2(0, 120));
	
	this->setRange(350.0f);
	this->setFireRate(1.0f);
	
	this->setScale(0.1f);
	this->runAction(EaseBackOut::create(ScaleTo::create(0.1f, 1.0f)));
	
	this->schedule(schedule_selector(Turret::updateTarget), 0.1f);
	this->scheduleUpdate();
	
	return true;
}

void Turret::upgrade() {
	this->setHead(Sprite::create("Turrets/turretHeadUpgraded.png"));
	this->getHead()->setAnchorPoint(Vec2(0.5f, 0.28f));
	this->setBulletOffset(Vec2(15, 120));
	
	this->setFireRate(2*this->getFireRate());
	
	this->setScale(0.1f);
	this->runAction(EaseBackOut::create(ScaleTo::create(0.1f, 1.0f)));
	
	this->setFireCountdown(0.0f);
	
	this->setUpgraded(true);
}

void Turret::setUpgraded(const bool u) {
	_isUpgraded = u;
}

bool Turret::isUpgraded() {
	return _isUpgraded;
}

void Turret::sell() {
	auto grid = dynamic_cast<TMXTiledMap*>(this->getParent());
	auto layer = grid->getLayer("default");
	int i = this->getTruePosition().x/128;
	int j = grid->getMapSize().height - ceil(this->getTruePosition().y/128);
	auto tile = layer->getTileAt(Vec2(i, j));
	layer->setTileGID(EMPTYGROUND, Vec2(i, j));
	this->unscheduleUpdate();
	
	this->runAction(Sequence::create(
		EaseBackOut::create(ScaleTo::create(0.1f, 0.1f)),
		RemoveSelf::create(),
		nullptr
	));
}

void Turret::updateTarget(float dt) {
	for (auto i : *EnemyManager::getInstance()->getEnemies()) {
		auto distanceToEnemy = this->getTruePosition().distance(i->getTruePosition());
		if (distanceToEnemy <= this->getRange()) {
			this->setTarget(i);
			return;
		}
	}
	
	this->setTarget(nullptr);
}

void Turret::update(float dt) {
	if (Player::getInstance()->isPaused()) return;
	
	_fireCountdown -= dt;
	
	if (this->getTarget() == nullptr) return;
	if (this->getTarget()->getTruePosition() < Vec2(0, 0)) updateTarget(0);
	if (this->getTarget() == nullptr) return;
	
	this->lockTarget(dt);
	
	if (_fireCountdown <= 0) {
		this->shoot();
		_fireCountdown = 1.0f/this->getFireRate();
	}
}

void Turret::lockTarget(float dt) {
	auto dir = this->getTarget()->getTruePosition() - this->getTruePosition();
	auto angle = atan2(dir.x, dir.y) * DEGREES;
	this->getHead()->setRotation(angle);
}

void Turret::shoot() {
	auto bullet = this->getBullet();
	bullet->setPosition(this->getBulletSpawnPosition());
	bullet->setRotation(this->getHead()->getRotation());
	this->getParent()->addChild(bullet);
}

Bullet* Turret::getBullet() {
	return Bullet::create(this->getTarget());
}

Vec2 Turret::getBulletSpawnPosition() {
	auto offset = this->getBulletOffset();
	this->setBulletOffset(Vec2(-offset.x, offset.y));
	offset.rotate(Vec2(0, 0), -this->getHead()->getRotation()/DEGREES);
	return this->getTruePosition() + offset;
}

void Turret::setHead(Sprite* h) {
	if (_head) {
		_head->removeFromParent();
	}
	_head = h;
	_head->setPosition(this->getBoundingBox().size/2);
	this->addChild(_head);
}

Sprite* Turret::getHead() {
	return _head;
}

void Turret::setTarget(Enemy* e) {
	_target = e;
}

Enemy* Turret::getTarget() {
	return _target;
}

void Turret::setRange(const float &r) {
	_range = r;
}

float Turret::getRange() {
	return _range;
}

void Turret::setFireRate(const float &f) {
	_fireRate = f;
}

float Turret::getFireRate() {
	return _fireRate;
}

void Turret::setFireCountdown(const float &f) {
	_fireCountdown = f;
}

float Turret::getFireCountdown() {
	return _fireCountdown;
}

void Turret::setOffset(const Vec2 o) {
	_offset = o;
}

Vec2 Turret::getOffset() {
	return _offset;
}

void Turret::setBulletOffset(const Vec2 o) {
	_bulletOffset = o;
}

Vec2 Turret::getBulletOffset() {
	return _bulletOffset;
}

void Turret::setPosition(const Vec2 pos) {
	Sprite::setPosition(pos + this->getOffset());
}

Vec2 Turret::getPosition() {
	return Sprite::getPosition() - this->getOffset();
}

void Turret::setTruePosition(const Vec2 pos) {
	Sprite::setPosition(pos);
}

Vec2 Turret::getTruePosition() {
	return Sprite::getPosition();
}
