#include "UI.h"

#include "UI_HitboxLockScrolBar.h"

extern D2D_Graphics graphics;

extern D2D1::ColorF selectedboxColor;

UI_HitboxLockScrolBar::UI_HitboxLockScrolBar()
{
}

UI_HitboxLockScrolBar::UI_HitboxLockScrolBar(float Xcenter, float Ycenter, float width, float height, float left, float right, float up, float down, float activation, std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback, int ID_Model):
	UI_HitboxScrolBar{ Xcenter, Ycenter, width, height, left, right, up, down, activation, callback, ID_Model}, selectedHitbox{ 0 }

{
}


UI_HitboxLockScrolBar::~UI_HitboxLockScrolBar()
{
}

void UI_HitboxLockScrolBar::draw()
{
	Abstr_UI_Hitbox::draw();

	std::vector< std::shared_ptr < Abstr_UI_Hitbox>> & UI_elements = get_UI_Elements();
		
	if (UI_elements.size() == 0)
	{
		//printf(" no elements");
	}

	getActionIndicator()->draw();
	getActionText()->draw();

	selectionBox->draw();

	for (int i = 0; i < UI_elements.size(); ++i)
	{
		UI_elements[i]->draw();
	}

	getTopFiller()->draw();
	getBottomFiller()->draw();
}

