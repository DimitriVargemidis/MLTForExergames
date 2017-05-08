
#include "Model.h"

#include "Abstr_UI_Hitbox.h"

void testCallback(int ID)
{
	printf("test callback succesfull !");
}


Abstr_UI_Hitbox::Abstr_UI_Hitbox():
widthActionArea {150}, heightActionArea{ 150 }
{
	centerCoordActionArea.x = 100;
	centerCoordActionArea.y = 200;
}

Abstr_UI_Hitbox::Abstr_UI_Hitbox(float Xcenter, float Ycenter, float width, float height):
	widthActionArea{width}, heightActionArea{ height }
{
	centerCoordActionArea.x = Xcenter;
	centerCoordActionArea.y = Ycenter;

	activateFunctionCallback = [](int) { printf("no particular action defined = wrong constructor used"); };
	ID_ModelObject = -1;
	
}

Abstr_UI_Hitbox::Abstr_UI_Hitbox(float Xcenter, float Ycenter, float width, float height, std::function<void(int)> callback, int ID_Model):
	widthActionArea{ width }, heightActionArea{ height }, ID_ModelObject{ID_Model}
{
	centerCoordActionArea.x = Xcenter;
	centerCoordActionArea.y = Ycenter;
	activateFunctionCallback = callback;
}

Abstr_UI_Hitbox::~Abstr_UI_Hitbox()
{

}

//check whether the given coordinate is within the action area of the ui object
bool Abstr_UI_Hitbox::checkCoordInside(D2D1_POINT_2F coord)
{
	if (coord.x < centerCoordActionArea.x + widthActionArea / 2 &&
		coord.x > centerCoordActionArea.x - widthActionArea / 2 &&
		coord.y < centerCoordActionArea.y + heightActionArea / 2 &&
		coord.y > centerCoordActionArea.y - heightActionArea / 2)
	{
		return true;
	}

	return false;
}

bool Abstr_UI_Hitbox::checkInputJointType(JointType type)
{
	for (int i = 0; i < inputJoints.size(); ++i)
	{
		if (inputJoints[i] == type)
		{
			return true;
		}
	}
	return false;
}

void Abstr_UI_Hitbox::setHover(bool a)
{
	hover = a;

	if (hover)
	{
		try
		{
			UI_objects[0]->changeColor(D2D1::ColorF::Red);
		}
		catch (const std::out_of_range& oor)
		{
			printf("there is no UI_object linked to this UI_hitbox");
		}
	}
	else
	{
		try
		{
			UI_objects[0]->changeColor(D2D1::ColorF::White);
		}
		catch (const std::out_of_range& oor)
		{
			printf("there is no UI_object linked to this UI_hitbox");
		}
	}

}

