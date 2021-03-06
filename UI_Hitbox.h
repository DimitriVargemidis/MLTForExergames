#pragma once
//author: Christiaan Vanbergen 


#include "Abstr_UI_Hitbox.h"

class UI;

//A child class of Abstr_UI_Hitbox that defines simple behavior of activating when the hand goes into handActiveState (default: hand closes) over the hitbox
//and deactivates when the hand goes out of 

//author: Christiaan Vanbergen 
class UI_Hitbox : public Abstr_UI_Hitbox
{
public:
	UI_Hitbox();
	UI_Hitbox(	float Xcenter, float Ycenter, float width, float height,
				std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback = UI_CallbackFunctions::testCallback, int ID_Model = -1);
	~UI_Hitbox();
	
	virtual void action(ActionTrigger act, const D2D1_POINT_2F & coord ) override;
	
	virtual void activateFunction() override;

	virtual void setLastPoint(D2D1_POINT_2F pos);
	virtual D2D1_POINT_2F getLastPoint();

};
