
#include <string>

#include "UI.h"

#include "UI_HitboxHoverSlideButton.h"



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
	float percentage;		//the percentage untill activation

	switch (act)
	{
	case ActionTrigger::HoverOn:
		setLastPoint(coord);
		(get_UI_Objects())[0]->changeColor(D2D1::ColorF::Green); //TO DO make callback
	//	getUI()->getScreen()->autoplayGesture(get_ID_ModelObject());

		//printf("ON \n");
		break;
	case ActionTrigger::HoverOff:
		
		setCenterCoordActionArea(getOriginalPos());
		setWidth(orginalWidth);

		deactivateFunction();

		UI_Objects[0]->changeColor(D2D1::ColorF::Black);
		UI_Objects[1]->changeColor(D2D1::ColorF::White);

		imagePos = getOriginalPos();
		imagePos.x += Xoffset;

		for (int j = 0; j < UI_Objects.size(); j++)
		{
			UI_Objects[j]->setCenter(imagePos);
		}

		//getUI()->getScreen()->StopPlayGesture();

		//printf("OFF\n");
		break;
	case ActionTrigger::HoverHold:
	
		//move the hitbox and the image
		if( coord.x > getOriginalPos().x ) //a way to only start moving when past the halve of the hitbox
		{
			MoveHitbox(coord);

			percentage = checkActivationCriteria();

			UI_Objects[0]->changeColor(D2D1::ColorF(percentage, 0, 0));
			UI_Objects[1]->changeColor(D2D1::ColorF(1.0, 1.0- percentage, 1.0 - percentage));

		//	UI_Objects[3]->setText(std::to_wstring(percentage));
			/*
			if(percentage < 0.1)
				UI_Objects[1]->changeColor(D2D1::ColorF(1, 1, 1));
			*/
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
		getUI()->getScreen()->autoplayGesture(get_ID_ModelObject());
		break;
	case ActionTrigger::ActiveHandOff:
		getUI()->getScreen()->StopPlayGesture();

		break;

	case ActionTrigger::ActiveHandHold:
		break;
	case ActionTrigger::ActiveHandOutsideOn:

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
	setFunctionActivation(true);
	activateFunctionCallback(get_ID_ModelObject(), 0, getModel(), getUI());
}

void UI_HitboxHoverSlideButton::setXoffset(float offset)
{
	Xoffset = offset;
}

float UI_HitboxHoverSlideButton::getXoffset()
{
	return Xoffset;
}
