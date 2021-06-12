#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "Player.h"
#include "EnemyManager.h"
#include "BuildManager.h"

#include "NodeUI.h"

#include "Enemy.h"

#include "Turret.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class Game : public Scene {
public:
	static Scene* createScene();

	virtual bool init();
    
	CREATE_FUNC(Game);
	
private:
	Size _visibleSize;
	

	Player* _player;
	BuildManager* _buildManager;
	EnemyManager* _enemyManager;
	std::list<Enemy*>* _enemies;
	std::vector<Sprite*>* _path;
	
	
	int _currentRound;
	
	
	TMXTiledMap* _grid;
	TMXLayer* _layer;
	
	
	int _turretTypes = 3;
	
	
	Menu* _shop;
	MenuItemLabel* _money;
	MenuItemLabel* _lives;
	std::vector<MenuItem*> _shopItems;
	
	
	Sprite* _overlay;
	
	Node* _gameOverOverlay;
	Label* _gameOverText;
	Label* _roundsSurvived;
	Label* _roundsSurvivedText;
	Button* _continue;
	
	Node* _pauseOverlay;
	Label* _pauseText;
	Button* _resume;
	
	Node* _commonOverlay;
	Button* _restart;
	Button* _menu;
	
	Button* _pauseButton;
	Vector<Node*> _pausedActions;
	
	
	NodeUI* _nodeUI;
	
	
	void update(float dt);
	
	
	bool generateMap(const Vec2 currentCo, Vec2 prevDir, std::vector<std::vector<bool>> &validity);
	bool validCo(const Vec2 currentCo);
	
	
	void buildTurret(const int i, const int j);
	void selectTurret(const int i, const int j);
	void resetTurret();
	
	
	void pauseGame();
	void resumeGame();
	void endGame();
	void continueGame();
};

#endif // __GAME_SCENE_H__
