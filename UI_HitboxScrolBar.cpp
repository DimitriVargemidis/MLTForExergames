#include <string>
//author: Christiaan Vanbergen 

#include "UI.h"
#include "Gesture.h"

#include "UI_HitboxScrolBar.h"

extern D2D_Graphics graphics;

UI_HitboxScrolBar::UI_HitboxScrolBar() :
	Abstr_UI_HitboxSlideButton()
{
}

UI_HitboxScrolBar::UI_HitboxScrolBar(float Xcenter, float Ycenter, float width, float height, float left, float right, float up, float down, float activation, std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback, int ID_Model) :
	Abstr_UI_HitboxSlideButton{ Xcenter, Ycenter, width, height, left, right, up, down, activation, callback, ID_Model }
{
	topFiller = std::make_shared<UI_Object>(Xcenter, Ycenter - height, width_UI_Element, height, D2D1::ColorF::White);
	bottomFiller = std::make_shared<UI_Object>(Xcenter, Ycenter + height, width_UI_Element, height, D2D1::ColorF::White);

	delayMoveLimit = height_UI_Element*0.1; //the delay before the scroll starts is 10% of the height of 1 UI_element
}

UI_HitboxScrolBar::~UI_HitboxScrolBar()
{
}

void UI_HitboxScrolBar::setModel(std::shared_ptr<Model> m)
{
	Abstr_UI_Hitbox::setModel(m);
	for (int i = 0; i < UI_elements.size(); ++i)
	{
		UI_elements[i]->setModel(m);
	}
}

void UI_HitboxScrolBar::moveLeftAction(D2D1_POINT_2F ref, float move)
{

}

void UI_HitboxScrolBar::moveRightAction(D2D1_POINT_2F ref, float move)
{
}

void UI_HitboxScrolBar::moveDownAction(D2D1_POINT_2F ref, float move)
{
	//printf("moveDown =  %f \n", move);

		//if (delayDownCounter > delayLimit)
		if (delayMoveDownCounter > delayMoveLimit)
		{	
		
				for (int j = 0; j < UI_elements.size(); j++)
				{
					UI_elements[j]->moveY(0 + move);
					UI_elements[j]->moveYoriginalPos(0 + move);
		
					//Every iteration a new vector is not Ideal
					std::vector<std::shared_ptr<UI_Object>> & UI_Objects = UI_elements[j]->get_UI_Objects();

					for (int j = 0; j < UI_Objects.size(); j++)
					{
						UI_Objects[j]->moveY(0.0 + move);
					}
				}
		}
		else
		{
			delayMoveDownCounter += move;
			delayMoveUpCounter = 0;
		}
	
	
}

void UI_HitboxScrolBar::moveUpAction(D2D1_POINT_2F ref, float move)
{
	//printf("moveUp =  %f \n", move);

		if (delayMoveUpCounter > delayMoveLimit)
		{
				for (int j = 0; j < UI_elements.size(); j++)
				{
					UI_elements[j]->moveY(0 - move);
					UI_elements[j]->moveYoriginalPos(0 - move);
					 
					std::vector<std::shared_ptr<UI_Object>> & UI_Objects = UI_elements[j]->get_UI_Objects();

					for (int j = 0; j < UI_Objects.size(); j++)
					{
						UI_Objects[j]->moveY(0.0 - move);
					}

				}
			//}
		}
		else
		{
			delayMoveUpCounter += move;
			delayMoveDownCounter = 0;
		}
	
}

void UI_HitboxScrolBar::action(ActionTrigger act, const D2D1_POINT_2F & coord)
{
	switch (act)
	{
	case ActionTrigger::HoverOn:
		setLastPoint(coord);
		//(get_UI_Objects())[0]->changeColor(D2D1::ColorF::Green);
		break;
	case ActionTrigger::HoverOff:
		//(get_UI_Objects())[0]->changeColor(D2D1::ColorF::Black);
		delayMoveUpCounter = 0;
		delayMoveDownCounter = 0;
		break;
	case ActionTrigger::HoverHold:
		MoveHitbox(coord);
		setLastPoint(coord);
		break;
	case ActionTrigger::ActiveHandOn:
		action(ActionTrigger::HoverHold, coord);
		break;
	case ActionTrigger::ActiveHandOff:
		action(ActionTrigger::HoverHold, coord);
		break;

	case ActionTrigger::ActiveHandHold:
		action(ActionTrigger::HoverHold, coord);
		break;
	case ActionTrigger::ActiveHandOutsideOn:
		action(ActionTrigger::HoverOff, coord);
		break;
	case ActionTrigger::ActiveHandOutsideOff:

		break;
	case ActionTrigger::ActiveHandOutsideHold:
		break;
	}
}

