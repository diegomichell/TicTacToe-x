#include "cocos2d.h"
#include "ui/UIButton.h"

/**
Created by: Diego Ivan Perez Michel
*/

class PopUp : public cocos2d::LayerColor
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();


	// implement the "static create()" method manually
	CREATE_FUNC(PopUp);

	void setTitle(std::string title);
	void setMessage(std::string message);
	cocos2d::ui::Button* button;

private:
	cocos2d::Label* titleLabel;
	cocos2d::Label* messageLabel;

};