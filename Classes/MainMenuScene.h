#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "GameScene.h"
#include "Turret.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class MainMenu : public Scene {
public:
	static Scene* createScene();

	virtual bool init();
    
	CREATE_FUNC(MainMenu);
	
	virtual void onEnter();
	
private:

};

#endif // __MAINMENU_SCENE_H__
