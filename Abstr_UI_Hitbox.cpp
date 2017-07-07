//author: Christiaan Vanbergen 

#include "Model.h"

#include "Abstr_UI_Hitbox.h"

#include "UI.h"


Abstr_UI_Hitbox::Abstr_UI_Hitbox():
widthActionArea {0}, heightActionArea{ 0 }
{
	centerCoordActionArea.x = -50;
	centerCoordActionArea.y = -50;

	activateFunctionCallback = UI_CallbackFunctions::testCallback;
	ID_ModelObject = -1;
}

Abstr_UI_Hitbox::Abstr_UI_Hitbox(float Xcenter, float Ycenter, float width, float height):
	widthActionArea{width}, heightActionArea{ height }
{
	centerCoordActionArea.x = Xcenter;
	centerCoordActionArea.y = Ycenter;

	activateFunctionCallback = UI_CallbackFunctions::testCallback;
	ID_ModelObject = -1;
	
}

std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> blabla();



Abstr_UI_Hitbox::Abstr_UI_Hitbox(float Xcenter, float Ycenter, float width, float height, std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback, int ID_Model):
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



//used by the attemptInteraction function
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
			if(!hover)				//if hand is not hovering over the hitbox
			{ 
				activeHandBeforeHover = true;
			}
			else if (!activeHand)	//is the hitbox not already handactivated
			{
				action(ActionTrigger::HoverOn, jointPoint);
			}

			if(!activeHandBeforeHover) //if the hand is already active before hovering over the button it is not activated
			{ 
				if (!activeHand) //is the hitbox not already handactivated
				{
					InertiaCounter = 0;
					action(ActionTrigger::ActiveHandOn, jointPoint);
					activeHand = true;

				}
				else
				{
					action(ActionTrigger::ActiveHandHold, jointPoint);
					InertiaCounter = 0;
				}
			}
		}
		else
		{
			activeHandBeforeHover = false;

			if (activeHand) //is the hitbox handactive already
			{
				if (InertiaCounter > InertiaLimit) 
				{
					action(ActionTrigger::ActiveHandOff, jointPoint);
					activeHand = false;
				}
				InertiaCounter++;
			}
			else if (hover) //is there already a valid joint hover over the hitbox?
			{
				action(ActionTrigger::HoverHold, jointPoint);
			}
			else
			{
				action(ActionTrigger::HoverOn, jointPoint);
			}

		}

	}
	else //if there is no joint hover over the hitbox at the moment
	{
		activeHandBeforeHover = false;
		try
		{
			if (activeHand == true) //was there a joint in handactive state before leaving the hitbox
			{
				action(ActionTrigger::ActiveHandOutsideOn, jointPoint);
				activeHandOutside = true;
				InertiaCounter = 0;

				activeHand = false;
			}
			
			if(hover) //was there a joint in handactive state before leaving the hitbox
			{
				action(ActionTrigger::HoverOff, jointPoint);
			}

			if (activeHandOutside) //is the joint that was previously inside the hitbox still in handActiveState outside of the hitbox?
			{
				if (hand == Activehandstate)
				{
					InertiaCounter = 0;
					action(ActionTrigger::ActiveHandOutsideHold, jointPoint);
				}
				else
				{
					if (InertiaCounter > InertiaLimit)
					{
						action(ActionTrigger::ActiveHandOutsideOff, jointPoint);
						activeHandOutside = false;
					}
					InertiaCounter++;
				}
			}

		}
		catch (const std::out_of_range& oor)
		{
			printf("there is no UI_object linked to this UI_hitbox");
		}
	}

	
	if (keepHoverState)			//if you don't want to change the hover state
		keepHoverState = false;
	else
		hover = a;
}

void Abstr_UI_Hitbox::setHover(bool a)
{
	hover = a;

}

bool Abstr_UI_Hitbox::getHover()
{
	return hover;
}

void Abstr_UI_Hitbox::setKeepHoverState(bool a)
{
	keepHoverState = a;
}

bool Abstr_UI_Hitbox::getKeepHoverState()
{
	return keepHoverState;
}

void Abstr_UI_Hitbox::setActiveHand(const bool & hand)
{
	activeHand = hand;
}

bool Abstr_UI_Hitbox::getActiveHand()
{
	return activeHand;
}

void Abstr_UI_Hitbox::setActiveHandBeforeHover(bool a)
{
	activeHandBeforeHover = a;
}

bool Abstr_UI_Hitbox::getActiveHandBeforeHover()
{
	return activeHandBeforeHover;
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

}

