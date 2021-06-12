#include "GameScene.h"

#define GRASS 1
#define BASEBOT 2
#define BASEBOTDMG1 3
#define BASEBOTDMG2 4
#define BASERIGHT 5
#define BASERIGHTDMG1 6
#define BASERIGHTDMG2 7
#define BASECORNERRIGHT 8
#define BASECORNERBOT 9
#define EMPTYGROUND 10
#define EMPTYGROUNDBUILD 11
#define GROUND 12
#define PATHHORIZONTAL 13
#define PATHVERTICAL 14
#define PATHBOTLEFT 15
#define PATHTOPLEFT 16
#define PATHTOPRIGHT 17
#define PATHBOTRIGHT 18

Scene* Game::createScene() {
	return Game::create();
}

bool Game::init() {
	if (!Scene::init()){
		return false;
	}
		
	_visibleSize = _director->getVisibleSize();
	Vec2 origin = _director->getVisibleOrigin();
	
	Player::Reset();
	BuildManager::Reset();
	NodeUI::Reset();
	EnemyManager::Reset();
	
	_player = Player::getInstance();
	_buildManager = BuildManager::getInstance();
	_nodeUI = NodeUI::getInstance();
	_enemyManager = EnemyManager::getInstance();
	_enemies = _enemyManager->getEnemies();
	_path = _enemyManager->getPath();
	
	
	
	_currentRound = -1;
	
	
	
	_grid = TMXTiledMap::create("Map/grid.tmx");
	_layer = _grid->getLayer("default");
	_grid->setScale(0.3f);
	_grid->setAnchorPoint(Vec2(0.0f, 1.0f));
	_grid->setPosition(Vec2(0, _visibleSize.height));
	this->addChild(_grid);
	std::vector<std::vector<bool>> validity(_grid->getMapSize().width, std::vector<bool>(_grid->getMapSize().height, true));
	int tmp = random(0, 1);
	Vec2 start;
	if (tmp) start = Vec2(1, 2); 
	else start = Vec2(2, 1);
	while (!generateMap(start, start-Vec2(1, 1), validity)) validity.assign(_grid->getMapSize().width, std::vector<bool>(_grid->getMapSize().height, true));
	std::reverse(_path->begin(), _path->end());
	_path->push_back(_layer->getTileAt(Vec2(1, 1)));
	if (_layer->getTileGIDAt(Vec2(1, 2)) != EMPTYGROUND) _layer->setTileGID(BASECORNERBOT, Vec2(1, 1));
	else _layer->setTileGID(BASECORNERRIGHT, Vec2(1, 1));
	
	
	_nodeUI->setContentSize(Size(_visibleSize.width, _visibleSize.height-_grid->getBoundingBox().size.height));
	this->addChild(_nodeUI);
	_grid->addChild(_enemyManager);
	
	
	
	_shop = Menu::create();
	
	auto lives = Label::createWithTTF("LIVES: " + std::to_string(_player->getLives()), "Azonix.ttf", 25.0f);
	_lives = MenuItemLabel::create(lives);
	_lives->setDisabledColor(Color3B(255, 255, 255));
	_lives->setEnabled(false);
	_shop->addChild(_lives);
	
	auto money = Label::createWithTTF("$"+std::to_string(_player->getMoney()), "Azonix.ttf", 25.0f);
	_money = MenuItemLabel::create(money);
	_money->setDisabledColor(Color3B(255, 255, 255));
	_money->setEnabled(false);
	_shop->addChild(_money);
	
	for (int i=1; i<=_turretTypes; i++) {
		auto turret = MenuItemImage::create("Shop/turret"+std::to_string(i)+".png", "Shop/turret"+std::to_string(i)+"Selected.png", "Shop/turret"+std::to_string(i)+"Disabled.png",
			[&](Ref* sender) {
				auto turret = dynamic_cast<MenuItemImage*>(sender);
				_buildManager->setTurretToBuild(turret->getTag());
				for (int i=0; i<_grid->getMapSize().width; i++) {
					for (int j=0; j<_grid->getMapSize().height; j++) {
						if (_layer->getTileGIDAt(Vec2(i, j)) == EMPTYGROUND) {
							_layer->setTileGID(EMPTYGROUNDBUILD, Vec2(i, j));
						}
					}
				}
				_nodeUI->Hide();
			}
		);
		turret->setTag(i);
		int price;
		if (i == 1) price = Turret::Cost;
		else if (i == 2) price = LaserBeamer::Cost;
		else if (i == 3) price = MissileLauncher::Cost;
		turret->setName(std::to_string(price));
		turret->setScale(0.5f);
		auto priceTag = Label::createWithTTF("$"+std::to_string(price), "Azonix.ttf", 25.0f);
		turret->addChild(priceTag);
		_shopItems.push_back(turret);
		_shop->addChild(turret);
	}
	
	_shop->alignItemsVerticallyWithPadding(20.0f);
	_shop->setPosition(Vec2(_visibleSize.width-120.0f, _visibleSize.height/2));
	this->addChild(_shop);
	
	
	
	auto touchListener = EventListenerTouchOneByOne::create();
	
	touchListener->onTouchBegan = [&](Touch* touch, Event* event) {
		if (_grid->getBoundingBox().containsPoint(touch->getLocation())) {
			auto location = _grid->convertToNodeSpace(touch->getLocation());
			int i = location.x/128;
			int j = _grid->getMapSize().height - ceil(location.y/128);
			auto tile = _layer->getTileAt(Vec2(i, j));
			if (_buildManager->getTurretToBuild() && _layer->getTileGIDAt(Vec2(i, j)) == EMPTYGROUNDBUILD) {
				buildTurret(i, j);
				return true;
			} else if (!_buildManager->getTurretToBuild() && _layer->getTileGIDAt(Vec2(i, j)) == GROUND) {
				selectTurret(i, j);
				return true;
			}
		}
		resetTurret();
		return false;
	};
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _grid);
	
	
	
	_pauseButton = Button::create("Buttons/squareButton.png", "Buttons/squareButtonSelected.png", "Buttons/squareButtonDisabled.png");
	_pauseButton->setTitleText("||");
	_pauseButton->setTitleColor(Color3B(0, 0, 0));
	_pauseButton->getTitleLabel()->setScaleX(2.0f);
	_pauseButton->getTitleLabel()->setPosition(_pauseButton->getTitleLabel()->getPosition()+Vec2(0, 3));
	_pauseButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		auto button = dynamic_cast<Button*>(sender);
		auto label = button->getTitleLabel();
		switch(type) {
			case Widget::TouchEventType::BEGAN:
				label->setPosition(label->getPosition()-Vec2(0, 2));
				break;
			case Widget::TouchEventType::ENDED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				pauseGame();
				break;
			case Widget::TouchEventType::CANCELED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				break;
			default:
				break;
		}
	});
	_pauseButton->setPosition(Vec2(this->getBoundingBox().size.width*9/10, this->getBoundingBox().size.height*9/10));
	this->addChild(_pauseButton);
	
	
	
	_overlay = Sprite::create("overlay.png");
	_overlay->setLocalZOrder(100);
	_overlay->setOpacity(0.0f);
	_overlay->setPosition(_visibleSize/2);
	_overlay->setVisible(false);
	this->addChild(_overlay);
	
	
	_gameOverOverlay = Node::create();
	_gameOverOverlay->setVisible(false);
	_overlay->addChild(_gameOverOverlay);
	
	_gameOverText = Label::createWithTTF("GAME OVER", "Azonix.ttf", 70.0f);
	_gameOverText->setOpacity(0.0f);
	_gameOverText->setPosition(Vec2(_overlay->getBoundingBox().size.width/2, _overlay->getBoundingBox().size.height));
	_gameOverOverlay->addChild(_gameOverText);
	
	_roundsSurvived = Label::createWithTTF("125", "Azonix.ttf", 150.0f);
	_roundsSurvived->setCascadeOpacityEnabled(true);
	_roundsSurvived->setOpacity(0.0f);
	_roundsSurvived->setScale(2.0f);
	_roundsSurvived->setPosition(_overlay->getBoundingBox().size/2);
	_gameOverOverlay->addChild(_roundsSurvived);
	_roundsSurvivedText = Label::createWithTTF("ROUNDS SURVIVED", "Azonix.ttf", 25.0f);
	_roundsSurvivedText->setPositionY(-_roundsSurvived->getBoundingBox().size.height/16);
	_roundsSurvived->addChild(_roundsSurvivedText);
	
	_continue = Button::create("Buttons/button.png", "Buttons/buttonSelected.png");
	_continue->setCascadeOpacityEnabled(true);
	_continue->setOpacity(0.0f);
	_continue->setScale(0.5f);
	_continue->setTitleFontName("Azonix.ttf");
	_continue->setTitleColor(Color3B(0, 0, 0));
	_continue->setTitleText("CONTINUE");
	_continue->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		auto button = dynamic_cast<Button*>(sender);
		auto label = button->getTitleLabel();
		switch(type) {
			case Widget::TouchEventType::BEGAN:
				label->setPosition(label->getPosition()-Vec2(0, 2));
				break;
			case Widget::TouchEventType::ENDED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				continueGame();
				break;
			case Widget::TouchEventType::CANCELED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				break;
			default:
				break;
		}
	});
	_continue->setPosition(Vec2(_overlay->getBoundingBox().size.width/2, _overlay->getBoundingBox().size.height/4));
	_gameOverOverlay->addChild(_continue);
	
	
	_pauseOverlay = Node::create();
	_pauseOverlay->setVisible(false);
	_overlay->addChild(_pauseOverlay);
	
	_pauseText = Label::createWithTTF("PAUSED", "Azonix.ttf", 70.0f);
	_pauseText->setOpacity(0.0f);
	_pauseText->setPosition(Vec2(_overlay->getBoundingBox().size.width/2, _overlay->getBoundingBox().size.height));
	_pauseOverlay->addChild(_pauseText);
	
	_resume = Button::create("Buttons/button.png", "Buttons/buttonSelected.png");
	_resume->setCascadeOpacityEnabled(true);
	_resume->setOpacity(0.0f);
	_resume->setScale(0.5f);
	_resume->setTitleFontName("Azonix.ttf");
	_resume->setTitleColor(Color3B(0, 0, 0));
	_resume->setTitleText("RESUME");
	_resume->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		auto button = dynamic_cast<Button*>(sender);
		auto label = button->getTitleLabel();
		switch(type) {
			case Widget::TouchEventType::BEGAN:
				label->setPosition(label->getPosition()-Vec2(0, 2));
				break;
			case Widget::TouchEventType::ENDED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				resumeGame();
				break;
			case Widget::TouchEventType::CANCELED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				break;
			default:
				break;
		}
	});
	_resume->setPosition(_overlay->getBoundingBox().size/2);
	_pauseOverlay->addChild(_resume);
	
	
	_commonOverlay = Node::create();
	_commonOverlay->setVisible(false);
	_overlay->addChild(_commonOverlay);
	
	_restart = Button::create("Buttons/button.png", "Buttons/buttonSelected.png");
	_restart->setCascadeOpacityEnabled(true);
	_restart->setOpacity(0.0f);
	_restart->setScale(0.5f);
	_restart->setTitleFontName("Azonix.ttf");
	_restart->setTitleColor(Color3B(0, 0, 0));
	_restart->setTitleText("RESTART");
	_restart->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		auto button = dynamic_cast<Button*>(sender);
		auto label = button->getTitleLabel();
		switch(type) {
			case Widget::TouchEventType::BEGAN:
				label->setPosition(label->getPosition()-Vec2(0, 2));
				break;
			case Widget::TouchEventType::ENDED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				_director->replaceScene(TransitionFade::create(0.5f, Game::createScene()));
				break;
			case Widget::TouchEventType::CANCELED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				break;
			default:
				break;
		}
	});
	_restart->setPosition(Vec2(_overlay->getBoundingBox().size.width*2/5, _overlay->getBoundingBox().size.height/3));
	_commonOverlay->addChild(_restart);
	
	_menu = Button::create("Buttons/button.png", "Buttons/buttonSelected.png");
	_menu->setCascadeOpacityEnabled(true);
	_menu->setOpacity(0.0f);
	_menu->setScale(0.5f);
	_menu->setTitleFontName("Azonix.ttf");
	_menu->setTitleColor(Color3B(0, 0, 0));
	_menu->setTitleText("MENU");
	_menu->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		auto button = dynamic_cast<Button*>(sender);
		auto label = button->getTitleLabel();
		switch(type) {
			case Widget::TouchEventType::BEGAN:
				label->setPosition(label->getPosition()-Vec2(0, 2));
				break;
			case Widget::TouchEventType::ENDED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				_director->popScene(TransitionFade::create(0.5f, _director->getPreviousScene()));
				break;
			case Widget::TouchEventType::CANCELED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				break;
			default:
				break;
		}
	});
	_menu->setPosition(Vec2(_overlay->getBoundingBox().size.width*3/5, _overlay->getBoundingBox().size.height/3));
	_commonOverlay->addChild(_menu);
	
	this->setName("GAME");
	
	this->scheduleUpdate();
	
	
	
	return true;
}