void UI_HitboxScrolBar::activateFunction()
{
}

bool UI_HitboxScrolBar::isRightHandActive()
{
	return false;
}

bool UI_HitboxScrolBar::isLeftHandActive()
{
	return false;
}

void UI_HitboxScrolBar::add_UI_Element(std::shared_ptr<Abstr_UI_Hitbox> hitbox)
{

	//hitbox->setWidth(width_UI_Element);

	printf("width UI %f and width %f", width_UI_Element, getWidth());
	hitbox->setWidth(getWidth());
	hitbox->setHeigth(height_UI_Element);
	//UI_elements.push_back(hitbox);
	UI_elements.push_back(hitbox);

	getUI()->getScreen()->set_UI_GestureID(UI_elements.size());

	for (int j = 0; j < UI_elements.size(); j++)
	{
		D2D1_POINT_2F center= getCenterCoordActionArea();
	
		center.y = (center.y - getHeight() / 2) + (height_UI_Element / 2 + j*(height_UI_Element));
		center.x -=  XoffsetDisplay; 
		
	
		hitbox->setCenterCoordActionArea(center);
		hitbox->setOriginalPos(center);
		hitbox->setXoffset(0);

		//Every iteration a new vector is not Ideal
		std::vector<std::shared_ptr<UI_Object>> & UI_Objects = hitbox->get_UI_Objects();
		
		UI_Objects[0]->setCenter(center);
		UI_Objects[0]->setHeight(height_UI_Element-10);
		UI_Objects[0]->setWidth(width_UI_Element-10);

		for (int j = 1; j < UI_Objects.size(); j++)
		{
			UI_Objects[j]->setCenter(center);
			UI_Objects[j]->setHeight(height_UI_Element-30);
			UI_Objects[j]->setWidth(width_UI_Element-30);
		}

	}

}

void UI_HitboxScrolBar::setMoveUpCounter(int count)
{
	delayMoveUpCounter = count;
}

int UI_HitboxScrolBar::getMoveUpCounter()
{
	return delayMoveUpCounter;
}

void UI_HitboxScrolBar::setMoveDownCounter(int count)
{
	delayMoveDownCounter = count;
}

int UI_HitboxScrolBar::getMoveDownCounter()
{
	return delayMoveDownCounter;
}

float UI_HitboxScrolBar::getMoveLimit()
{
	return delayMoveLimit;
}

void UI_HitboxScrolBar::setHeight_UI_element(float height)
{
	height_UI_Element = height;
}

float UI_HitboxScrolBar::getHeight_UI_element()
{
	return height_UI_Element;
}

void UI_HitboxScrolBar::setWidth_UI_element(float width)
{
	width_UI_Element = width;
}

float UI_HitboxScrolBar::getWidth_UI_element()
{
	return width_UI_Element;
}

void UI_HitboxScrolBar::setActionText(std::shared_ptr<UI_TextObject> textObject)
{
	actionText = textObject;
}

std::shared_ptr<UI_TextObject> UI_HitboxScrolBar::getActionText()
{
	return actionText;
}

void UI_HitboxScrolBar::setActionIndicator(std::shared_ptr<UI_Object> actionIndic)
{
	actionIndicator = actionIndic;
}

std::shared_ptr<UI_Object> UI_HitboxScrolBar::getActionIndicator()
{
	return actionIndicator;
}

void UI_HitboxScrolBar::setTopFiller(std::shared_ptr<UI_Object> topFill)
{
	topFiller = topFill;
}

