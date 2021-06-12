#include "MainMenuScene.h"

Scene* MainMenu::createScene() {
	return MainMenu::create();
}

bool MainMenu::init() {
	if (!Scene::init()){
		return false;
	}
	
	return true;
}

void MainMenu::onEnter() {
	Scene::onEnter();
	
	this->removeAllChildren();
	
	auto visibleSize = _director->getVisibleSize();
	Vec2 origin = _director->getVisibleOrigin();
	
	auto turret = Turret::create();
	turret->unscheduleUpdate();
	turret->setName("turret");
	turret->setCascadeOpacityEnabled(true);
	turret->setOpacity(0.0f);
	turret->setTruePosition(visibleSize/2);
	this->addChild(turret);
	turret->runAction(Sequence::create(
		DelayTime::create(0.6f),
		ScaleTo::create(0.0f, 0.01f),
		Spawn::createWithTwoActions(
			Sequence::create(
				FadeIn::create(0.3f),
				CallFunc::create([&]() {
					auto turret = dynamic_cast<Turret*>(this->getChildByName("turret"));
					turret->getHead()->runAction(RepeatForever::create(RotateBy::create(1.0f, 90.0f)));
				}),
				nullptr
			),
			EaseBackOut::create(ScaleTo::create(0.5f, 1.0f))
		),
		nullptr
	));
	
	auto title = Label::createWithTTF("TOWER DEFENDER", "Azonix.ttf", 70.0f);
	title->setOpacity(0.0f);
	title->setPosition(Vec2(visibleSize.width/2, visibleSize.height));
	this->addChild(title);
	title->runAction(Spawn::createWithTwoActions(
		FadeIn::create(0.3f),
		EaseBackOut::create(MoveTo::create(0.5f, Vec2(visibleSize.width/2, visibleSize.height*3/4)))
	));
	
	auto play = Button::create("Buttons/button.png", "Buttons/buttonSelected.png");
	play->setCascadeOpacityEnabled(true);
	play->setOpacity(0.0f);
	play->setScale(0.5f);
	play->setTitleFontName("Azonix.ttf");
	play->setTitleColor(Color3B(0, 0, 0));
	play->setTitleText("PLAY");
	play->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		auto button = dynamic_cast<Button*>(sender);
		auto label = button->getTitleLabel();
		switch(type) {
			case Widget::TouchEventType::BEGAN:
				label->setPosition(label->getPosition()-Vec2(0, 2));
				break;
			case Widget::TouchEventType::ENDED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				_director->pushScene(TransitionFade::create(0.5f, Game::createScene()));
				break;
			case Widget::TouchEventType::CANCELED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				break;
			default:
				break;
		}
	});
	play->setPosition(Vec2(visibleSize.width*(4.0f+sqrt(3.0f))/8, visibleSize.height*3/8));
	this->addChild(play);
	play->runAction(Sequence::create(
		DelayTime::create(0.2f),
		Spawn::createWithTwoActions(
			FadeIn::create(0.3f),
			EaseBackOut::create(ScaleTo::create(0.5f, 1.0f))
		),
		nullptr
	));
	
	auto exit = Button::create("Buttons/button.png", "Buttons/buttonSelected.png");
	exit->setCascadeOpacityEnabled(true);
	exit->setOpacity(0.0f);
	exit->setScale(0.5f);
	exit->setTitleFontName("Azonix.ttf");
	exit->setTitleColor(Color3B(0, 0, 0));
	exit->setTitleText("EXIT");
	exit->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
		auto button = dynamic_cast<Button*>(sender);
		auto label = button->getTitleLabel();
		switch(type) {
			case Widget::TouchEventType::BEGAN:
				label->setPosition(label->getPosition()-Vec2(0, 2));
				break;
			case Widget::TouchEventType::ENDED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				_director->end();
				break;
			case Widget::TouchEventType::CANCELED:
				label->setPosition(label->getPosition()+Vec2(0, 2));
				break;
			default:
				break;
		}
	});
	exit->setPosition(Vec2(visibleSize.width*(4.0f-sqrt(3.0f))/8, visibleSize.height*3/8));
	this->addChild(exit);
	exit->runAction(Sequence::create(
		DelayTime::create(0.4f),
		Spawn::createWithTwoActions(
			FadeIn::create(0.3f),
			EaseBackOut::create(ScaleTo::create(0.5f, 1.0f))
		),
		nullptr
	));
}