void Game::update(float dt) {
	if (_player->isPaused()) return;
	if (_player->isGameOver()) {
		endGame();
		return;
	}
	
	if (_currentRound != _player->getRound()) {
		_currentRound = _player->getRound();
		auto dispRound = Label::createWithTTF("ROUND "+std::to_string(_currentRound+1), "Azonix.ttf", 70.0f);
		dispRound->setOpacity(0.0f);
		dispRound->setPosition(Vec2(_visibleSize.width/2, _visibleSize.height*2/3));
		this->addChild(dispRound);
		dispRound->runAction(Sequence::create(
			FadeIn::create(0.2f),
			FadeOut::create(2.0f),
			RemoveSelf::create(),
			nullptr
		));
	}
	
	_lives->setString("LIVES: " + std::to_string(_player->getLives()));
	if (_player->getLives()/(float)_player->getStartLives() < 0.34f) {
		_layer->setTileGID(BASEBOTDMG2, Vec2(0, 1));
		_layer->setTileGID(BASERIGHTDMG2, Vec2(1, 0));
	} else if (_player->getLives()/(float)_player->getStartLives() < 0.67f) {
		_layer->setTileGID(BASEBOTDMG1, Vec2(0, 1));
		_layer->setTileGID(BASERIGHTDMG1, Vec2(1, 0));
	}
	
	_money->setString("$"+std::to_string(_player->getMoney()));
	
	for (auto i : _shopItems) {
		if (_player->getMoney() >= stoi(i->getName())) {
			i->setEnabled(true);
		} else {
			i->setEnabled(false);
		}
	}
	
	if (_nodeUI->isVisible()) {
		if (_player->getMoney() < _buildManager->getTurret()->getUpgradeCost()) {
			_nodeUI->disableUpgrade();
		} else if (!_buildManager->getTurret()->isUpgraded()) {
			_nodeUI->enableUpgrade();
		}
	}
}

