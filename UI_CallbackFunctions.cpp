#include "UI_CallbackFunctions.h"

void UI_CallbackFunctions::dragButton(const ActionTrigger act, std::vector<std::shared_ptr<UI_Object>> UI_Objects, UI_Hitbox* hitbox)
{
	UI_HitboxSlideButton* hitboxSlide = static_cast<UI_HitboxSlideButton*>(hitbox);

	switch (act)
	{
	case ActionTrigger::HoverOn:
		UI_Objects[0]->changeColor(D2D1::ColorF::Gray); //TO DO make callback
		break;
	case ActionTrigger::HoverOff:
		UI_Objects[0]->changeColor(D2D1::ColorF::White);
		break;
	case ActionTrigger::HoverHold:
		break;
	case ActionTrigger::ActiveHandOn:
		UI_Objects[0]->changeColor(D2D1::ColorF::Green); //TO DO make callback
		break;
	case ActionTrigger::ActiveHandOff:
		UI_Objects[0]->setCenter(hitboxSlide->getOriginalPos());
		UI_Objects[0]->changeColor(D2D1::ColorF::Gray); //TO DO make callback

		break;

	case ActionTrigger::ActiveHandHold:
		UI_Objects[0]->setCenter(hitbox->getCenterCoordActionArea());
		break;
	case ActionTrigger::ActiveHandOutsideOn:
		break;
	case ActionTrigger::ActiveHandOutsideOff:
		break;
	case ActionTrigger::ActiveHandOutsideHold:
		break;
	}

}
