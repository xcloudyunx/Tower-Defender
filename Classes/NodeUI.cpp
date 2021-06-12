#include "NodeUI.h"

static NodeUI* _instance = nullptr;

void NodeUI::Reset() {
	_instance = nullptr;
}

NodeUI* NodeUI::getInstance() {
	if (!_instance) {
		_instance = new (std::nothrow) NodeUI;
		_instance->init();
    }
	return _instance;
}

bool NodeUI::init() {
	_visibleSize = _director->getVisibleSize();
	
	_buildManager = BuildManager::getInstance();
	
	
	this->setAnchorPoint(Vec2(0.5f, 0.0f));
	this->setPosition(Vec2(_visibleSize.width/2, 0.0f));
	
	_upgrade = Button::create("Buttons/button.png", "Buttons/buttonSelected.png", "Buttons/buttonDisabled.png");
	_upgrade->setTitleFontName("Azonix.ttf");
	_upgrade->setTitleColor(Color3B(0, 0, 0));
	_upgrade->setTitleText("UPGRADE: $");
	_upgrade->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		auto label = _upgrade->getTitleLabel();
		switch(type) {
			case Widget::TouchEventType::BEGAN:
				label->setPosition(label->getPosition()-Vec2(0, 2));
				break;
			case Widget::TouchEventType::ENDED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				_buildManager->upgradeTurret(std::stoi(label->getString().substr(10)));
				this->Hide();
				break;
			case Widget::TouchEventType::CANCELED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				break;
			default:
				break;
		}
	});
	_upgrade->setPosition(Vec2(_visibleSize.width/3, _visibleSize.height/12));
	this->addChild(_upgrade);
	
	_sell = Button::create("Buttons/button.png", "Buttons/buttonSelected.png", "Buttons/buttonDisabled.png");
	_sell->setTitleFontName("Azonix.ttf");
	_sell->setTitleColor(Color3B(0, 0, 0));
	_sell->setTitleText("SELL: $");
	_sell->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		auto label = _sell->getTitleLabel();
		switch(type) {
			case Widget::TouchEventType::BEGAN:
				label->setPosition(label->getPosition()-Vec2(0, 2));
				break;
			case Widget::TouchEventType::ENDED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				_buildManager->sellTurret(std::stoi(label->getString().substr(7)));
				this->Hide();
				break;
			case Widget::TouchEventType::CANCELED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				break;
			default:
				break;
		}
	});
	_sell->setPosition(Vec2(_visibleSize.width*2/3, _visibleSize.height/12));
	this->addChild(_sell);
	
	this->setScale(0.1f);
	this->setVisible(false);
	_isVisible = false;
	
	return true;
}

void NodeUI::setUpgradeCost(const std::string c) {
	_upgrade->setTitleText("UPGRADE: "+c);
}

void NodeUI::setSellCost(const int c) {
	_sell->setTitleText("SELL: $"+std::to_string(c));
}

void NodeUI::setEnabled(const bool e) {
	_upgrade->setEnabled(e);
	_sell->setEnabled(e);
}

void NodeUI::disableUpgrade() {
	_upgrade->setEnabled(false);
}

void NodeUI::enableUpgrade() {
	_upgrade->setEnabled(true);
}

void NodeUI::Hide() {
	_isVisible = false;
	this->runAction(Sequence::create(
		Spawn::createWithTwoActions(
			EaseBackOut::create(ScaleTo::create(0.1f, 0.1f)),
			MoveTo::create(0.1f, _prevPosition)
		),
		CallFunc::create([&] {
			this->setVisible(false);
		}),
		nullptr
	));
}

void NodeUI::Show() {
	_isVisible = true;
	_upgrade->setEnabled(true);
	_prevPosition = this->getPosition();
	this->setVisible(true);
	this->setScale(0.1f);
	this->runAction(Spawn::createWithTwoActions(
		EaseBackOut::create(ScaleTo::create(0.2f, 1.0f)),
		MoveTo::create(0.1f, Vec2(_visibleSize.width/2, 0.0f))
	));
}

bool NodeUI::isVisible() {
	return _isVisible;
}