bool Game::generateMap(const Vec2 currentCo, Vec2 prevDir, std::vector<std::vector<bool>> &validity) {
	if (!validCo(currentCo)) return false;
	if (!validity[currentCo.x][currentCo.y]) return false;
	
	if (_layer->getTileGIDAt(currentCo) != EMPTYGROUND) return validity[currentCo.x][currentCo.y] = false;
	
	Vec2 p = Vec2(0, 0)-prevDir;
	for (int i=0; i<3; i++) {
		auto x = p.x;
		auto y = p.y;
		p.x = -y;
		p.y = x;
		if (validCo(currentCo+p)) {
			if (_layer->getTileGIDAt(currentCo+p) != EMPTYGROUND) return validity[currentCo.x][currentCo.y] = false;
		}
	}
	
	if (prevDir.x != 0) _layer->setTileGID(PATHHORIZONTAL, currentCo);
	else _layer->setTileGID(PATHVERTICAL, currentCo);
	_path->push_back(_layer->getTileAt(currentCo));
	if (currentCo == Vec2(_grid->getMapSize().width-1, _grid->getMapSize().height-1)) return true;
	
	int d = random(0, 3);
	Vec2 dir;
	if (d == 0) dir = Vec2(0, -1);
	else if (d == 1) dir = Vec2(1, 0);
	else if (d == 2) dir = Vec2(0, 1);
	else if (d == 3) dir = Vec2(-1, 0);
	
	bool valid = generateMap(currentCo+dir, dir, validity);
	int tmp = random(0, 1);
	for (int i=0; !valid && i<3; i++) {
		auto x = dir.x;
		auto y = dir.y;
		if (tmp) {
			dir.x = -y;
			dir.y = x;
		} else {
			dir.x = y;
			dir.y = -x;
		}
		valid = generateMap(currentCo+dir, dir, validity);
	}
	if (!valid) {
		_layer->setTileGID(EMPTYGROUND, currentCo);
		_path->pop_back();
	} else {
		if ((prevDir.x == 1 && dir.y == -1) || (prevDir.y == 1 && dir.x == -1)) _layer->setTileGID(PATHBOTRIGHT, currentCo);
		else if ((prevDir.x == 1 && dir.y == 1) || (prevDir.y == -1 && dir.x == -1)) _layer->setTileGID(PATHTOPRIGHT, currentCo);
		else if ((prevDir.x == -1 && dir.y == -1) || (prevDir.y == 1 && dir.x == 1)) _layer->setTileGID(PATHBOTLEFT, currentCo);
		else if ((prevDir.x == -1 && dir.y == 1) || (prevDir.y == -1 && dir.x == 1)) _layer->setTileGID(PATHTOPLEFT, currentCo);
	}
	return validity[currentCo.x][currentCo.y] = valid;
}