void Abstr_UI_Hitbox::deactivateFunction()
{
	functionActive = false;
}

void Abstr_UI_Hitbox::setUpdateFunction(std::function<void(Abstr_UI_Hitbox*)> funct)
{
	updateFunctionCallback = funct;
}

std::function<void(Abstr_UI_Hitbox*)> Abstr_UI_Hitbox::getupdateFunction()
{
	return updateFunctionCallback;
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

void Abstr_UI_Hitbox::setFading(bool fade)
{
	fading = fade;
}

bool Abstr_UI_Hitbox::getFading()
{
	return fading;
}

float Abstr_UI_Hitbox::getFadeProgress()
{
	return static_cast<float>(fadeCounter) / static_cast<float>(fadeLimit);
}


void Abstr_UI_Hitbox::add_UI_Element(std::shared_ptr<Abstr_UI_Hitbox> hitbox)
{
}

void Abstr_UI_Hitbox::clear_UI_elements()
{
}

std::shared_ptr<UI_Object> Abstr_UI_Hitbox::getActionIndicator()
{
	return nullptr;
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


void Abstr_UI_Hitbox::setXoffset(float offset)
{
}

float Abstr_UI_Hitbox::getXoffset()
{
	return 0.0f;
}

void Abstr_UI_Hitbox::draw()
{
	//if the fading is activated each frame the fade counter is increased and the alpha of each UI_object within this hitbox is decreased accordingly
	float fade;
	if (fading)
	{
		if(fadeCounter != fadeLimit)
			fadeCounter++;
		
		fade = 1- (static_cast<float>(fadeCounter) / static_cast<float>(fadeLimit))*0.7;
		if (fadeCounter >= fadeLimit)
		{
			activateFunction();
		}
	}
	else
	{
		fade = 1.0f;
		fadeCounter = 0;
	}
	

	for (int i = 0; i < UI_objects.size(); ++i)
	{
		if (UI_objects[i]->getVisibele())
		{
			//UI_ptr->drawRectangle(UI_objects[i]->getCenter(), UI_objects[i]->getWidth(), UI_objects[i]->getHeight(), UI_objects[i]->getColor());
			UI_objects[i]->setAlpha(fade);
			UI_objects[i]->draw();
		}
	}

}

void Abstr_UI_Hitbox::attemptInteraction(D2D1_POINT_2F jointPoint, JointType type, HandState leftHand, HandState rightHand)
{
	if (checkInputJointType(type) && !totalLock) //check if the jointtype is able to interact with the hitbox and if total lock is not on
	{
		if (checkCoordInside(jointPoint))		//check if the coordinate of the joint is inside of the hitbox
		{
	
			//NOW only works for right and left hand !
			//is the joint of type righthand or lefthand? is that hand not already busy interacting with another hitbox? 
			if (type == JointType_HandRight && !(UI_ptr->getRightHandBusy()))
			{
				setHover(true, type, rightHand, jointPoint);
				UI_ptr->setRightHandBusy(isRightHandActive());


			}
			else if (type == JointType_HandLeft && !(UI_ptr->getLeftHandBusy()))
			{
				setHover(true, type, leftHand, jointPoint);
				UI_ptr->setLeftHandBusy(isLeftHandActive());
			}

		}
		else
		{
	
			if (type == JointType_HandRight)
			{
				setHover(false, type, rightHand, jointPoint);
				UI_ptr->setRightHandBusy(isRightHandActive());

			}
			else
			{
				setHover(false, type, leftHand, jointPoint);
				UI_ptr->setLeftHandBusy(isLeftHandActive());
			}
			
		}

	}


}

void Abstr_UI_Hitbox::updateData()
{
	updateFunctionCallback(this);
}


void Abstr_UI_Hitbox::setModel(std::shared_ptr<Model> m)
{
	model = m;
}

std::shared_ptr<Model> Abstr_UI_Hitbox::getModel()
{
	return model;
}

void Abstr_UI_Hitbox::setUI(std::shared_ptr<UI> m)
{
	UI_ptr = m;
}

std::shared_ptr<UI> Abstr_UI_Hitbox::getUI()
{
	return UI_ptr;
}

void Abstr_UI_Hitbox::add_UI_Object(std::shared_ptr<UI_Object> object)
{
	UI_objects.push_back(object);
}

//not used
void Abstr_UI_Hitbox::remove_UI_Object(std::shared_ptr<UI_Object> object)
{
	//TO DO define this function
}

std::vector<std::shared_ptr<UI_Object>> & Abstr_UI_Hitbox::get_UI_Objects()
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
