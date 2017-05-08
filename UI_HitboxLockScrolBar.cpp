#include "UI_HitboxLockScrolBar.h"



UI_HitboxLockScrolBar::UI_HitboxLockScrolBar()
{
}

UI_HitboxLockScrolBar::UI_HitboxLockScrolBar(float Xcenter, float Ycenter, float width, float height, float left, float right, float up, float down, float activation, std::function<void(int)> callback, int ID_Model):
	UI_HitboxScrolBar{ Xcenter, Ycenter, width, height, left, right, up, down, activation, callback, ID_Model}, selectedHitbox{ 0 }

{
}


UI_HitboxLockScrolBar::~UI_HitboxLockScrolBar()
{
}

void UI_HitboxLockScrolBar::action(ActionTrigger act, const D2D1_POINT_2F & coord)
{
	D2D1_POINT_2F	scrolcenter = getCenterCoordActionArea();
	float			scrolHeight = getHeight();
	float			UI_Height = getHeight_UI_element();


	switch (act)
	{
	case ActionTrigger::HoverOn:
		setLastPoint(coord);
		//	(get_UI_Objects())[0]->changeColor(D2D1::ColorF::Gray); //TO DO make callback
		break;
	case ActionTrigger::HoverOff:
		//(get_UI_Objects())[0]->changeColor(D2D1::ColorF::White);
		//delayUpCounter = 0;
		//delayDownCounter = 0;
		findSelectedHitbox();
		lock_UI_Elements();
		//printf("hover off triggered \n");
		
		setMoveUpCounter(0);
		setMoveDownCounter(0);
		break;
	case ActionTrigger::HoverHold:
	

		if (coord.y > scrolcenter.y - scrolHeight / 2 + UI_Height && coord.y < scrolcenter.y - scrolHeight / 2 + UI_Height * 2 && getMoveDownCounter() < getMoveLimit() && getMoveUpCounter() < getMoveLimit())
		{
		}
		else
		{
			MoveHitbox(coord);
			setLastPoint(coord);

			if (getMoveDownCounter() < getMoveLimit() && getMoveUpCounter() < getMoveLimit())
			{
				findSelectedHitbox();
				lock_UI_Elements();
			//	printf("hover hold triggered \n");
			}
		}
	

		break;
	case ActionTrigger::ActiveHandOn:
		action(ActionTrigger::HoverHold, coord);
		break;
	case ActionTrigger::ActiveHandOff:
		action(ActionTrigger::HoverHold, coord);
		break;

	case ActionTrigger::ActiveHandHold:
		action(ActionTrigger::HoverHold, coord);
		break;
	case ActionTrigger::ActiveHandOutsideOn:

		break;
	case ActionTrigger::ActiveHandOutsideOff:

		break;
	case ActionTrigger::ActiveHandOutsideHold:
		break;
	}
}

void UI_HitboxLockScrolBar::add_UI_Element(std::shared_ptr<Abstr_UI_Hitbox> hitbox)
{
	UI_HitboxScrolBar::add_UI_Element(hitbox);
	selectedHitbox = (get_UI_Elements().size() - 1);
	lock_UI_Elements();
}

void UI_HitboxLockScrolBar::findSelectedHitbox()
{
	std::vector<std::shared_ptr<Abstr_UI_Hitbox>> & UI_elements = get_UI_Elements();

	D2D1_POINT_2F	scrolcenter = getCenterCoordActionArea();
	float			scrolHeight = getHeight();
	float			UI_Height = getHeight_UI_element();
	float			Ypos_1 = scrolcenter.y - scrolHeight / 2 + UI_Height / 2;
	D2D1_POINT_2F	UI_center;					//parameter that will temporarily keep the center coordinates of each UI_element

	float topBorder = scrolcenter.y - scrolHeight / 2 + UI_Height;
	float underborder = scrolcenter.y - scrolHeight / 2 + UI_Height * 2;

	
	int top = 0;		//how many are above the border
	int bottom = 0;		//how many are under the border
	bool set = false;	//if a hitbox center was within the borders

												//first loop through the elements to see in which position they should lock
	for (int i = UI_elements.size() - 1; i >= 0; i--)
	{

		UI_center = UI_elements[i]->getCenterCoordActionArea();

	

		//if (UI_center.y > scrolcenter.y - scrolHeight / 2 + UI_Height && UI_center.y < scrolcenter.y - scrolHeight / 2 + UI_Height * 2)
		if (UI_center.y > topBorder  && UI_center.y < underborder)
		{
			selectedHitbox = i;
			set = true;
			//printf("hitbox = %d \n", selectedHitbox);
		}
		else if (UI_center.y < topBorder) //logic to make sure that the last and first hitbox are easy to select
		{
			top++;
		}
		else if (UI_center.y > underborder)
		{
			bottom++;
		}

		if (!set)
		{
			if (top > bottom)
				selectedHitbox = 0;
			else
				selectedHitbox = UI_elements.size() - 1;
		}

	}
}