bool Game::validCo(const Vec2 currentCo) {
	if (currentCo.x < 0 || currentCo.x >= _grid->getMapSize().width || currentCo.y < 0 || currentCo.y >= _grid->getMapSize().height) return false;
	return true;
}

void Game::buildTurret(const int i, const int j) {
	auto turret = _buildManager->buildTurret();
	turret->setPosition(_layer->getTileAt(Vec2(i, j))->getPosition());
	turret->setTag(i*_grid->getMapSize().height + j);
	_grid->addChild(turret);
	_layer->setTileGID(GROUND, Vec2(i, j));
	
	resetTurret();
}

void Game::selectTurret(const int i, const int j) {
	auto turret = dynamic_cast<Turret*>(_grid->getChildByTag(i*_grid->getMapSize().height + j));
	_buildManager->setTurret(turret);
	_nodeUI->setScale(0.1f);
	_nodeUI->setPosition(_grid->convertToWorldSpace(turret->getTruePosition())-Vec2(0.0f, turret->getBoundingBox().size.height));
	_nodeUI->Show();
	if (turret->isUpgraded()) {
		_nodeUI->disableUpgrade();
		_nodeUI->setUpgradeCost("DONE");
		_nodeUI->setSellCost(turret->getUpgradedSellCost());
	} else {
		_nodeUI->setUpgradeCost("$"+std::to_string(turret->getUpgradeCost()));
		_nodeUI->setSellCost(turret->getSellCost());
	}
}

