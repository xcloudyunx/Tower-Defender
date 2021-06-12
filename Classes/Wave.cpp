#include "Wave.h"

Wave* Wave::create(std::vector<int> e, const float r) {
	Wave *wave = new (std::nothrow) Wave();
	if (wave && wave->init(e, r)) {
		wave->autorelease();
		return wave;
	}
	CC_SAFE_DELETE(wave);

	return nullptr;
}

bool Wave::init(std::vector<int> e, const float r) {
	_enemies = e;
	_rate = r;
	
	this->retain();
	return true;
}

int Wave::getEnemy(const unsigned int i) {
	return _enemies.at(i);
}

int Wave::getSize() {
	return _enemies.size();
}

float Wave::getRate() {
	return _rate;
}