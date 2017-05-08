#include "UI_HitboxHoverSlideButton.h"



UI_HitboxHoverSlideButton::UI_HitboxHoverSlideButton()
{
}

UI_HitboxHoverSlideButton::UI_HitboxHoverSlideButton(float Xcenter, float Ycenter, float width, float height, float left, float right, float up, float down, float activation, std::function<void(int)> callback, int ID_Model):
	Abstr_UI_HitboxSlideButton {Xcenter, Ycenter, width, height, left, right, up, down, activation, callback, ID_Model}
{
}



UI_HitboxHoverSlideButton::~UI_HitboxHoverSlideButton()
{



}

void UI_HitboxHoverSlideButton::action(ActionTrigger act, const D2D1_POINT_2F & coord)
{
	switch (act)
	{
	case ActionTrigger::HoverOn:
		setLastPoint(coord);
		(get_UI_Objects())[0]->changeColor(D2D1::ColorF::Gray); //TO DO make callback
		//printf("ON \n");
		break;
	case ActionTrigger::HoverOff:
		setCenterCoordActionArea(getOriginalPos());

		deactivateFunction();

		(get_UI_Objects())[0]->changeColor(D2D1::ColorF::White);

		get_UI_Objects()[0]->setCenter(getOriginalPos());
		//printf("OFF\n");
		break;
	case ActionTrigger::HoverHold:
		MoveHitbox(coord);
		setLastPoint(coord);

		checkActivationCriteria();

		get_UI_Objects()[0]->setCenter(getCenterCoordActionArea());
		//printf("HOLD ");
		break;
	case ActionTrigger::ActiveHandOn:		
		break;
	case ActionTrigger::ActiveHandOff:
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
}

void UI_HitboxHoverSlideButton::moveRightAction(D2D1_POINT_2F ref, float move)
{
	D2D1_POINT_2F newPos = ref;
	newPos.x = ref.x + move;
	setCenterCoordActionArea(newPos);
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
}
