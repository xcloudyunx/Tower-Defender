#include "EnemyManager.h"

static EnemyManager* _instance = nullptr;

void EnemyManager::Reset() {
	_instance = nullptr;
}

EnemyManager* EnemyManager::getInstance() {
	if (!_instance) {
		_instance = new (std::nothrow) EnemyManager;
		_instance->init();
    }
	return _instance;
}

bool EnemyManager::init() {
	_player = Player::getInstance();
	
	_enemies = new std::list<Enemy*>;
	_path = new std::vector<Sprite*>;
	
	addWaves();
	_waveIndex = 0;
	
	_waitTime = 2.0f;
	_countdown = _waitTime;
	
	this->schedule(schedule_selector(EnemyManager::updateEnemies), 0.1f);
	this->scheduleUpdate();
	
	return true;
}

std::list<Enemy*>* EnemyManager::getEnemies() {
	return _enemies;
}

std::vector<Sprite*>* EnemyManager::getPath() {
	return _path;
}

void EnemyManager::update(float dt) {
	if (_player->isPaused() || _player->isGameOver()) return;
	if (!_finishedRound || _enemies->size() > 0) return;
	
	_countdown -= dt;
	if (_countdown <= 0) {
		spawnWave();
		_countdown = _waitTime;
	}
}

void EnemyManager::updateEnemies(float dt) {
	if (_enemies->empty()) return;
	for (auto i=prev(_enemies->end()); i!=_enemies->begin(); i--) {
		if ((*i)->getIndex() > (*prev(i))->getIndex()) {
			std::swap(*i, *prev(i));
		}
	}
}

void EnemyManager::spawnWave() {
	_player->setRound(_waveIndex);
	
	if (!_player->isEndless() && _waveIndex == _waves.size()) {
		_player->setGameOver(true);
		return;
	}
	
	if (_player->isEndless()) generateWave(_player->getRound());

	auto wave = _waves.at(_waveIndex);
	
	 _counter = 0;
	 _finishedRound = false;
	for (int i=0; i<wave->getSize(); i++) {
		runAction(Sequence::create(
			DelayTime::create(i/wave->getRate()),
			CallFunc::create([&]() {
				spawnEnemy(_waves.at(_waveIndex)->getEnemy(_counter++));
			}),
			nullptr
		));
	}
	
	runAction(Sequence::create(
		DelayTime::create(wave->getSize()/wave->getRate()),
		CallFunc::create([&]() {
			_waves.at(_waveIndex++)->release();
			_finishedRound = true;
		}),
		nullptr
	));
}

void EnemyManager::spawnEnemy(const int e) {
	Enemy* enemy = Enemy::create(_enemies, _path, e);
	this->getParent()->addChild(enemy);
}

void EnemyManager::addWaves() {
	auto wave1 = Wave::create({0, 0, 0, 0}, 0.5f);
	auto wave2 = Wave::create({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0.5f);
	auto wave3 = Wave::create({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 1.0f);
	auto wave4 = Wave::create({0, 1, 0, 1, 0, 1}, 1.0f);
	auto wave5 = Wave::create({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, 1.0f);
	auto wave6 = Wave::create({0, 2, 0, 2, 0, 2}, 2.0f);
	auto wave7 = Wave::create({2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, 1.0f);
	auto wave8 = Wave::create({0, 3, 0, 3, 0, 3}, 1.0f);
	auto wave9 = Wave::create({2, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1, 3, 3, 3, 3}, 1.5f);
	auto wave10 = Wave::create({3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3}, 1.0f);
	auto wave11 = Wave::create({0, 0, 0, 0, 4, 4, 4, 4, 0, 0, 0, 0, 4, 4, 4, 4}, 1.0f);
	auto wave12 = Wave::create({1, 1, 1, 1, 5, 5, 5, 5, 1, 1, 1, 1, 5, 5, 5, 5}, 1.0f);
	auto wave13 = Wave::create({2, 2, 2, 2, 6, 6, 6, 6, 2, 2, 2, 2, 6, 6, 6, 6}, 2.0f);
	auto wave14 = Wave::create({3, 3, 3, 3, 7, 7, 7, 7, 3, 3, 3, 3, 7, 7, 7, 7}, 1.0f);
	auto wave15 = Wave::create({0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7, 
								2, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1, 1, 3, 3, 3, 3,
								6, 6, 6, 6, 4, 4, 4, 4, 5, 5, 5, 5, 7, 7, 7, 7}, 1.5f);
	auto wave16 = Wave::create({6, 6, 6, 6, 4, 4, 4, 4, 5, 5, 5, 5, 7, 7, 7, 7,
								5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
								4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
								7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7}, 2.0f);
	auto wave17 = Wave::create({0, 0, 0, 0, 4, 4, 4, 4, 8, 8, 8, 8, 8, 8, 8, 8}, 1.0f);
	auto wave18 = Wave::create({8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8}, 1.0f);
	auto wave19 = Wave::create({1, 1, 1, 1, 5, 5, 5, 5, 9, 9, 9, 9, 9, 9, 9, 9}, 1.0f);
	auto wave20 = Wave::create({9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9}, 1.0f);
	auto wave21 = Wave::create({2, 2, 2, 2, 6, 6, 6, 6, 10, 10, 10, 10, 10, 10, 10, 10}, 1.0f);
	auto wave22 = Wave::create({10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10}, 1.0f);
	auto wave23 = Wave::create({3, 3, 3, 3, 7, 7, 7, 7, 11, 11, 11, 11, 11, 11, 11, 11}, 1.0f);
	auto wave24 = Wave::create({11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11}, 1.0f);
	auto wave25 = Wave::create({0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
								1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
								2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
								3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
								4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
								5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
								6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
								7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
								8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
								9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
								10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
								11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
								0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
								0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
								11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
								11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1}, 1.5f);

	_waves.push_back(wave1);
	_waves.push_back(wave2);
	_waves.push_back(wave3);
	_waves.push_back(wave4);
	_waves.push_back(wave5);
	_waves.push_back(wave6);
	_waves.push_back(wave7);
	_waves.push_back(wave8);
	_waves.push_back(wave9);
	_waves.push_back(wave10);
	_waves.push_back(wave11);
	_waves.push_back(wave12);
	_waves.push_back(wave13);
	_waves.push_back(wave14);
	_waves.push_back(wave15);
	_waves.push_back(wave16);
	_waves.push_back(wave17);
	_waves.push_back(wave18);
	_waves.push_back(wave19);
	_waves.push_back(wave20);
	_waves.push_back(wave21);
	_waves.push_back(wave22);
	_waves.push_back(wave23);
	_waves.push_back(wave24);
	_waves.push_back(wave25);
	generateWave(26);
}

void EnemyManager::generateWave(const int round) {
	std::vector<int> e;
	for (int i=0; i<(1<<(round-18)); i++) {
		e.push_back(random(0, 11));
	}
	_waves.push_back(Wave::create(e, random(round/20+1, round/10+1)*(rand_0_1()+1)));
}