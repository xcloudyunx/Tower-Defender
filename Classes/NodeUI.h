#ifndef __NODEUI_H__
#define __NODEUI_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include "BuildManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class NodeUI : public Node {
public:
	static void Reset();
	static NodeUI* getInstance();
	
	virtual bool init();
	
	void setUpgradeCost(const std::string c);
	void setSellCost(const int c);
	
	void setEnabled(const bool e);
	
	void disableUpgrade();
	void enableUpgrade();
	
	void Hide();
	void Show();
	bool isVisible();
	
private:
	Size _visibleSize;

	BuildManager* _buildManager;

	Button* _upgrade;
	Button* _sell;
	
	bool _isVisible;
	
	Vec2 _prevPosition;
};

#endif // __NODEUI_H__