void Game::resetTurret() {
	if (_buildManager->getTurretToBuild()) {
		_buildManager->setTurretToBuild(0);
		for (int x=0; x<_grid->getMapSize().width; x++) {
			for (int y=0; y<_grid->getMapSize().height; y++) {
				if (_layer->getTileGIDAt(Vec2(x, y)) == EMPTYGROUNDBUILD) {
					_layer->setTileGID(EMPTYGROUND, Vec2(x, y));
				}
			}
		}
	} else {
		_buildManager->setTurret(nullptr);
		_nodeUI->Hide();
	}
}

void Game::pauseGame() {
	_player->setPaused(true);
	
	_pausedActions = _actionManager->pauseAllRunningActions();
	_eventDispatcher->pauseEventListenersForTarget(_grid);
	
	_shop->setEnabled(false);
	_nodeUI->setEnabled(false);
	_pauseButton->setEnabled(false);
	
	_overlay->setVisible(true);
	_pauseOverlay->setVisible(true);
	_commonOverlay->setVisible(true);

	_overlay->runAction(FadeIn::create(0.1f));
	_pauseText->runAction(Spawn::createWithTwoActions(
		FadeIn::create(0.15f),
		EaseBackOut::create(MoveTo::create(0.25f, Vec2(_overlay->getBoundingBox().size.width/2, _overlay->getBoundingBox().size.height*2/3)))
	));
	_resume->runAction(Sequence::create(
		DelayTime::create(0.1f),
		Spawn::createWithTwoActions(
			FadeIn::create(0.15f),
			EaseBackOut::create(ScaleTo::create(0.25f, 1.0f))
		),
		nullptr
	));
	_restart->runAction(Sequence::create(
		DelayTime::create(0.2f),
		Spawn::createWithTwoActions(
			FadeIn::create(0.15f),
			EaseBackOut::create(ScaleTo::create(0.25f, 1.0f))
		),
		nullptr
	));
	_menu->runAction(Sequence::create(
		DelayTime::create(0.3f),
		Spawn::createWithTwoActions(
			FadeIn::create(0.15f),
			EaseBackOut::create(ScaleTo::create(0.25f, 1.0f))
		),
		nullptr
	));
}

