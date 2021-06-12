#include "Player.h"

static Player* _instance = nullptr;

void Player::Reset() {
	_instance = nullptr;
}

Player* Player::getInstance() {
	if (!_instance) {
		_instance = new (std::nothrow) Player;
		_instance->init();
    }
	return _instance;
}

bool Player::init() {
	_money = 250;
	_startLives = 100;
	_lives = _startLives;
	_rounds = -1;
	
	_isPaused = false;
	_isGameOver = false;
	_isEndless = false;
	
	return true;
}

void Player::setMoney(const int m) {
	_money = m;
}

int Player::getMoney() {
	return _money;
}

void Player::setLives(const int l) {
	_lives = l;
	if (_lives <= 0) this->setGameOver(true);
}

int Player::getLives() {
	return _lives;
}

int Player::getStartLives() {
	return _startLives;
}

void Player::setRound(const int r) {
	_rounds = r;
}

int Player::getRound() {
	return _rounds;
}

void Player::setPaused(const bool p) {
	_isPaused = p;
}

bool Player::isPaused() {
	return _isPaused;
}

void Player::setGameOver(const bool g) {
	_isGameOver = g;
}

bool Player::isGameOver() {
	return _isGameOver;
}

void Player::setEndless(const bool e) {
	_isEndless = e;
}

bool Player::isEndless() {
	return _isEndless;
}