#include "HelloWorldScene.h"
#include <iostream>
#include <stdio.h>

USING_NS_CC;

/**
	Created by: Diego Ivan Perez Michel
*/

// TAGS: 0 => Celda Vacia, 1 => Equis, 2 => Circulo

Scene* HelloWorld::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!HelloWorld::initWithColor(Color4B(255, 255, 255, 255)))
	{	// 'scene' is an autorelease object
		auto scene = Scene::create();
    
		// 'layer' is an autorelease object
		auto layer = HelloWorld::create();

		// add layer as a child to scene
		scene->addChild(layer);

		return false;
	}

	//El primer jugador utilizara la equis. 1 = equis, 2 = circulo
	_player = 1;

	auto director = Director::getInstance();
	auto winSize = director->getWinSize();

	this->popUp = PopUp::create();

	//Dialog en el centro de la pantalla
	popUp->setPosition(winSize.width / 2, winSize.height / 2);
	popUp->button->addTouchEventListener(CC_CALLBACK_2(HelloWorld::dialogEvent, this));

	this->addChild(popUp, 2);

	/*Dirty Area, Codigo sucio temporal, PopUp distinto en caso de celda ocupada*/

	this->continuePopUp = PopUp::create();
	continuePopUp->setPosition(winSize.width / 2, winSize.height / 2);
	continuePopUp->removeChild(continuePopUp->button);
	continuePopUp->button = ui::Button::create("continueOption.png", "continueOption.png");
	Size dialogSize = continuePopUp->getContentSize();
	continuePopUp->button ->setPosition(Vec2(dialogSize.width / 2, dialogSize.height - 150));
	continuePopUp->addChild(continuePopUp->button);

	continuePopUp->button->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			continuePopUp->setVisible(false);
			isPlayable = true;
		}
	});

	this->addChild(continuePopUp, 2);

	/****/
	

	initCells();
	listenForTouch();

	auto backgroundSprite = Sprite::create("background.png");
	backgroundSprite->setPosition(winSize.width / 2, winSize.height / 2);

	auto titleLabel = Label::createWithTTF("TicTacToe", "fonts/Marker Felt.ttf", 32);
	titleLabel->setTextColor(Color4B(120,150,120,120));
	
	titleLabel->setPosition(Vec2(winSize.width / 2,winSize.height - 70));

	auto titleMovBy = MoveBy::create(2, Vec2(0,35));
	titleLabel->runAction(titleMovBy);

	addChild(backgroundSprite, 0);
	addChild(titleLabel, 1);
    

    return true;
}

void HelloWorld::initCells()
{
	int x = 50, y = 120;
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			auto cell = _cells[i][j] = Sprite::create("blank.png");
			cell->setScale(2.5);
			cell->setAnchorPoint(Vec2::ZERO);
			cell->setPosition(Vec2(x, y));

			//Se le anade una etiqueta para identificar el status, 0 = vacio, 1 = equis , 2= circulo;
			cell->setTag(0);

			this->addChild(cell, 1);

			x += 150;
		}
		
		y += 190;
		x = 50;
	}

}