void Game::resumeGame() {
	_pauseText->runAction(Spawn::createWithTwoActions(
		FadeOut::create(0.15f),
		EaseBackIn::create(MoveTo::create(0.25f, Vec2(_overlay->getBoundingBox().size.width/2, _overlay->getBoundingBox().size.height)))
	));
	_resume->runAction(Sequence::create(
		DelayTime::create(0.1f),
		Spawn::createWithTwoActions(
			FadeOut::create(0.15f),
			EaseBackOut::create(ScaleTo::create(0.25f, 0.5f))
		),
		nullptr
	));
	_restart->runAction(Sequence::create(
		DelayTime::create(0.2f),
		Spawn::createWithTwoActions(
			FadeOut::create(0.15f),
			EaseBackIn::create(ScaleTo::create(0.25f, 0.5f))
		),
		nullptr
	));
	_menu->runAction(Sequence::create(
		DelayTime::create(0.3f),
		Spawn::createWithTwoActions(
			FadeOut::create(0.15f),
			EaseBackIn::create(ScaleTo::create(0.25f, 0.5f))
		),
		nullptr
	));
	_overlay->runAction(Sequence::create(
		DelayTime::create(0.6f),
		FadeOut::create(0.2f),
		CallFunc::create([&]() {
			_overlay->setVisible(false);
			_pauseOverlay->setVisible(false);
			_commonOverlay->setVisible(false);
			
			_shop->setEnabled(true);
			_nodeUI->setEnabled(true);
			_pauseButton->setEnabled(true);
			
			_actionManager->resumeTargets(_pausedActions);
			_eventDispatcher->resumeEventListenersForTarget(_grid);
			
			_player->setPaused(false);
		}),
		nullptr
	));
}

