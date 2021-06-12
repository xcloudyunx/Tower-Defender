#include "Enemy.h"

Enemy* Enemy::create(std::list<Enemy*>* &enemies, std::vector<Sprite*>* &path, const int n) {
	Enemy *enemy = new (std::nothrow) Enemy();
	if (enemy && enemy->init(enemies, path, n)) {
		enemy->autorelease();
		return enemy;
	}
	CC_SAFE_DELETE(enemy);

	return nullptr;
}

bool Enemy::init(std::list<Enemy*>* &enemies, std::vector<Sprite*>* &path, const int n) {
	if (!this->initWithFile("Enemies/enemy"+std::to_string(n)+".png")) {
		return false;
	}
	
	_player = Player::getInstance();
	_enemies = enemies;
	_path = path;
	
	_enemies->push_back(this);
	
	for (int i=1; i<=3; i++) {
		auto damage = Sprite::create("Enemies/enemyDamage"+std::to_string(n/4)+"-"+std::to_string(i)+".png");
		damage->setPosition(this->getBoundingBox().size/2);
		damage->setTag(i);
		damage->setVisible(false);
		this->addChild(damage);
	}
	
	_dead = false;
	
	_currentPos = 0;
	_target = _path->at(0);
	_offset = _target->getBoundingBox().size/2;
	this->setPosition(_target->getPosition());
	this->setLocalZOrder(8);
	
	_healthBarBase = Sprite::create("Enemies/healthBarBase.png");
	_healthBarBase->setScale(0.2f);
	_healthBarBase->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height*3/2));
	this->addChild(_healthBarBase);
	_healthBar = LoadingBar::create("Enemies/healthBar.png");
	_healthBar->setPosition(_healthBarBase->getContentSize()/2);
	_healthBarBase->addChild(_healthBar);
	
	
	switch(n) {
		case 0:										// first normal
			this->setStartingHealth(100.0f);
			this->setValue(10);
			this->setSpeed(300.0f);
			break;
		case 1:										// first speed
			this->setStartingHealth(50.0f);
			this->setValue(5);
			this->setSpeed(400.0f);
			break;
		case 2:										// first tank
			this->setStartingHealth(150.0f);
			this->setValue(15);
			this->setSpeed(200.0f);
			break;
		case 3:										// first all
			this->setStartingHealth(150.0f);
			this->setValue(20);
			this->setSpeed(400.0f);
			break;
		case 4:										// second normal
			this->setStartingHealth(400.0f);
			this->setValue(25);
			this->setSpeed(500.0f);
			break;
		case 5:										// second speed
			this->setStartingHealth(200.0f);
			this->setValue(10);
			this->setSpeed(700.0f);
			break;
		case 6:										// second tank
			this->setStartingHealth(600.0f);
			this->setValue(30);
			this->setSpeed(300.0f);
			break;
		case 7:										// second all
			this->setStartingHealth(600.0f);
			this->setValue(40);
			this->setSpeed(700.0f);
			break;
		case 8:										// third normal
			this->setStartingHealth(800.0f);
			this->setValue(50);
			this->setSpeed(800.0f);
			break;
		case 9:										// third speed
			this->setStartingHealth(400.0f);
			this->setValue(25);
			this->setSpeed(1000.0f);
			break;
		case 10:									// third tank
			this->setStartingHealth(1200.0f);
			this->setValue(60);
			this->setSpeed(600.0f);
			break;
		case 11:									// third all
			this->setStartingHealth(1200.0f);
			this->setValue(85);
			this->setSpeed(1000.0f);
			break;
		default:
			log("IDK how this happened but unlucky");
			log("This guy should also be invisible so...");
			this->setStartingHealth(1000000000.0f);
			this->setValue(0);
			this->setSpeed(1200.0f);
			break;
	}
	
	
	
	this->setHealth(this->getStartingHealth());
	this->setSlow(1.0f);
	
	this->scheduleUpdate();
	
	return true;
}

void Enemy::update(float dt) {
	if (_player->isPaused()) return;
	
	auto dir = _target->getPosition() - this->getPosition();
	this->runAction(MoveBy::create(dt, dir.getNormalized()*this->getSpeed()*dt*this->getSlow()));
	
	auto angle = atan2(dir.x, dir.y) * DEGREES;
	this->runAction(RotateTo::create(dt/(10.0f*this->getSpeed()), angle));
	
	_healthBarBase->setRotation(-this->getRotation());
	auto pos = Vec2(this->getContentSize().width/2, this->getContentSize().height*3/2);
	pos.rotate(this->getContentSize()/2, this->getRotation()/DEGREES);
	_healthBarBase->setPosition(pos);
	
	if (this->getPosition().distance(_target->getPosition()) <= 20.0f) {
		getNextTarget();
	}
	
	this->setSlow(1.0f);
}

void Enemy::setPosition(const Vec2 pos) {
	Sprite::setPosition(pos + _offset);
}

Vec2 Enemy::getPosition() {
	return Sprite::getPosition() - _offset;
}

Vec2 Enemy::getTruePosition() {
	return Sprite::getPosition();
}

void Enemy::setStartingHealth(const float h) {
	_startingHealth = h;
}

float Enemy::getStartingHealth() {
	return _startingHealth;
}

void Enemy::setHealth(const float h) {
	_health = h;
	_healthBar->setPercent(100*_health/_startingHealth);
}

float Enemy::getHealth() {
	return _health;
}

void Enemy::setValue(const int v) {
	_value = v;
}

int Enemy::getValue() {
	return _value;
}

void Enemy::setSpeed(const float s) {
	_speed = s;
}

float Enemy::getSpeed() {
	return _speed;
}

void Enemy::setSlow(const float s) {
	_slow = s;
}

float Enemy::getSlow() {
	return _slow;
}

void Enemy::getNextTarget() {
	_currentPos++;
	
	if (_currentPos >= _path->size()) {
		_player->setLives(_player->getLives()-1);
		die();
		return;
	}
	
	_target = _path->at(_currentPos);
}

int Enemy::getIndex() {
	return _currentPos;
}

void Enemy::slow(const float s) {
	this->setSlow(std::min(s, this->getSlow()));
}

void Enemy::damage(const float d) {
	this->setHealth(this->getHealth() - d);
	
	if (this->getHealth() <= 0) {
		_player->setMoney(_player->getMoney() + this->getValue());
		this->die();
	} else if (this->getHealth() <= this->getStartingHealth()*1/4) {
		this->getChildByTag(3)->setVisible(true);
	} else if (this->getHealth() <= this->getStartingHealth()*1/2) {
		this->getChildByTag(2)->setVisible(true);
	} else if (this->getHealth() <= this->getStartingHealth()*3/4) {
		this->getChildByTag(1)->setVisible(true);
	}
}

void Enemy::die() {
	if (!_dead) {
		_dead = true;
		_healthBarBase->removeFromParent();
		_enemies->remove(this);
		this->unscheduleUpdate();
		this->runAction(Sequence::create(
			FadeOut::create(0.5f),
			RemoveSelf::create(),
			nullptr
		));
	}
}