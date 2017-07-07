#include <string>
//author: Christiaan Vanbergen 

#include "Model.h"
#include "UI.h"
#include "Gesture.h"

#include "UI_Update_CallbackFunctions.h"

extern  std::shared_ptr<UI> UI_global;
extern  std::shared_ptr<Model> model_global;

void UI_Update_CallbackFunctions::standardUpdateFunction(Abstr_UI_Hitbox * hitbox)
{

}

void UI_Update_CallbackFunctions::stopRecord(Abstr_UI_Hitbox * hitbox)
{
	//if the model is no longer in the process of recording 
	if(!(model_global->getRefresh() || model_global->getRecording()))
	{
		//turn of the record screen and show the puppet in absolute coordinates
		UI_global->getScreen()->setShowRecordScreen(false);
		UI_global->getScreen()->setDrawAbsCoord(true);
	}
	else
	{
		//update the stage of recording
		UI_global->getScreen()->setRecordCountDown(model_global->getCountDown());
	}

	if (UI_global->getScreen()->getShowRecordVisual())
	{
		hitbox->moveAbsY(hitbox->getOriginalPos().y);
	}
	
	
}



void UI_Update_CallbackFunctions::updateScrolbarGestures(Abstr_UI_Hitbox * hitbox)
{
	//look for the active GestureClass of the UI_Screen in the Model
	std::shared_ptr<GestureClass> ActiveGestureClass = model_global->getGestureClassByID(UI_global->getScreen()->getGestureClassID());
	if (ActiveGestureClass != nullptr)
	{

		std::vector<std::shared_ptr<Gesture>> gestures = ActiveGestureClass->getGestures();
		//clear the current items in the scrollbar hitbox
		hitbox->clear_UI_elements();
		//set the progressbar in the replay screen to the beginning
		hitbox->getActionIndicator()->setHorFillPercen(0.0F);
		
		//variable to temporarily store the ID of the current Gesture
		int gestureID;	
		
		//go through all the gestures in the gesture class
		for (int i = 0; i < gestures.size(); i++)
		{
			//Get the first and last frame to be used as preview
			gestureID = gestures[i]->getGestureID();
			std::vector<Frame> frames;
			frames.push_back(gestures[i]->getFrames()[0]);
			frames.push_back(gestures[i]->getFrames()[(gestures[i]->getFrames().size() - 1)]);

			//create all the necessary UI_Objects and the hitbox to display this gesture
			//the size and position of these elements don't matter they are resized in the scrollbar hitbox
			std::shared_ptr<UI_Object> borders = std::make_shared<UI_Object>(400 + 250, 350, 150, 150, D2D1::ColorF::Black);
			std::shared_ptr<UI_Object> filling = std::make_shared<UI_Object>(400 + 250, 350, 150, 150, D2D1::ColorF::White);
			std::shared_ptr<UI_Object> framesObject = std::make_shared<UI_FramesObject>(400 + 250, 350, 150, 150, D2D1::ColorF::White, frames);
			std::shared_ptr<UI_Object> text = std::make_shared<UI_TextObject>(400 + 250 - 30, 350, 75, 150, D2D1::ColorF::Black, std::to_wstring(i + 1), 40, DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
			std::shared_ptr<Abstr_UI_Hitbox> HitboxElement(new UI_HitboxHoverSlideButton(400 + 250 + 50, 350, 100, 150, 0, 100, 0, 0, 0.6, UI_CallbackFunctions::deleteGesture, gestureID, 0));

			//add the UI_Objects to the HitboxElement
			HitboxElement->add_UI_Object(borders);
			HitboxElement->add_UI_Object(filling);
			HitboxElement->add_UI_Object(framesObject);
			HitboxElement->add_UI_Object(text);
		
			//set some settings
			HitboxElement->addInputJoint(JointType_HandLeft);
			HitboxElement->addInputJoint(JointType_HandRight);
			HitboxElement->setUI(UI_global);
			HitboxElement->setModel(model_global);
			
			//add HitboxElement to the scrollbar hitbox
			hitbox->add_UI_Element(HitboxElement);
		}
	}
	else
	{
		//if there were nog Gestures in the GestureClass just clear the scrollbar hitbox
		hitbox->clear_UI_elements();
	}
}

void UI_Update_CallbackFunctions::updatePlayVisual(Abstr_UI_Hitbox * hitbox)
{
	//color the replay screen green when something is replayed color and Light Gray when nothing is replayed
	if (UI_global->getScreen()->getPlayGesture())
	{
		(hitbox->get_UI_Objects()[0])->changeBorderColor(D2D1::ColorF::Green);
	}
	else
	{
		(hitbox->get_UI_Objects()[0])->changeBorderColor(D2D1::ColorF::LightGray);
	}
}