void Game::endGame() {
	this->unscheduleUpdate();
	
	_eventDispatcher->pauseEventListenersForTarget(_grid);
	_shop->setEnabled(false);
	_nodeUI->setEnabled(false);
	_pauseButton->setEnabled(false);
	
	_overlay->setVisible(true);
	_gameOverOverlay->setVisible(true);
	_commonOverlay->setVisible(true);
	_roundsSurvived->setString(std::to_string(_player->getRound()));
	_roundsSurvivedText->setPositionX(_roundsSurvived->getBoundingBox().size.width/(2*_roundsSurvived->getScale()));
	
	_overlay->runAction(FadeIn::create(0.2f));
	_gameOverText->runAction(Spawn::createWithTwoActions(
		FadeIn::create(0.3f),
		EaseBackOut::create(MoveTo::create(0.5f, Vec2(_overlay->getBoundingBox().size.width/2, _overlay->getBoundingBox().size.height*2/3)))
	));
	_roundsSurvived->runAction(Sequence::create(
		DelayTime::create(0.3f),
		Spawn::createWithTwoActions(
			FadeIn::create(0.2f),
			EaseBackOut::create(ScaleTo::create(0.5f, 1.0f))
		),
		nullptr
	));
	_restart->runAction(Sequence::create(
		DelayTime::create(0.4f),
		Spawn::createWithTwoActions(
			FadeIn::create(0.3f),
			EaseBackOut::create(ScaleTo::create(0.5f, 1.0f))
		),
		nullptr
	));
	_menu->runAction(Sequence::create(
		DelayTime::create(0.6f),
		Spawn::createWithTwoActions(
			FadeIn::create(0.3f),
			EaseBackOut::create(ScaleTo::create(0.5f, 1.0f))
		),
		nullptr
	));
	if (_player->getLives() > 0) {
		_continue->runAction(Sequence::create(
			DelayTime::create(0.8f),
			Spawn::createWithTwoActions(
				FadeIn::create(0.3f),
				EaseBackOut::create(ScaleTo::create(0.5f, 1.0f))
			),
			nullptr
		));
	}
}

void Game::continueGame() {
	_gameOverText->runAction(Spawn::createWithTwoActions(
		FadeOut::create(0.15f),
		EaseBackIn::create(MoveTo::create(0.25f, Vec2(_overlay->getBoundingBox().size.width/2, _overlay->getBoundingBox().size.height)))
	));
	_roundsSurvived->runAction(Sequence::create(
		DelayTime::create(0.15f),
		Spawn::createWithTwoActions(
			FadeOut::create(0.1f),
			EaseBackIn::create(ScaleTo::create(0.25f, 2.0f))
		),
		nullptr
	));
	_restart->runAction(Sequence::create(
		DelayTime::create(0.2f),
		Spawn::createWithTwoActions(
			FadeOut::create(0.15f),
			EaseBackIn::create(ScaleTo::create(0.25f, 0.5f))
		),
		nullptr
	));
	_menu->runAction(Sequence::create(
		DelayTime::create(0.3f),
		Spawn::createWithTwoActions(
			FadeOut::create(0.15f),
			EaseBackIn::create(ScaleTo::create(0.25f, 0.5f))
		),
		nullptr
	));
	_continue->runAction(Sequence::create(
		DelayTime::create(0.3f),
		Spawn::createWithTwoActions(
			FadeOut::create(0.15f),
			EaseBackIn::create(ScaleTo::create(0.25f, 0.5f))
		),
		nullptr
	));
	_overlay->runAction(Sequence::create(
		DelayTime::create(0.6f),
		FadeOut::create(0.2f),
		CallFunc::create([&]() {
			_overlay->setVisible(false);
			_gameOverOverlay->setVisible(false);
			_commonOverlay->setVisible(false);
			
			_shop->setEnabled(true);
			_nodeUI->setEnabled(true);
			_pauseButton->setEnabled(true);
			
			_eventDispatcher->resumeEventListenersForTarget(_grid);
			
			_player->setEndless(true);
			_player->setGameOver(false);
			
			this->scheduleUpdate();
		}),
		nullptr
	));
}
