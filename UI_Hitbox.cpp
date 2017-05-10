
#include "Model.h"
#include "UI.h"

#include "UI_Hitbox.h"

UI_Hitbox::UI_Hitbox(): 
	Abstr_UI_Hitbox()
{
	
}


UI_Hitbox::UI_Hitbox(float Xcenter, float Ycenter, float width, float height, std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback, int ID_Model):
	Abstr_UI_Hitbox(Xcenter, Ycenter, width, height,callback,ID_Model)
{
}

UI_Hitbox::~UI_Hitbox()
{
}
/*
//check whether the given coordinate is within the action area of the ui object
bool UI_Hitbox::checkCoordInside(D2D1_POINT_2F coord)
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

bool UI_Hitbox::checkInputJointType(JointType type)
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

void UI_Hitbox::setHover(bool a)
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

void UI_Hitbox::setHover(const bool &  a, const JointType & joint, const HandState & hand, const D2D1_POINT_2F & jointPoint)
{
	
	//if one joint is already hovering or still active outside the object 
	//then if the calling joint is not the current joint this method returns
	//if nothing is hover or still active outside
	//then current joint becomes the hover joint
	if(hover || activeHandOutside) 
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
		

		if (hand == Activehandstate ) //is the hand in the state that activates the hitbox
		{
			if (!activeHand) //is the hitbox not already handactivated
			{
				InertiaCounter = 0;
				//ActiveHandOnAction();
				action(ActionTrigger::ActiveHandOn, jointPoint);

				activeHand = true;
				printf("active = true /n");
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
					printf("active = false /n");
				}
				InertiaCounter++;
				printf("! ");
			}
			else if(hover)
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
				printf("Active outside ON? \n");
			}
			else if (hover)
			{
				action(ActionTrigger::HoverOff, jointPoint);
			}

			if (activeHandOutside)
			{
				if (hand == Activehandstate)
				{
					InertiaCounter = 0;
					action(ActionTrigger::ActiveHandOutsideHold, jointPoint);
					printf("Active outside HOLD? \n");
				}
				else
				{
					if (InertiaCounter > InertiaLimit)
					{
						action(ActionTrigger::ActiveHandOutsideOff, jointPoint);
						activeHandOutside = false;
						printf("Active outside OFF? \n");
					}
					InertiaCounter++;
					printf("! ");
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

bool UI_Hitbox::getHover()
{
	return hover;
}

void UI_Hitbox::setHoverJoint(JointType joint)
{
	HoverJoint = joint;
}

JointType UI_Hitbox::getHoverJoint()
{
	return HoverJoint;
}

void UI_Hitbox::HoverOnAction()
{
	UI_objects[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
}

void UI_Hitbox::HoverOffAction()
{
	UI_objects[0]->changeColor(D2D1::ColorF::White);
}

void UI_Hitbox::HoverHoldAction()
{
}

void UI_Hitbox::ActiveHandOnAction()
{
	UI_objects[0]->changeColor(D2D1::ColorF::Blue); //TO DO make callback
}

void UI_Hitbox::ActiveHandOffAction()
{
	HoverOnAction();
}

void UI_Hitbox::ActiveHandHoldAction()
{
}
*/
void UI_Hitbox::action(ActionTrigger action, const D2D1_POINT_2F & coord)
{
	switch (action)
	{
	case ActionTrigger::HoverOn:
		get_UI_Objects()[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
		break;
	case ActionTrigger::HoverOff:
		get_UI_Objects()[0]->changeColor(D2D1::ColorF::White);
		break;
	case ActionTrigger::HoverHold:
		break;
	case ActionTrigger::ActiveHandOn:

		activateFunction();

		get_UI_Objects()[0]->changeColor(D2D1::ColorF::Blue); //TO DO make callback
		break;
	case ActionTrigger::ActiveHandOff:
	
		deactivateFunction();

		get_UI_Objects()[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
		break;
	case ActionTrigger::ActiveHandHold:
		break;
	case ActionTrigger::ActiveHandOutsideOn:
		get_UI_Objects()[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
		break;
	case ActionTrigger::ActiveHandOutsideOff:
		//UI_objects[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
		break;
	case ActionTrigger::ActiveHandOutsideHold:
		//UI_objects[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
		break;
	}


}
/*
bool UI_Hitbox::isRightHandActive()
{
	if ((activeHand || activeHandOutside) && HoverJoint == JointType_HandRight)
	{
		return true;
	}

	return false;
}

bool UI_Hitbox::isLeftHandActive()
{
	if ((activeHand || activeHandOutside) && HoverJoint == JointType_HandLeft)
	{
		return true;
	}

	return false;
}

void UI_Hitbox::setCenterCoordActionArea(D2D1_POINT_2F center)
{
	centerCoordActionArea = center;
	//printf("element center set \n");
}

D2D1_POINT_2F UI_Hitbox::getCenterCoordActionArea()
{
	return centerCoordActionArea;
}
*/
void UI_Hitbox::activateFunction()
{
	if (!getFunctionActivation())
	{
			setFunctionActivation(true);
			activateFunctionCallback(get_ID_ModelObject(),0, getModel(),getUI());
	}
}
void UI_Hitbox::setLastPoint(D2D1_POINT_2F pos)
{

}
D2D1_POINT_2F UI_Hitbox::getLastPoint()
{
	return D2D1_POINT_2F();
}
/*
void UI_Hitbox::deactivateFunction()
{
	functionActive = false;
}



void UI_Hitbox::setFunctionActivation(bool function)
{
	functionActive = function;
}

bool UI_Hitbox::getFunctionActivation()
{
	return functionActive;
}




void UI_Hitbox::setModel(std::shared_ptr<Model> m)
{
	model = m;
}

std::shared_ptr<Model> UI_Hitbox::getModel()
{
	return model;
}

void UI_Hitbox::add_UI_Object(std::shared_ptr<UI_Object> object)
{
	UI_objects.push_back(object);
}

void UI_Hitbox::remove_UI_Object(std::shared_ptr<UI_Object> object)
{
	//TO DO define this function
}

std::vector<std::shared_ptr<UI_Object>> UI_Hitbox::get_UI_Objects()
{
	return UI_objects;
}

void UI_Hitbox::addInputJoint(JointType joint)
{
	inputJoints.push_back(joint);
}

std::vector<JointType> UI_Hitbox::getInputJoints()
{
	return inputJoints;
}
*/
