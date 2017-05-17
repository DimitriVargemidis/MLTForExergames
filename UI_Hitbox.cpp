
#include "Model.h"
#include "UI.h"

#include "UI_Hitbox.h"

UI_Hitbox::UI_Hitbox(): 
	Abstr_UI_Hitbox()
{
	
}


UI_Hitbox::UI_Hitbox(float Xcenter, float Ycenter, float width, float height, std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback, int ID_Model):
	Abstr_UI_Hitbox(Xcenter, Ycenter, width, height,callback,ID_Model)
{
}

UI_Hitbox::~UI_Hitbox()
{
}

void UI_Hitbox::action(ActionTrigger action, const D2D1_POINT_2F & coord)
{
	switch (action)
	{
	case ActionTrigger::HoverOn:
		get_UI_Objects()[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
		break;
	case ActionTrigger::HoverOff:
		get_UI_Objects()[0]->changeColor(D2D1::ColorF::White);
		break;
	case ActionTrigger::HoverHold:
		break;
	case ActionTrigger::ActiveHandOn:

		activateFunction();

		get_UI_Objects()[0]->changeColor(D2D1::ColorF::Blue); //TO DO make callback
		break;
	case ActionTrigger::ActiveHandOff:
	
		deactivateFunction();

		get_UI_Objects()[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
		break;
	case ActionTrigger::ActiveHandHold:
		break;
	case ActionTrigger::ActiveHandOutsideOn:
		get_UI_Objects()[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
		break;
	case ActionTrigger::ActiveHandOutsideOff:
		//UI_objects[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
		break;
	case ActionTrigger::ActiveHandOutsideHold:
		//UI_objects[0]->changeColor(D2D1::ColorF::Red); //TO DO make callback
		break;
	}


}

void UI_Hitbox::activateFunction()
{
	if (!getFunctionActivation())
	{
			setFunctionActivation(true);
			activateFunctionCallback(get_ID_ModelObject(),0, getModel(),getUI());
	}
}
void UI_Hitbox::setLastPoint(D2D1_POINT_2F pos)
{

}
D2D1_POINT_2F UI_Hitbox::getLastPoint()
{
	return D2D1_POINT_2F();
}

