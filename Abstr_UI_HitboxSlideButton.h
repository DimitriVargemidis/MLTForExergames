#pragma once
//author: Christiaan Vanbergen 
#include "Abstr_UI_Hitbox.h"

class UI;

//Abstract class that is a child of Abstr_UI_Hitbox adding functionality to calculate the movement of a joint over the hitbox in order to mover hitboxes and UI_Object according to it.

//author: Christiaan Vanbergen 
class Abstr_UI_HitboxSlideButton: 
	public Abstr_UI_Hitbox
{
public:
	Abstr_UI_HitboxSlideButton();
	Abstr_UI_HitboxSlideButton(float Xcenter, float Ycenter, float width, float height,
		float left = 100.0, float right = 0.0, float up = 0.0, float down = 0.0, float activation = 0.5,
		std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback = UI_CallbackFunctions::testCallback, int ID_Model = -1);
	~Abstr_UI_HitboxSlideButton();

	//functions to move to move hitboxes and UI_Objects different directions
	//"ref" is a point on the screen  
	//"move" is the movement relative it that the hitbox needs to make
	//primarily used by the MoveHitbox function
	virtual void moveLeftAction(D2D1_POINT_2F ref, float move) =0;
	virtual void moveRightAction(D2D1_POINT_2F ref, float move) = 0;
	virtual void moveDownAction(D2D1_POINT_2F ref, float move) = 0;
	virtual void moveUpAction(D2D1_POINT_2F ref, float move) = 0;

	//action function overriden from Abstr_UI_Hitbox
	void action(ActionTrigger act, const D2D1_POINT_2F & coord) override =0;

	//mayor function that checks criteria for moving the hitboxes and activates the moving of the hitbox
	//primarily used by the action function
	void MoveHitbox(const D2D1_POINT_2F & coord);

	//checks if the criteria for activating the hitbox are reached
	virtual float checkActivationCriteria();
	//activates the hitbox and contains additional action to be taken at activation
	virtual void activateFunction() override = 0;

	void setLastPoint(D2D1_POINT_2F pos) override;
	D2D1_POINT_2F getLastPoint() override;

private:

	D2D1_POINT_2F lastPoint;	//the coordinate of the last time the joint was activating the dragfuntion
	bool dragging;				//true when the user is dragging

	//The relative distance that the hitbox center can move from its original position
	float endLeft;
	float endRight;
	float endUp;
	float endDown;

	//how much distance towards an endpoint needs to be traveled to activate the function
	float ActivationPoint; // 0-1

};



