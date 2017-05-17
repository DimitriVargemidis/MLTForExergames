#include "UI.h"

#include "Abstr_UI_HitboxSlideButton.h"



Abstr_UI_HitboxSlideButton::Abstr_UI_HitboxSlideButton():
	Abstr_UI_Hitbox()
{
	setOriginalPos(this->getCenterCoordActionArea());

	endLeft = 100;
	endRight = 100;
	endDown = 100;
	endUp = 100;
}



Abstr_UI_HitboxSlideButton::Abstr_UI_HitboxSlideButton(float Xcenter, float Ycenter, float width, float height, float left, float right, float up, float down, float activation, std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback, int ID_Model):
	Abstr_UI_Hitbox {Xcenter, Ycenter, width, height,callback,ID_Model}, dragging{ false }, lastPoint{ NULL },
	ActivationPoint{ activation }, endLeft{ left }, endRight{ right }, endDown{ down }, endUp{ up }
{
		setOriginalPos(this->getCenterCoordActionArea());
}


Abstr_UI_HitboxSlideButton::~Abstr_UI_HitboxSlideButton()
{
}

void Abstr_UI_HitboxSlideButton::HoverOnAction()
{
}

void Abstr_UI_HitboxSlideButton::HoverOffAction()
{
}

void Abstr_UI_HitboxSlideButton::HoverHoldAction()
{
}

void Abstr_UI_HitboxSlideButton::ActiveHandOnAction()
{
}

void Abstr_UI_HitboxSlideButton::ActiveHandOffAction()
{
}

void Abstr_UI_HitboxSlideButton::ActiveHandHoldAction()
{
}

void Abstr_UI_HitboxSlideButton::MoveHitbox(const D2D1_POINT_2F & coord)
{
	D2D1_POINT_2F newPos = getCenterCoordActionArea();

	//printf("hello \n");
	float moveLeft = (lastPoint.x - coord.x);
	float moveRight = (coord.x - lastPoint.x);
	float moveDown = (coord.y - lastPoint.y);
	float moveUp = (lastPoint.y - coord.y);
	//printf("MoveLeft = %f MoveRight = %f moveDown = %f moveUp = %f \n", moveLeft, moveRight, moveDown, moveUp);

	//printf("x is %f and y is %f \n", newPos.x, newPos.y);

	float moveLeftFromOrigin = (getOriginalPos().x - (newPos.x - moveLeft));
	float moveRightFromOrigin = ((newPos.x + moveRight) - (getOriginalPos()).x);
	float moveDownFromOrigin = ((newPos.y + moveDown) - (getOriginalPos()).y);
	float moveUpFromOrigin = ((getOriginalPos()).y - (newPos.y - moveUp));

	//if (moveLeft > 0  && endLeft != 0)
	if (moveLeft > 0)
	{
		//if (((getOriginalPos()).x - (newPos.x - moveLeft)) < endLeft)
		if ((moveLeftFromOrigin) < endLeft)
			//newPos.x = newPos.x - moveLeft;	
			moveLeftAction(newPos, moveLeft);
		else if (endLeft != 0)
		{
			//newPos.x = originalPos.x - endLeft;
			moveLeftAction((getOriginalPos()), endLeft);
		}

		//	printf("move left to x = %f  \n", newPos.x);
	}
	//if (moveRight > 0 && endRight != 0)
	if (moveRight > 0 )
	{
		//if (((newPos.x + moveRight) - (getOriginalPos()).x) < endRight)
		if ((moveRightFromOrigin) < endRight)
			//newPos.x = newPos.x + moveRight;
			moveRightAction(newPos, moveRight);
		else if (endRight != 0)
		{
			//newPos.x = originalPos.x + endRight;
			moveRightAction((getOriginalPos()), endRight);
		}
		//	printf("move right to x = %f  \n", newPos.x);
	}
	//if (moveDown > 0 && endDown!= 0
	if (moveDown > 0)
	{
		if ((moveDownFromOrigin) < endDown)
			//newPos.y = newPos.y + moveDown;
			moveDownAction(newPos, moveDown);
		else if (endDown != 0)
		{
			//newPos.y = originalPos.y + endDown;
			moveDownAction((getOriginalPos()), endDown);
		}
		//	printf("move down to y = %f  \n", newPos.y);
	}
	//if (moveUp > 0 && endUp != 0)
	if (moveUp > 0)
	{
		if ((moveUpFromOrigin) < endUp)
			//newPos.y = newPos.y - moveUp;
			moveUpAction(newPos, moveUp);
		else if (endUp != 0)
		{
			moveUpAction((getOriginalPos()), endUp);
			//newPos.y = originalPos.y - endUp;
		}
		//	printf("move up to y = %f  \n", newPos.y);
	}


	//printf("changed x is %f and y is %f \n", newPos.x, newPos.y);


	//setCenterCoordActionArea(newPos);
	//	lastPoint = newPos;
}