void Abstr_UI_Hitbox::setHover(const bool &  a, const JointType & joint, const HandState & hand, const D2D1_POINT_2F & jointPoint)
{

	//if one joint is already hovering or still active outside the object 
	//then if the calling joint is not the current joint this method returns
	//if nothing is hover or still active outside
	//then current joint becomes the hover joint
	if (hover || activeHandOutside)
	{
		if (HoverJoint != joint)
		{
			return;
		}
	}
	else
	{
		HoverJoint = joint;
	}


	if (a)//is the joint now hovering over hitbox?
	{


		if (hand == Activehandstate) //is the hand in the state that activates the hitbox
		{
			if (!activeHand) //is the hitbox not already handactivated
			{
				InertiaCounter = 0;
				//ActiveHandOnAction();
				action(ActionTrigger::ActiveHandOn, jointPoint);

				activeHand = true;
				//printf("active = true /n");
			}
			else
			{
				action(ActionTrigger::ActiveHandHold, jointPoint);
				InertiaCounter = 0;
			}


		}
		else
		{

			if (activeHand) //is the hitbox handactive already
			{
				if (InertiaCounter > InertiaLimit)
				{
					//ActiveHandOffAction();
					action(ActionTrigger::ActiveHandOff, jointPoint);

					activeHand = false;
			//		printf("active = false /n");
				}
				InertiaCounter++;
			//	printf("! ");
			}
			else if (hover)
			{
				//HoverOnAction();
				action(ActionTrigger::HoverHold, jointPoint);
			}
			else
			{
				action(ActionTrigger::HoverOn, jointPoint);
			}

		}

	}
	else
	{
		try
		{
			if (activeHand == true)
			{
				//ActiveHandOffAction();
				action(ActionTrigger::ActiveHandOutsideOn, jointPoint);
				activeHandOutside = true;
				InertiaCounter = 0;

				activeHand = false;
			//	printf("Active outside ON? \n");
			}
			
			//else if (hover)
			if(hover)
			{
				action(ActionTrigger::HoverOff, jointPoint);
			}

			if (activeHandOutside)
			{
				if (hand == Activehandstate)
				{
					InertiaCounter = 0;
					action(ActionTrigger::ActiveHandOutsideHold, jointPoint);
				//	printf("Active outside HOLD? \n");
				}
				else
				{
					if (InertiaCounter > InertiaLimit)
					{
						action(ActionTrigger::ActiveHandOutsideOff, jointPoint);
						activeHandOutside = false;
						//printf("Active outside OFF? \n");
					}
					InertiaCounter++;
					//printf("! ");
				}
			}
			//HoverOffAction();

		}
		catch (const std::out_of_range& oor)
		{
			printf("there is no UI_object linked to this UI_hitbox");
		}
	}

	hover = a;
}

bool Abstr_UI_Hitbox::getHover()
{
	return hover;
}

void Abstr_UI_Hitbox::setActiveHand(const bool & hand)
{
	activeHand = hand;
}

bool Abstr_UI_Hitbox::getActiveHand()
{
	return activeHand;
}

void Abstr_UI_Hitbox::setActiveHandOutside(const bool & hand)
{
	activeHandOutside = hand;
}

bool Abstr_UI_Hitbox::getActiveHandOutside()
{
	return activeHandOutside;
}

void Abstr_UI_Hitbox::setHoverJoint(JointType joint)
{
	HoverJoint = joint;
}

JointType Abstr_UI_Hitbox::getHoverJoint()
{
	return HoverJoint;
}