void UI_HitboxLockScrolBar::lock_UI_Elements()
{
	std::vector<std::shared_ptr<Abstr_UI_Hitbox>> & UI_elements = get_UI_Elements();

	D2D1_POINT_2F	scrolcenter = getCenterCoordActionArea();
	float			scrolHeight = getHeight();
	float			UI_Height = getHeight_UI_element();
	float			Ypos_1 = scrolcenter.y - scrolHeight / 2 + UI_Height/2;
	D2D1_POINT_2F	UI_center;					//parameter that will temporarily keep the center coordinates of each UI_element

	for (int i = UI_elements.size()-1; i >= 0; i--)
	{	

		std::vector<std::shared_ptr<UI_Object>> UI_Objects = UI_elements[i]->get_UI_Objects();
		D2D1_POINT_2F lastpoint = getLastPoint();


		if (i > selectedHitbox + 1)
		{
			//UI_elements[i]->moveAbsY(Ypos_1 - UI_Height*( i- (selectedHitbox+1) ));
			UI_elements[i]->moveAbsYoriginalPos(Ypos_1 - UI_Height*(i - (selectedHitbox + 1)));
			UI_elements[i]->setCenterCoordActionArea(getOriginalPos());

			//deactivate any active states of the hitbox
			if (UI_elements[i]->getActiveHandOutside())
			{
				UI_elements[i]->setActiveHandOutside(false);
				UI_elements[i]->action(ActionTrigger::ActiveHandOutsideOff, lastpoint);
			}
			if (UI_elements[i]->getActiveHand())
			{
				UI_elements[i]->setActiveHand(false);
				UI_elements[i]->action(ActionTrigger::ActiveHandOff, lastpoint);
			}		
			if (UI_elements[i]->getHover())
			{
				UI_elements[i]->setHover(false);
				UI_elements[i]->action(ActionTrigger::HoverOff, lastpoint);
			}
				


			UI_elements[i]->setTotalLock(true);
		//	UI_elements[i]->setVisible(false);

			for (int j = 0; j < UI_Objects.size(); j++)
			{
				UI_Objects[j]->moveYabs(Ypos_1 - UI_Height*(i - (selectedHitbox + 1)));
		//		UI_Objects[j]->setVisible(false);
			}
		}
		else if (i == selectedHitbox + 1)
		{
			UI_elements[i]->moveAbsY(Ypos_1);
			UI_elements[i]->moveAbsYoriginalPos(Ypos_1);

			//deactivate any active states of the hitbox
			if (UI_elements[i]->getActiveHandOutside())
			{
				UI_elements[i]->setActiveHandOutside(false);
				UI_elements[i]->action(ActionTrigger::ActiveHandOutsideOff, lastpoint);
			}
			if (UI_elements[i]->getActiveHand())
			{
				UI_elements[i]->setActiveHand(false);
				UI_elements[i]->action(ActionTrigger::ActiveHandOff, lastpoint);
			}
			if (UI_elements[i]->getHover())
			{
				UI_elements[i]->setHover(false);
				UI_elements[i]->action(ActionTrigger::HoverOff, lastpoint);
			}

			UI_elements[i]->setTotalLock(true);
		//	UI_elements[i]->setVisible(true);

			for (int j = 0; j < UI_Objects.size(); j++)
			{
				UI_Objects[j]->moveYabs(Ypos_1);
		//		UI_Objects[j]->setVisible(true);
			}
		}
		else if (i == selectedHitbox)
		{
			UI_elements[i]->moveAbsY(Ypos_1 + UI_Height);
			UI_elements[i]->moveAbsYoriginalPos(Ypos_1 + UI_Height);

			UI_elements[i]->setTotalLock(false);
		//	UI_elements[i]->setVisible(true);

			for (int j = 0; j < UI_Objects.size(); j++)
			{
				UI_Objects[j]->moveYabs(Ypos_1 + UI_Height);
			//	UI_Objects[j]->setVisible(true);
			}
		}
		else if (/*i >= selectedHitbox - 3 &&*/ i < selectedHitbox)
		{
			UI_elements[i]->moveAbsY(Ypos_1 + UI_Height + UI_Height*(selectedHitbox-i));
			UI_elements[i]->moveAbsYoriginalPos(Ypos_1 + UI_Height + UI_Height*(selectedHitbox - i));
			
			//deactivate any active states of the hitbox
			if (UI_elements[i]->getActiveHandOutside())
			{
				UI_elements[i]->setActiveHandOutside(false);
				UI_elements[i]->action(ActionTrigger::ActiveHandOutsideOff, lastpoint);
			}
			if (UI_elements[i]->getActiveHand())
			{
				UI_elements[i]->setActiveHand(false);
				UI_elements[i]->action(ActionTrigger::ActiveHandOff, lastpoint);
			}
			if (UI_elements[i]->getHover())
			{
				UI_elements[i]->setHover(false);
				UI_elements[i]->action(ActionTrigger::HoverOff, lastpoint);
			}

			UI_elements[i]->setTotalLock(true);
		//	UI_elements[i]->setVisible(true);

			for (int j = 0; j < UI_Objects.size(); j++)
			{
				UI_Objects[j]->moveYabs(Ypos_1 + UI_Height + UI_Height*(selectedHitbox - i));
		//		UI_Objects[j]->setVisible(true);
			}

		}
		/*
		else if (i < selectedHitbox - 3)
		{
			UI_elements[i]->setTotalLock(true);
	//		UI_elements[i]->setVisible(false);

			for (int j = 0; j < UI_Objects.size(); j++)
			{
	//			UI_Objects[j]->setVisible(false);
			}
		}
		*/

	}

}
