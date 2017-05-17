#pragma once
#include "Abstr_UI_Hitbox.h"

class UI;

class Abstr_UI_HitboxSlideButton: 
	public Abstr_UI_Hitbox
{
public:
	Abstr_UI_HitboxSlideButton();
	Abstr_UI_HitboxSlideButton(float Xcenter, float Ycenter, float width, float height,
		float left = 100.0, float right = 0.0, float up = 0.0, float down = 0.0, float activation = 0.5,
		std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback = UI_CallbackFunctions::testCallback, int ID_Model = -1);
	~Abstr_UI_HitboxSlideButton();

	//action executed when a condition is met, to be overrided by the child
	virtual void HoverOnAction() override;
	virtual void HoverOffAction() override;
	virtual void HoverHoldAction() override;

	virtual void ActiveHandOnAction() override;
	virtual void ActiveHandOffAction() override;
	virtual void ActiveHandHoldAction() override;

	virtual void moveLeftAction(D2D1_POINT_2F ref, float move) =0;
	virtual void moveRightAction(D2D1_POINT_2F ref, float move) = 0;
	virtual void moveDownAction(D2D1_POINT_2F ref, float move) = 0;
	virtual void moveUpAction(D2D1_POINT_2F ref, float move) = 0;

	void action(ActionTrigger act, const D2D1_POINT_2F & coord) override =0;

	void MoveHitbox(const D2D1_POINT_2F & coord);

	virtual float checkActivationCriteria();
	virtual void activateFunction() override = 0;

	void setLastPoint(D2D1_POINT_2F pos) override;
	D2D1_POINT_2F getLastPoint() override;

private:

	D2D1_POINT_2F lastPoint;	//the coordinate of the last time the joint was activating the dragfuntion
	bool dragging;				//true when the user is dragging

								//the relative movemnnt to the end point
	float endLeft;
	float endRight;
	float endUp;
	float endDown;

	float ActivationPoint; //how much distance needs to be traveled to activate the function

};



