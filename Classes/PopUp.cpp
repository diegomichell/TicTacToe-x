#include "PopUp.h"

USING_NS_CC;

Scene* PopUp::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = PopUp::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool PopUp::init()
{
	//////////////////////////////
	// 1. super init first
	if (!PopUp::initWithColor(Color4B(255, 102, 0, 255)))
	{
		return false;
	}

	this->setVisible(false);

	ignoreAnchorPointForPosition(false);
	this->setAnchorPoint(Vect(0.5, 0.5));
	this->setContentSize(Size(300,200));
		
	this->titleLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 32);
	this->titleLabel->setTextColor(Color4B(255, 255, 255, 255));

	this->messageLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 32);
	this->messageLabel->setTextColor(Color4B(255, 255, 255, 255));

	button = ui::Button::create("option.png", "option.png");

	Size dialogSize = this->getContentSize();

	titleLabel->setPosition(Vec2(dialogSize.width / 2, dialogSize.height - 20));
	
	messageLabel->setPosition(Vec2(dialogSize.width / 2, dialogSize.height - 70));

	button ->setPosition(Vec2(dialogSize.width / 2, dialogSize.height - 150));

	this->addChild(titleLabel);
	this->addChild(messageLabel);
	this->addChild(button);

	return true;
}

void PopUp::setTitle(std::string title)
{
	titleLabel->setString(title);
}

void PopUp::setMessage(std::string message)
{
	messageLabel->setString(message);
}