bool HelloWorld::playerDidWin()
{
	auto winningCell = _cells[0][0];

	// Verificacion Horizontal
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// Si la celda esta vacia
			if (_cells[i][j]->getTag() == 0)
			{
				break;
			}

			// Verifico que j no este en la ultima celda de la fila y si no es asi j es incrementada en 1 para optener la celda que le sigue
			if (_cells[i][(j < 2) ? j + 1: j] ->getTag() != _cells[i][j]->getTag())
			{
				break;
			}

			if (j == 2)
			{
				return true;
			}

		}
	}

	// Verificacion Vertical
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// Si la celda esta vacia
			if (_cells[j][i]->getTag() == 0)
			{
				break;
			}

			// Verifico que j no este en la ultima celda de la columna y si no es asi j es incrementada en 1 para optener la celda que le sigue
			if (_cells[(j < 2) ? j + 1 : j][i]->getTag() != _cells[j][i]->getTag())
			{
				break;
			}


			if (j == 2)
			{
				return true;
			}

		}
	}


	winningCell = _cells[0][0];
	// Verificacion Diagonal (Izquielda a Derecha)
	for (int i = 0; i < 3; i++)
	{
		if (_cells[i][i]->getTag() == 0)
		{
			break;
		}

		if (winningCell -> getTag() != _cells[i][i] -> getTag())
		{
			break;
		}

		winningCell = _cells[i][i];

		if (i == 2)
		{
			return true;
		}
	}


	winningCell = _cells[0][2];
	// Verificacion Diagonal (Derecha a Izquierda)
	for (int i = 2; i >= 0; i--)
	{
		if (_cells[2 - i][i]->getTag() == 0)
		{
			break;
		}

		if (winningCell->getTag() != _cells[2 - i][i]->getTag())
		{
			break;
		}

		winningCell = _cells[2 - i][i];

		if (i == 0)
		{
			return true;
		}
	}

	return false;
}

bool HelloWorld::isDraw()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (_cells[i][j]->getTag() == 0)
			{
				return false;
			}
		}
	}

	return true;
}


void HelloWorld::listenForTouch()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* touch, Event* event) {

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				auto cell = _cells[i][j];
				Rect rect = cell->getBoundingBox();

				if (isPlayable)
				{ 
					
					//Comprobar si se ha tocado  una celda
					if (rect.containsPoint(touch->getLocation()))
					{
						// Verifico que la celda este vacia
						if (cell->getTag() == 0)
						{

							// Identifico cual jugador hizo la jugada 
							if (_player == 1)
							{
								//Remplaza la imagen del sprite
								cell->setSpriteFrame(SpriteFrame::create("x.png", Rect(0, 0, 32, 32)));
								_player = 2;
								cell->setTag(1);
							}
							else
							{
								//Remplaza la imagen del sprite
								cell->setSpriteFrame(SpriteFrame::create("circle.png", Rect(0, 0, 32, 32)));
								_player = 1;
								cell->setTag(2);
							}


							if (playerDidWin())
							{
								std::ostringstream ss;

								// Invertir, El jugador que no ha perdido
								_player = (_player == 1) ? 2 : 1;

								ss << "El jugador " << _player << " ha ganado";
								std::string message = ss.str();

								showDialog("Juego Terminado", message.c_str());

							}
							else if (isDraw())
							{
								showDialog("Juego Terminado","Hubo un empate");
							}

						}
						else
						{
							//Dirty Code
							continuePopUp->setTitle("No permitido");
							continuePopUp->setMessage("Celda no disponible");
							continuePopUp->setVisible(true);
							isPlayable = false;
						}

						return true;
					}
				
				}
				

			}
		}

		return false; 
	
	};

	listener->onTouchMoved = [](Touch* touch, Event* event) {};
	listener->onTouchEnded = [=](Touch* touch, Event* event) {};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


void HelloWorld::resetGame()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			_cells[i][j]->setSpriteFrame(SpriteFrame::create("blank.png", Rect(0, 0, 32, 32)));
			_cells[i][j]->setTag(0);
		}
	}
}

void HelloWorld::showDialog(std::string title, std::string message)
{
	popUp->setTitle(title);
	popUp->setMessage(message);
	popUp->setVisible(true);
	isPlayable = false;
	
};


void HelloWorld::dialogEvent(Ref* ref, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		popUp->setVisible(false);
		resetGame();
		isPlayable = true;
		break;
	case ui::Widget::TouchEventType::MOVED:
		// TODO
		break;
	case ui::Widget::TouchEventType::ENDED:
		// TODO
		break;
	case ui::Widget::TouchEventType::CANCELED:
		// TODO
		break;
	default:
		// TODO
		break;
	}
};