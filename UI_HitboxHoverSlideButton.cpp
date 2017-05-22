
#include <string>

#include "UI.h"

#include "UI_HitboxHoverSlideButton.h"

extern D2D1::ColorF selectedboxColor;

UI_HitboxHoverSlideButton::UI_HitboxHoverSlideButton()
{
}

UI_HitboxHoverSlideButton::UI_HitboxHoverSlideButton(float Xcenter, float Ycenter, float width, float height, float left, float right, float up, float down, float activation, std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback, int ID_Model, float XImageOffset ):
	Abstr_UI_HitboxSlideButton{ Xcenter, Ycenter, width, height, left, right, up, down, activation, callback, ID_Model }, orginalWidth{ width }, Xoffset{ XImageOffset }
{
}



UI_HitboxHoverSlideButton::~UI_HitboxHoverSlideButton()
{



}

void UI_HitboxHoverSlideButton::action(ActionTrigger act, const D2D1_POINT_2F & coord)
{
	std::vector<std::shared_ptr<UI_Object>> & UI_Objects = get_UI_Objects();
	D2D1_POINT_2F imagePos;
	volatile float percentage = 0.0f;		//the percentage untill activation

	switch (act)
	{
	case ActionTrigger::HoverOn:
		setLastPoint(coord);
		//(get_UI_Objects())[0]->changeColor(D2D1::ColorF::Green); 
		UI_Objects[0]->changeColor(D2D1::ColorF::Black);
		(get_UI_Objects())[0]->setObjectState(ObjectState::hover);

		getUI()->getScreen()->getScrollbar()->getSelectionBox()->changeColor(D2D1::ColorF::Green);
		getUI()->getScreen()->getPlayVisual()->get_UI_Objects()[0]->changeBorderColor(D2D1::ColorF::Green);

		getUI()->getScreen()->autoplayGesture(get_ID_ModelObject());

		break;
	case ActionTrigger::HoverOff:
		
		setCenterCoordActionArea(getOriginalPos());
		setWidth(orginalWidth);

		deactivateFunction();

		UI_Objects[0]->changeColor(D2D1::ColorF::Black);
		UI_Objects[1]->changeColor(D2D1::ColorF::White);
		(get_UI_Objects())[0]->setObjectState(ObjectState::standard);

		getUI()->getScreen()->getScrollbar()->getSelectionBox()->changeColor(D2D1::ColorF(255.0 / 255.0, 102.0 / 255.0, 0));
		getUI()->getScreen()->getPlayVisual()->get_UI_Objects()[0]->changeBorderColor(D2D1::ColorF::LightGray);

		getUI()->getScreen()->getScrollbar()->getActionIndicator()->setHorFillPercen(0.0F);
		getUI()->getScreen()->StopPlayGesture();

		imagePos = getOriginalPos();
		imagePos.x += Xoffset;

		for (int j = 0; j < UI_Objects.size(); j++)
		{
			UI_Objects[j]->setCenter(imagePos);
		}

		moveOn = false;

		//getUI()->getScreen()->StopPlayGesture();

		//printf("OFF\n");
		break;
	case ActionTrigger::HoverHold:
	
		//move the hitbox and the image
		if( coord.x > getOriginalPos().x-10 || moveOn) //a way to only start moving when past the halve of the hitbox
		{
			moveOn = true;


			MoveHitbox(coord);

			percentage = checkActivationCriteria();

			if (percentage > 1.0f)
				percentage = 1.0F;
			else if (percentage < 0.0f)
				percentage = 0.0f;
			//printf("percent = %f \n", percentage);

			

			//UI_Objects[0]->changeColor(D2D1::ColorF(percentage, 0, 0));
			//UI_Objects[1]->changeColor(D2D1::ColorF(1.0, 1.0- percentage, 1.0 - percentage));

			//UI_Objects[0]->setAlpha(1.0f - percentage);
			//UI_Objects[1]->setAlpha(1.0f - percentage);
		
			getUI()->getScreen()->getScrollbar()->getActionIndicator()->setHorFillPercen(percentage);
		//	UI_Objects[3]->setText(std::to_wstring(percentage));
			
			if (percentage < 0.1)
			{
				UI_Objects[1]->changeColor(D2D1::ColorF(1, 1, 1));
				UI_Objects[0]->changeColor(D2D1::ColorF::Black);
				if (getHover())
					action(ActionTrigger::HoverOn, coord);
			}
				
			
			imagePos = getCenterCoordActionArea();
			imagePos.x += Xoffset;

			for (int j = 0; j < UI_Objects.size(); j++)
			{
				UI_Objects[j]->setCenter(imagePos);

			}
		}
	
	
		
		setLastPoint(coord);

		//printf("HOLD ");
		break;
	case ActionTrigger::ActiveHandOn:	
		(get_UI_Objects())[0]->setObjectState(ObjectState::handActive);
		/*
		if (getActiveHandBeforeHover())
			action(ActionTrigger::HoverOn, coord);
		else	*/
			action(ActionTrigger::HoverHold, coord);

		break;
	case ActionTrigger::ActiveHandOff:

		//if (getHover())
			action(ActionTrigger::HoverHold, coord);
		//else
		//	action(ActionTrigger::HoverOn, coord);

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

void UI_HitboxHoverSlideButton::moveLeftAction(D2D1_POINT_2F ref, float move)
{
	D2D1_POINT_2F newPos = ref;
	newPos.x = ref.x - move;
	setCenterCoordActionArea(newPos);
	setWidth(abs(newPos.x-getOriginalPos().x)+ orginalWidth);

}

void UI_HitboxHoverSlideButton::moveRightAction(D2D1_POINT_2F ref, float move)
{
	D2D1_POINT_2F newPos = ref;
	newPos.x = ref.x + move;
	setCenterCoordActionArea(newPos);
	//setWidth(getWidth() + move * 2);
	setWidth(abs(newPos.x - getOriginalPos().x) + orginalWidth);
}

void UI_HitboxHoverSlideButton::moveDownAction(D2D1_POINT_2F ref, float move)
{
	D2D1_POINT_2F newPos = ref;
	newPos.y = ref.y + move;
	setCenterCoordActionArea(newPos);
}

void UI_HitboxHoverSlideButton::moveUpAction(D2D1_POINT_2F ref, float move)
{
	D2D1_POINT_2F newPos = ref;
	newPos.y = ref.y - move;
	setCenterCoordActionArea(newPos);
}

void UI_HitboxHoverSlideButton::activateFunction()
{
	
	if (!getFading())
	{
		setFunctionActivation(true);
		setTotalLock(true);
		setFading(true);

		getUI()->getScreen()->getScrollbar()->setTotalLock(true);
		get_UI_Objects()[0]->changeColor(D2D1::ColorF::Red);
		get_UI_Objects()[1]->changeColor(D2D1::ColorF::LightPink);

	}
	else
	{ 
	

		getUI()->getScreen()->getScrollbar()->setTotalLock(false);
		activateFunctionCallback(get_ID_ModelObject(), 0, getModel(), getUI());
	}
}

void UI_HitboxHoverSlideButton::setXoffset(float offset)
{
	Xoffset = offset;
}

float UI_HitboxHoverSlideButton::getXoffset()
{
	return Xoffset;
}

void UI_HitboxHoverSlideButton::setMoveOn(bool move)
{
	moveOn = move;
}

bool UI_HitboxHoverSlideButton::getMoveOn()
{
	return moveOn;
}