std::shared_ptr<UI_Object> UI_HitboxScrolBar::getTopFiller()
{
	return topFiller;
}

void UI_HitboxScrolBar::setBottomFiller(std::shared_ptr<UI_Object> bottomFill)
{
	bottomFiller = bottomFill;
}

std::shared_ptr<UI_Object> UI_HitboxScrolBar::getBottomFiller()
{
	return bottomFiller;
}

std::vector<std::shared_ptr<Abstr_UI_Hitbox>> & UI_HitboxScrolBar::get_UI_Elements()
{
	return UI_elements;
}

void UI_HitboxScrolBar::clear_UI_elements()
{
	UI_elements.clear();
}

void UI_HitboxScrolBar::draw()
{
	Abstr_UI_Hitbox::draw();

	if (UI_elements.size() == 0)
	{
		//printf(" no elements");
	}

	actionIndicator->draw();
	actionText->draw();

	for (int i = 0; i < UI_elements.size(); ++i)
	{
		UI_elements[i]->draw();
	}

	topFiller->draw();
	bottomFiller->draw();

}

void UI_HitboxScrolBar::attemptInteraction(D2D1_POINT_2F jointPoint, JointType type, HandState leftHand, HandState rightHand)
{
	Abstr_UI_Hitbox::attemptInteraction(jointPoint, type, leftHand, rightHand);

	for (int i = 0; i < UI_elements.size(); ++i)
	{
		UI_elements[i]->attemptInteraction(jointPoint, type, leftHand, rightHand);
	}

}
//for now it is hardcoded but the idea is to use callbackfunction on your hitboxes to define this behavior
void UI_HitboxScrolBar::updateData()
{
	Abstr_UI_Hitbox::updateData();

	for (int i = 0; i < UI_elements.size(); ++i)
	{
		UI_elements[i]->updateData();
	}
	
	/*
		//std::shared_ptr<UI> shared_ptr_this(this);
		std::shared_ptr<GestureClass> ActiveGestureClass = getModel()->getGestureClassByID(get_ID_ModelObject());
		std::vector<Gesture> gestures = ActiveGestureClass->getGestures();

		/*
		std::shared_ptr<UI_Object> background = std::make_shared<UI_Object>(550 + 250, 350, 150, 500, D2D1::ColorF::Red);
		//UI_Objects.push_back(testObject5);
		std::shared_ptr<UI_Object> selectedBox = std::make_shared<UI_Object>(550 + 250, 250, 150, 100, D2D1::ColorF::Blue);
		//UI_Objects.push_back(selectedBox);

		add_UI_Object(background);
		add_UI_Object(selectedBox);
		*/
	/*
		int gestureID;

		//for (int i = gestures.size()-1; i >= 0; i--)
		for (int i = 0; i < gestures.size(); i++)
		{
			gestureID = gestures[i].getGestureID();


			std::shared_ptr<UI_Object> testObject7 = std::make_shared<UI_Object>(400 + 250, 350, 150, 150, D2D1::ColorF::White);
			std::shared_ptr<UI_Object> text1 = std::make_shared<UI_TextObject>(400 + 250 - 30, 350, 75, 150, D2D1::ColorF::Black, std::to_wstring(i + 1), 20);
			//std::shared_ptr<UI_Object> text = std::make_shared<UI_TextObject>(400 + 250+30, 350, 75, 150, D2D1::ColorF::Black,(std::to_wstring(gestureID)),20);
			std::shared_ptr<Abstr_UI_Hitbox> testHitbox7(new UI_HitboxHoverSlideButton(400 + 250, 350, 150, 150, 0, 100, 0, 0, 0.5, UI_CallbackFunctions::deleteGesture, gestures[i].getGestureID()));

			testHitbox7->add_UI_Object(testObject7);
			//testHitbox7->add_UI_Object(text);
			testHitbox7->add_UI_Object(text1);
			testHitbox7->addInputJoint(JointType_HandLeft);
			testHitbox7->addInputJoint(JointType_HandRight);
			testHitbox7->setUI(getUI());

			add_UI_Element(testHitbox7);
		}
		*/
}



