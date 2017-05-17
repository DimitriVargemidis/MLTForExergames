#include "Model.h"
#include "UI.h"

#include "UI_HitboxSlideButton.h"

UI_HitboxSlideButton::UI_HitboxSlideButton():
	Abstr_UI_HitboxSlideButton()
{
}

UI_HitboxSlideButton::UI_HitboxSlideButton(float Xcenter, float Ycenter, float width, float height, float left, float right, float up, float down, float activation, std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback, int ID_Model):
	Abstr_UI_HitboxSlideButton{ Xcenter,Ycenter,width,height,left,right,up,down,activation,callback,ID_Model}
{
}


UI_HitboxSlideButton::~UI_HitboxSlideButton()
{
}

/*

void UI_HitboxSlideButton::HoverOnAction()
{
}

void UI_HitboxSlideButton::HoverOffAction()
{
}

void UI_HitboxSlideButton::HoverHoldAction()
{
}

void UI_HitboxSlideButton::ActiveHandOnAction()
{
}

void UI_HitboxSlideButton::ActiveHandOffAction()
{
}

void UI_HitboxSlideButton::ActiveHandHoldAction()
{
}
*/
void UI_HitboxSlideButton::action(ActionTrigger act, const D2D1_POINT_2F & coord)
{
	D2D_POINT_2F point;

	switch (act)
	{
	case ActionTrigger::HoverOn:
		(get_UI_Objects())[0]->changeColor(D2D1::ColorF::Gray); //TO DO make callback
		break;
	case ActionTrigger::HoverOff:
		if (!activeOutside)
		{
			(get_UI_Objects())[0]->changeColor(D2D1::ColorF::White);
		}

		break;
	case ActionTrigger::HoverHold:

		break;
	case ActionTrigger::ActiveHandOn:
		setLastPoint(coord);
		
		(get_UI_Objects())[0]->changeColor(D2D1::ColorF::Green); //TO DO make callback
		break;
	case ActionTrigger::ActiveHandOff:
		setCenterCoordActionArea(getOriginalPos());

		deactivateFunction();

		point = getCenterCoordActionArea();
		point.y -= 100;
		
		get_UI_Objects()[0]->setCenter(point);
		get_UI_Objects()[0]->changeColor(D2D1::ColorF::Gray); //TO DO make callback

		break;

	case ActionTrigger::ActiveHandHold:
		MoveHitbox(coord);
		setLastPoint(coord);

		checkActivationCriteria();

		point = getCenterCoordActionArea();
		point.y -= 100;

		get_UI_Objects()[0]->setCenter(point);

		break;
	case ActionTrigger::ActiveHandOutsideOn:
		action(ActionTrigger::ActiveHandHold, coord);
		activeOutside = true;
		break;
	case ActionTrigger::ActiveHandOutsideOff:
		action(ActionTrigger::ActiveHandOff, coord);
		activeOutside = false;
		action(ActionTrigger::HoverOff, coord);
		break;
	case ActionTrigger::ActiveHandOutsideHold:
		action(ActionTrigger::ActiveHandHold, coord);
		break;
	}
}
/*
void UI_HitboxSlideButton::MoveHitbox(const D2D1_POINT_2F & coord)
{
	D2D1_POINT_2F newPos = getCenterCoordActionArea();
	
	//printf("hello \n");
	float moveLeft =	(lastPoint.x - coord.x);
	float moveRight =	(coord.x - lastPoint.x);
	float moveDown =	(coord.y - lastPoint.y);
	float moveUp =		(lastPoint.y - coord.y);
	//printf("MoveLeft = %f MoveRight = %f moveDown = %f moveUp = %f \n", moveLeft, moveRight, moveDown, moveUp);

	if (moveRight > 100)
	{
		printf("why though");
	}
	
	//printf("x is %f and y is %f \n", newPos.x, newPos.y);

	if (moveLeft > 0)
	{
		if ((originalPos.x - (newPos.x - moveLeft)) < endLeft)
			newPos.x = newPos.x - moveLeft;
		else
		{
			newPos.x = originalPos.x - endLeft;
		}

		//	printf("move left to x = %f  \n", newPos.x);
	}
	if (moveRight > 0)
	{
		if(((newPos.x + moveRight) - originalPos.x)  < endRight)
			newPos.x = newPos.x + moveRight;
		else
		{
			newPos.x = originalPos.x + endRight;
		}
		//	printf("move right to x = %f  \n", newPos.x);
	}
	if (moveDown > 0)
	{
		if(((newPos.y + moveDown) - originalPos.y) < endDown)
			newPos.y = newPos.y + moveDown;
		else
		{
			newPos.y = originalPos.y + endDown;
		}
		//	printf("move down to y = %f  \n", newPos.y);
	}
	if (moveUp > 0)
	{
		if((originalPos.y - (newPos.y - moveUp)) < endUp)
			newPos.y = newPos.y - moveUp;
		else
		{
			newPos.y = originalPos.y - endUp;
		}
		//	printf("move up to y = %f  \n", newPos.y);
	}

	
	//printf("changed x is %f and y is %f \n", newPos.x, newPos.y);


	setCenterCoordActionArea(newPos);
//	lastPoint = newPos;
}

void UI_HitboxSlideButton::checkActivationCriteria()
{
	if (!getFunctionActivation())
	{
		D2D1_POINT_2F center = getCenterCoordActionArea();

		if (endLeft > 0)
		{
			if ((endLeft*ActivationPoint) < (originalPos.x - center.x))
				activateFunction();
			//	printf("move left to x = %f  \n", newPos.x);
		}
		if (endRight > 0)
		{
			if ((endRight*ActivationPoint) < (center.x - originalPos.x))
				activateFunction();

			//	printf("move right to x = %f  \n", newPos.x);
		}
		if (endDown > 0)
		{
			if ((endDown*ActivationPoint) < (center.y - originalPos.y))
				activateFunction();

			//	printf("move down to y = %f  \n", newPos.y);
		}
		if (endUp > 0)
		{
			if ((endUp*ActivationPoint) < (originalPos.y - center.y))
				activateFunction();

			//	printf("move up to y = %f  \n", newPos.y);
		}
	}

}
*/
void UI_HitboxSlideButton::activateFunction()
{
	activateFunctionCallback(get_ID_ModelObject(), 0, getModel(), getUI());
	//printf("activated? \n");
	//getModel()->setRefresh(true);
	setFunctionActivation(true);
}
void UI_HitboxSlideButton::moveLeftAction(D2D1_POINT_2F ref, float move)
{
	D2D1_POINT_2F newPos = ref;
	newPos.x = ref.x - move;
	setCenterCoordActionArea(newPos);
}
void UI_HitboxSlideButton::moveRightAction(D2D1_POINT_2F ref, float move)
{
	D2D1_POINT_2F newPos = ref;
	newPos.x = ref.x + move;
	setCenterCoordActionArea(newPos);
}
void UI_HitboxSlideButton::moveDownAction(D2D1_POINT_2F ref, float move)
{
	D2D1_POINT_2F newPos = ref;
	newPos.y = ref.y + move;
	setCenterCoordActionArea(newPos);
}
void UI_HitboxSlideButton::moveUpAction(D2D1_POINT_2F ref, float move)
{
	D2D1_POINT_2F newPos = ref;
	newPos.y = ref.y - move;
	setCenterCoordActionArea(newPos);
}
/*
void UI_HitboxSlideButton::setOriginalPos(D2D1_POINT_2F pos)
{
	originalPos = pos;
}

D2D1_POINT_2F UI_HitboxSlideButton::getOriginalPos()
{
	return originalPos;
}
*/