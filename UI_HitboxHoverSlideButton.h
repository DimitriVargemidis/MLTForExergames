#pragma once
//author: Christiaan Vanbergen 
#include "Abstr_UI_HitboxSlideButton.h"

class UI;

//A child class of Abstr_UI_HitboxSlideButton add behavior to use joints in any handstate to slide a hitbox to an activation point and then fade before activating the callback function

//author: Christiaan Vanbergen 
class UI_HitboxHoverSlideButton :
	public Abstr_UI_HitboxSlideButton
{
public:
	UI_HitboxHoverSlideButton();
	UI_HitboxHoverSlideButton(	float Xcenter, float Ycenter, float width, float height, float left, float right, float up, float down, float activation= 0.5f,
								std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback = UI_CallbackFunctions::testCallback, int ID_Model = -1, float XImageOffset = 0);
	~UI_HitboxHoverSlideButton();

	//redefine parent functions
	void action(ActionTrigger act, const D2D1_POINT_2F & coord) override;

	virtual void	moveLeftAction(D2D1_POINT_2F ref, float move) override;
	virtual void	moveRightAction(D2D1_POINT_2F ref, float move) override;
	virtual void	moveDownAction(D2D1_POINT_2F ref, float move) override;
	virtual void	moveUpAction(D2D1_POINT_2F ref, float move) override;

	virtual void	activateFunction() override;

	virtual void	setXoffset(float offset) override;
	virtual float	getXoffset() override;
	
	void setMoveOn(bool move);
	bool getMoveOn();
private:

	float orginalWidth;		//the original width of the hitbox	
	float Xoffset;			//offset in the X direction with which the UI_Objects are drawn

	bool moveOn = false;	//flag to start sliding, activated when a valid joint moves beyond the half of the width of the hitbox

};

