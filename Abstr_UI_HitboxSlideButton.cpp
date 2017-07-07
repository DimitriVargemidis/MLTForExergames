#include "UI.h"
//author: Christiaan Vanbergen 

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


void Abstr_UI_HitboxSlideButton::MoveHitbox(const D2D1_POINT_2F & coord)
{
	//the initiates the coordinates of the new position that the hitbox is going to take
	D2D1_POINT_2F newPos = getCenterCoordActionArea();

	//calculates the relative distance between the new coordinates and the coordinates of the previous frame
	float moveLeft = (lastPoint.x - coord.x);
	float moveRight = (coord.x - lastPoint.x);
	float moveDown = (coord.y - lastPoint.y);
	float moveUp = (lastPoint.y - coord.y);
	
	//calculates the relative distance between the origin of the hitbox and the next position of the center of the hitbox if the action is completed
	float moveLeftFromOrigin = (getOriginalPos().x - (newPos.x - moveLeft));
	float moveRightFromOrigin = ((newPos.x + moveRight) - (getOriginalPos()).x);
	float moveDownFromOrigin = ((newPos.y + moveDown) - (getOriginalPos()).y);
	float moveUpFromOrigin = ((getOriginalPos()).y - (newPos.y - moveUp));

	//for every direction there are check whether the move is positive and not further than the limit
	if (moveLeft > 0)
	{
		if ((moveLeftFromOrigin) < endLeft)// not further than the limit?
		{
			moveLeftAction(newPos, moveLeft); //move the center of the hitbox
		}
		else if (endLeft != 0) //further than the limit and there is a limit higher than 0?
		{
			moveLeftAction((getOriginalPos()), endLeft); // mover to the end position of the action
		}
	}

	if (moveRight > 0 )
	{
		if ((moveRightFromOrigin) < endRight)
			moveRightAction(newPos, moveRight);
		else if (endRight != 0)
		{
			moveRightAction((getOriginalPos()), endRight);
		}
	}

	if (moveDown > 0)
	{
		if ((moveDownFromOrigin) < endDown)
			moveDownAction(newPos, moveDown);
		else if (endDown != 0)
		{
			moveDownAction((getOriginalPos()), endDown);
		}
	}

	if (moveUp > 0)
	{
		if ((moveUpFromOrigin) < endUp)
			moveUpAction(newPos, moveUp);
		else if (endUp != 0)
		{
			moveUpAction((getOriginalPos()), endUp);
		}
	}
}

float Abstr_UI_HitboxSlideButton::checkActivationCriteria()
{
	std::vector<std::shared_ptr<UI_Object>> & UI_Objects = get_UI_Objects();
	float Xmoved;						//how much has been moved in the X direction relative to the original position
	float Ymoved;						//how much has been moved in the Y direction relative to the original position
	float NbrActivationPoint;			//how much distance to activation
	volatile float percentage = 0.0f;	//the percentage of distance to activation that has been travelled (volatile becuase the compiler kept optimizing it out while it is needed)
	float percent = 0.0f;

	if (!getFunctionActivation()) //is the function already activated?
	{
		D2D1_POINT_2F center = getCenterCoordActionArea();

		//checks for every direction if the percentage of the end distance that has been moved exceeds the preset value need to activate
		if (endLeft > 0)
		{
			Xmoved = (getOriginalPos()).x - center.x;
			NbrActivationPoint = endLeft*ActivationPoint; //calculates the actual distance that need to be traveled

			percentage = Xmoved / NbrActivationPoint;

			if (NbrActivationPoint < Xmoved)
			{
				activateFunction();				
			}
		}
		if (endRight > 0)
		{
			Xmoved = center.x - (getOriginalPos()).x;
			NbrActivationPoint = endRight*ActivationPoint;

			percent = Xmoved / NbrActivationPoint;

			if (percentage < percent)
				percentage = percent;

			if (NbrActivationPoint < Xmoved)
			{
				activateFunction();
			}
		}
		if (endDown > 0)
		{
			Ymoved = center.y - (getOriginalPos()).y;
			NbrActivationPoint = endDown*ActivationPoint;

			percent = Ymoved / NbrActivationPoint;

			if (percentage < percent)
				percentage = percent;


			if (NbrActivationPoint < Ymoved)
			{
				activateFunction();
			}
		}
		if (endUp > 0)
		{
			Ymoved = getOriginalPos().y - center.y;
			NbrActivationPoint = endUp*ActivationPoint;

			percent = Ymoved / NbrActivationPoint;

			if (percentage < percent)
				percentage = percent;

			if (NbrActivationPoint < Ymoved)
			{
				activateFunction();
			}
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