void UI_HitboxLockScrolBar::action(ActionTrigger act, const D2D1_POINT_2F & coord)
{
	D2D1_POINT_2F	scrolcenter = getCenterCoordActionArea();
	float			scrolHeight = getHeight();
	float			UI_Height = getHeight_UI_element();
	float			selectboxScrollOffset = UI_Height / 6;

	//parameter to implement the scroll stop at the first and last element
	//float centerYlast = (get_UI_Elements().front()->getCenterCoordActionArea()).y;
	//float centerYfirst = (get_UI_Elements().back()->getCenterCoordActionArea()).y;

	//printf("UI_HitboxLocScrolbar 57: centerYlast %f  centerYfirst %f \n ", centerYlast, centerYfirst);


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
		
		//printf("hover off triggered \n");
		if (getMoveDownCounter() < getMoveLimit() && getMoveUpCounter() < getMoveLimit())
		{
			findSelectedHitbox();
			lock_UI_Elements();
			setMoveUpCounter(0);
			setMoveDownCounter(0);			
		}
		else
		{
			setKeepHoverState(true);
			action(ActionTrigger::HoverHold, coord);
		}
	
		break;
	case ActionTrigger::HoverHold:
		if ((coord.y > scrolcenter.y - scrolHeight / 2 + UI_Height + selectboxScrollOffset)		&&
			(coord.y < scrolcenter.y - scrolHeight / 2 + UI_Height * 2 - selectboxScrollOffset) &&
			(getMoveDownCounter() < getMoveLimit())						&& 
			(getMoveUpCounter() < getMoveLimit()))
		{
		}
		else
		{
		
			//if clause to check that you do not scrol farther then the first and last element
			//if (centerYlast > scrolcenter.y - scrolHeight / 2 + UI_Height*1.4 && centerYfirst < scrolcenter.y - scrolHeight / 2 + UI_Height*1.6)
			//{
				MoveHitbox(coord);
				setLastPoint(coord);
				/*
			}
			else
			{
				printf("UI_HitboxLocScrolbar 99: to far \n NOT fullfilled \n centerYlast %f > ref %f \n  centerYfirst %f < ref %f ", centerYlast , scrolcenter.y - scrolHeight / 2 + UI_Height*1.4 , centerYfirst , scrolcenter.y - scrolHeight / 2 + UI_Height*1.6);
				lock_UI_Elements();
			}
			*/

			findSelectedHitbox();

			if (getMoveDownCounter() < getMoveLimit() && getMoveUpCounter() < getMoveLimit())
			{
				
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
		UI_elements[i]->get_UI_Objects()[1]->changeColor(D2D1::ColorF::White);
		
		if (i == UI_elements.size() - 1)
		{
			//printf("UI_HitboxLockScrolBar 180: UI_center[0]: %f \n=", UI_center.y);
		}

		//if (UI_center.y > scrolcenter.y - scrolHeight / 2 + UI_Height && UI_center.y < scrolcenter.y - scrolHeight / 2 + UI_Height * 2)
		//printf("UI_HitboxLockScrolBar 183: UI_center[%d].y: %f \n=",i, UI_center.y);
		if (UI_center.y >= topBorder  && UI_center.y <= underborder)
		{

			selectedHitbox = i;
			UI_elements[i]->get_UI_Objects()[1]->changeColor(D2D1::ColorF::LightGreen);
			set = true;
			//printf("hitbox = %d \n", selectedHitbox);
		}
		else if (UI_center.y < topBorder) //logic to make sure that it jumps to the last or first hitbox when going over to far
		{
			top++;
		}
		else if (UI_center.y > underborder)
		{
			bottom++;
		}
		
	}
	//printf("UI_HitboxLockScrolBar 202: -----------------------------\n\n");
	if (!set)
	{
	//	printf("UI_HitboxLockScrolBar 200: lock to edge top %f bottom %f \n", top, bottom);
		if (top > bottom)
		{
			selectedHitbox = 0;
		}	
		else if (top == bottom)
		{
		}
		else
		{
		selectedHitbox = UI_elements.size() - 1;
		}
	}
	else
	{

	}

}

void UI_HitboxLockScrolBar::lock_UI_Elements()
{
	std::vector<std::shared_ptr<Abstr_UI_Hitbox>> & UI_elements = get_UI_Elements();
	if(UI_elements.size())
		UI_elements[selectedHitbox]->get_UI_Objects()[1]->changeColor(D2D1::ColorF::White);

	D2D1_POINT_2F	scrolcenter = getCenterCoordActionArea();
	float			scrolHeight = getHeight();
	float			UI_Height = (getHeight_UI_element());
	float			Ypos_1 = scrolcenter.y - scrolHeight / 2 + UI_Height/2;
	D2D1_POINT_2F	UI_center;					//parameter that will temporarily keep the center coordinates of each UI_element

	//printf("UI_HitboxLockScrolbar 250 : selectedBox= %d \n", selectedHitbox);

	for (int i = UI_elements.size()-1; i >= 0; i--)
	{	

		std::vector<std::shared_ptr<UI_Object>> UI_Objects = UI_elements[i]->get_UI_Objects();
		D2D1_POINT_2F lastpoint = getLastPoint();

		

		if (i > selectedHitbox + 1)
		{
			//UI_elements[i]->moveAbsY(Ypos_1 - UI_Height*( i- (selectedHitbox+1) ));
			//printf("UI_HitboxLockScrolbar 250 : lock top elements i = %d at Y = %f\n", i, Ypos_1 - (UI_Height)*(i - (selectedHitbox + 1)));
			UI_elements[i]->moveAbsYoriginalPos(Ypos_1 - (UI_Height)*(i - (selectedHitbox + 1)));
			UI_elements[i]->setCenterCoordActionArea(UI_elements[i]->getOriginalPos());
			//printf("UI_HitboxLockScrolbar 253 : BAAAA!! i = %d at Y = %f\n", i, UI_elements[i]->getOriginalPos().y);
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

void UI_HitboxLockScrolBar::setRefFrame(const D2D1_POINT_2F ref)
{
	refFrame = ref;
}

D2D1_POINT_2F UI_HitboxLockScrolBar::getRefFrame()
{
	return refFrame;
}

void UI_HitboxLockScrolBar::setSelectionBox(std::shared_ptr<UI_Object> selectionVisual)
{
	selectionBox = selectionVisual;
}

std::shared_ptr<UI_Object> UI_HitboxLockScrolBar::getSelectionBox()
{
	return selectionBox;
}