float Abstr_UI_HitboxSlideButton::checkActivationCriteria()
{
	std::vector<std::shared_ptr<UI_Object>> & UI_Objects = get_UI_Objects();
	float Xmoved;				//how much has been moved in the X direction relative to the original position
	float Ymoved;				//how much has been moved in the Y direction relative to the original position
	float NbrActivationPoint;	//how much distance to activation
	float percentage;			//the percentage of distance to activation that has been travelled

	if (!getFunctionActivation())
	{
		D2D1_POINT_2F center = getCenterCoordActionArea();

		if (endLeft > 0)
		{
			//UI_Objects[0]->changeColor(D2D1::ColorF(0.5 + , 0, 0));
			Xmoved = (getOriginalPos()).x - center.x;
			NbrActivationPoint = endLeft*ActivationPoint;

			percentage = Xmoved / NbrActivationPoint;

			if (NbrActivationPoint < Xmoved)
			{
				//activateFunction();
				setFunctionActivation(true);
				activateFunctionCallback(get_ID_ModelObject(),0, getModel(), getUI());
				
			}
			//	printf("move left to x = %f  \n", newPos.x);
		}
		if (endRight > 0)
		{
			Xmoved = center.x - (getOriginalPos()).x;
			NbrActivationPoint = endRight*ActivationPoint;
			if (percentage > Xmoved / NbrActivationPoint)
				percentage = Xmoved / NbrActivationPoint;

			if (NbrActivationPoint < Xmoved)
			{
				//activateFunction();
				setFunctionActivation(true);
				activateFunctionCallback(get_ID_ModelObject(), 0, getModel(), getUI());
				
			}

			//	printf("move right to x = %f  \n", newPos.x);
		}
		if (endDown > 0)
		{
			Ymoved = center.y - (getOriginalPos()).y;
			NbrActivationPoint = endDown*ActivationPoint;

			if (percentage > Ymoved / NbrActivationPoint)
				percentage = Ymoved / NbrActivationPoint;


			if (NbrActivationPoint < Ymoved)
			{
				//activateFunction();
				setFunctionActivation(true);
				activateFunctionCallback(get_ID_ModelObject(), 0, getModel(), getUI());
			
			}
			//	printf("move down to y = %f  \n", newPos.y);
		}
		if (endUp > 0)
		{
			Ymoved = getOriginalPos().y - center.y;
			NbrActivationPoint = endUp*ActivationPoint;

			if (percentage > Ymoved / NbrActivationPoint)
				percentage = Ymoved / NbrActivationPoint;

			if (NbrActivationPoint < Ymoved)
			{
				//activateFunction();
				setFunctionActivation(true);
				activateFunctionCallback(get_ID_ModelObject(), 0, getModel(), getUI());

			}
			//	printf("move up to y = %f  \n", newPos.y);
		}

		
	}
	return percentage;

}




void Abstr_UI_HitboxSlideButton::setLastPoint(D2D1_POINT_2F pos)
{
	lastPoint = pos;
}

D2D1_POINT_2F Abstr_UI_HitboxSlideButton::getLastPoint()
{
	return lastPoint;
}

