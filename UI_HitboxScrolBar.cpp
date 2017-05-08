#include "UI_HitboxScrolBar.h"



UI_HitboxScrolBar::UI_HitboxScrolBar() :
	Abstr_UI_HitboxSlideButton()
{
}

UI_HitboxScrolBar::UI_HitboxScrolBar(float Xcenter, float Ycenter, float width, float height, float left, float right, float up, float down, float activation, std::function<void(int)> callback, int ID_Model):
Abstr_UI_HitboxSlideButton {Xcenter, Ycenter, width, height, left, right, up, down, activation, callback, ID_Model}
{
	delayMoveLimit = height_UI_Element*0.25; //the delay before the scroll starts is 75% of the height of 1 UI_element
}

UI_HitboxScrolBar::~UI_HitboxScrolBar()
{
}

void UI_HitboxScrolBar::moveLeftAction(D2D1_POINT_2F ref, float move)
{

}

void UI_HitboxScrolBar::moveRightAction(D2D1_POINT_2F ref, float move)
{
}

void UI_HitboxScrolBar::moveDownAction(D2D1_POINT_2F ref, float move)
{


		//if (delayDownCounter > delayLimit)
		if (delayMoveDownCounter > delayMoveLimit)
		{	
			/*
			auto timeNow = Clock::now();
			auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - TimeLastMoveUp).count();
			printf("DOWN time is %ld \n", diff);
			if (diff > delayTimeAfterMoveLimit)
			{
				TimeLastMoveDown = timeNow;
				*/
				//printf("ACTIVE delayMoveDownCounter = %f \n", delayMoveDownCounter);
				for (int j = 0; j < UI_elements.size(); j++)
				{
					/*
					D2D1_POINT_2F center = UI_elements[j]->getOriginalPos();
					D2D1_POINT_2F current = UI_elements[j]->getCenterCoordActionArea();

					center.y = center.y + move;
					current.y = center.y;

					UI_elements[j]->setCenterCoordActionArea(current);
					UI_elements[j]->setOriginalPos(center);
					*/

					UI_elements[j]->moveY(0 + move);
					UI_elements[j]->moveYoriginalPos(0 + move);
			//		printf("MOVED DOWN \n", diff);

					//Every iteration a new vector is not Ideal
					std::vector<std::shared_ptr<UI_Object>> UI_Objects = UI_elements[j]->get_UI_Objects();

					for (int j = 0; j < UI_Objects.size(); j++)
					{
						//UI_Objects[j]->setCenter(center);
						UI_Objects[j]->moveY(0.0 + move);
					}

				//}
			}
		}
		else
		{
			delayMoveDownCounter += move;
			delayMoveUpCounter = 0;
			//printf("delayMoveDownCounter = %f \n", delayMoveDownCounter);
			//delayDownCounter++;
			//delayUpCounter = 0;
		}
	
	
}

void UI_HitboxScrolBar::moveUpAction(D2D1_POINT_2F ref, float move)
{
	
	
		//if (delayUpCounter > delayLimit)
		if (delayMoveUpCounter > delayMoveLimit)
		{
			/*
			auto timeNow = Clock::now();
			auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - TimeLastMoveDown).count();
			printf("UP time is %ld \n", diff);
			if (diff > delayTimeAfterMoveLimit)
			{
				TimeLastMoveUp = timeNow;
				*/
				//printf("ACTIVE delayMoveUpCounter = %f\n", delayMoveUpCounter);
				for (int j = 0; j < UI_elements.size(); j++)
				{
					//D2D1_POINT_2F center = UI_elements[j]->getOriginalPos();
					//D2D1_POINT_2F current = UI_elements[j]->getCenterCoordActionArea();

					//center.y = center.y - move;
					//current.y = center.y;

					//UI_elements[j]->setCenterCoordActionArea(current);
					//UI_elements[j]->setOriginalPos(center);

					UI_elements[j]->moveY(0 - move);
					UI_elements[j]->moveYoriginalPos(0 - move);
				//	printf("MOVED UP \n", diff);

					//Every iteration a new vector is not Ideal
					std::vector<std::shared_ptr<UI_Object>> UI_Objects = UI_elements[j]->get_UI_Objects();

					for (int j = 0; j < UI_Objects.size(); j++)
					{
						//UI_Objects[j]->setCenter(center);
						UI_Objects[j]->moveY(0.0 - move);
					}

				}
			//}
		}
		else
		{
			//delayUpCounter++;
			//delayDownCounter = 0;
			delayMoveUpCounter += move;
			delayMoveDownCounter = 0;
			//printf("delayMoveUpCounter = %f\n", delayMoveUpCounter);
		}
	
}

void UI_HitboxScrolBar::action(ActionTrigger act, const D2D1_POINT_2F & coord)
{
	switch (act)
	{
	case ActionTrigger::HoverOn:
		setLastPoint(coord);
	//	(get_UI_Objects())[0]->changeColor(D2D1::ColorF::Gray); //TO DO make callback
		break;
	case ActionTrigger::HoverOff:
		//(get_UI_Objects())[0]->changeColor(D2D1::ColorF::White);
		//delayUpCounter = 0;
		//delayDownCounter = 0;
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

	hitbox->setWidth(width_UI_Element/2);
	hitbox->setHeigth(height_UI_Element);
	//UI_elements.push_back(hitbox);
	UI_elements.push_back(hitbox);

	for (int j = 0; j < UI_elements.size(); j++)
	{
		D2D1_POINT_2F center= getCenterCoordActionArea();
	
		center.y = (center.y - getHeight() / 2) + (height_UI_Element / 2 + j*height_UI_Element);
		center.x -=  XoffsetDisplay; 
		
	
		hitbox->setCenterCoordActionArea(center);
		hitbox->setOriginalPos(center);

		//Every iteration a new vector is not Ideal
		std::vector<std::shared_ptr<UI_Object>> & UI_Objects = hitbox->get_UI_Objects();
		
		for (int j = 0; j < UI_Objects.size(); j++)
		{
			UI_Objects[j]->setCenter(center);
			UI_Objects[j]->setHeight(height_UI_Element-10);
			UI_Objects[j]->setWidth(width_UI_Element);
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

std::vector<std::shared_ptr<Abstr_UI_Hitbox>> UI_HitboxScrolBar::get_UI_Elements()
{
	return UI_elements;
}