void Abstr_UI_Hitbox::HoverOnAction()
{
	UI_objects[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
}

void Abstr_UI_Hitbox::HoverOffAction()
{
	UI_objects[0]->changeColor(D2D1::ColorF::White);
}

void Abstr_UI_Hitbox::HoverHoldAction()
{
}

void Abstr_UI_Hitbox::ActiveHandOnAction()
{
	UI_objects[0]->changeColor(D2D1::ColorF::Blue); //TO DO make callback
}

void Abstr_UI_Hitbox::ActiveHandOffAction()
{
	HoverOnAction();
}

void Abstr_UI_Hitbox::ActiveHandHoldAction()
{
}
/*
void Abstr_UI_Hitbox::action(ActionTrigger action, const D2D1_POINT_2F & coord)
{
	switch (action)
	{
	case ActionTrigger::HoverOn:
		UI_objects[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
		break;
	case ActionTrigger::HoverOff:
		UI_objects[0]->changeColor(D2D1::ColorF::White);
		break;
	case ActionTrigger::HoverHold:
		break;
	case ActionTrigger::ActiveHandOn:

		activateFunction();

		UI_objects[0]->changeColor(D2D1::ColorF::Blue); //TO DO make callback
		break;
	case ActionTrigger::ActiveHandOff:

		deactivateFunction();

		UI_objects[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
		break;
	case ActionTrigger::ActiveHandHold:
		break;
	case ActionTrigger::ActiveHandOutsideOn:
		UI_objects[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
		break;
	case ActionTrigger::ActiveHandOutsideOff:
		//UI_objects[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
		break;
	case ActionTrigger::ActiveHandOutsideHold:
		//UI_objects[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
		break;
	}


}
*/

bool Abstr_UI_Hitbox::isRightHandActive()
{
	if ((handLock || activeHand || activeHandOutside) && HoverJoint == JointType_HandRight)
	{
		return true;
	}

	return false;
}

bool Abstr_UI_Hitbox::isLeftHandActive()
{
	if ((handLock || activeHand || activeHandOutside) && HoverJoint == JointType_HandLeft)
	{
		return true;
	}

	return false;
}

void Abstr_UI_Hitbox::setCenterCoordActionArea(D2D1_POINT_2F center)
{
	centerCoordActionArea = center;
	//printf("element center set \n");
}

D2D1_POINT_2F Abstr_UI_Hitbox::getCenterCoordActionArea()
{
	return centerCoordActionArea;
}

void Abstr_UI_Hitbox::setWidth(float width)
{
	widthActionArea = width;
}

float Abstr_UI_Hitbox::getWidth()
{
	return widthActionArea;
}

void Abstr_UI_Hitbox::setHeigth(float height)
{
	heightActionArea = height;
}

float Abstr_UI_Hitbox::getHeight()
{
	return heightActionArea;
}

void Abstr_UI_Hitbox::setOriginalPos(D2D1_POINT_2F pos)
{
	originalPos = pos;
}

D2D1_POINT_2F Abstr_UI_Hitbox::getOriginalPos()
{
	return originalPos;
}

void Abstr_UI_Hitbox::activateFunction()
{
	if (!functionActive)
	{
		model->setRefresh(true);
		functionActive = true;
	}
}

void Abstr_UI_Hitbox::deactivateFunction()
{
	functionActive = false;
}

void Abstr_UI_Hitbox::set_ID_ModelObject(int ID)
{
	ID_ModelObject = ID;
}

int Abstr_UI_Hitbox::get_ID_ModelObject()
{
	return ID_ModelObject;
}

void Abstr_UI_Hitbox::setFunctionActivation(bool function)
{
	functionActive = function;
}

bool Abstr_UI_Hitbox::getFunctionActivation()
{
	return functionActive;
}

void Abstr_UI_Hitbox::setHandLock(bool hand)
{
	handLock = hand;
}

bool Abstr_UI_Hitbox::getHandLock()
{
	return handLock;
}

void Abstr_UI_Hitbox::setTotalLock(bool hand)
{
	totalLock = hand;
}

bool Abstr_UI_Hitbox::getTotalLock()
{
	return totalLock;
}

void Abstr_UI_Hitbox::setVisible(bool visual)
{
	visible = visual;
}

bool Abstr_UI_Hitbox::getVisible()
{
	return visible;
}


void Abstr_UI_Hitbox::add_UI_Element(std::shared_ptr<Abstr_UI_Hitbox> hitbox)
{
}

void Abstr_UI_Hitbox::moveY(float move)
{
	centerCoordActionArea.y = centerCoordActionArea.y + move;
}

void Abstr_UI_Hitbox::moveYoriginalPos(float move)
{
	originalPos.y = originalPos.y + move;
}

void Abstr_UI_Hitbox::moveAbsY(float pos)
{
	centerCoordActionArea.y = pos;
}

void Abstr_UI_Hitbox::moveAbsYoriginalPos(float pos)
{
	originalPos.y = pos;
}


void Abstr_UI_Hitbox::setModel(std::shared_ptr<Model> m)
{
	model = m;
}

std::shared_ptr<Model> Abstr_UI_Hitbox::getModel()
{
	return model;
}

void Abstr_UI_Hitbox::add_UI_Object(std::shared_ptr<UI_Object> object)
{
	UI_objects.push_back(object);
}

void Abstr_UI_Hitbox::remove_UI_Object(std::shared_ptr<UI_Object> object)
{
	//TO DO define this function
}

std::vector<std::shared_ptr<UI_Object>> Abstr_UI_Hitbox::get_UI_Objects()
{
	return UI_objects;
}

void Abstr_UI_Hitbox::addInputJoint(JointType joint)
{
	inputJoints.push_back(joint);
}

std::vector<JointType> Abstr_UI_Hitbox::getInputJoints()
{
	return inputJoints;
}
