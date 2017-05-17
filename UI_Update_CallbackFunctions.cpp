#include <string>

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
	if(!(model_global->getRefresh() || model_global->getRecording()))
	{
		UI_global->getScreen()->setShowRecordScreen(false);
		UI_global->getScreen()->setDrawAbsCoord(true);
	}
	else
	{
		UI_global->getScreen()->setRecordCountDown(model_global->getCountDown());
	}
	
}

void UI_Update_CallbackFunctions::RecordCounter(Abstr_UI_Hitbox * hitbox)
{
	
}

void UI_Update_CallbackFunctions::updateScrolbarGestures(Abstr_UI_Hitbox * hitbox)
{
	//std::shared_ptr<UI> shared_ptr_this(this);
	std::shared_ptr<GestureClass> ActiveGestureClass = model_global->getGestureClassByID(UI_global->getScreen()->getGestureClassID());
	std::vector<std::shared_ptr<Gesture>> gestures = ActiveGestureClass->getGestures();

	hitbox->clear_UI_elements();

	int gestureID;

	//for (int i = gestures.size()-1; i >= 0; i--)
	for (int i = 0; i < gestures.size(); i++)
	{
		gestureID = gestures[i]->getGestureID();
		std::vector<Frame> frames;
		frames.push_back(gestures[i]->getFrames()[0]);
		frames.push_back(gestures[i]->getFrames()[(gestures[i]->getFrames().size()-1)]);

		//the size and position of these elements don't matter they are resized in the scrollbar hitbox
		std::shared_ptr<UI_Object> borders = std::make_shared<UI_Object>(400 + 250, 350, 150, 150, D2D1::ColorF::Black);
		std::shared_ptr<UI_Object> filling= std::make_shared<UI_Object>(400 + 250, 350, 150, 150, D2D1::ColorF::White);
		std::shared_ptr<UI_Object> framesObject = std::make_shared<UI_FramesObject>(400 + 250, 350, 150, 150, D2D1::ColorF::White, frames);
		std::shared_ptr<UI_Object> text = std::make_shared<UI_TextObject>(400 + 250 - 30, 350, 75, 150, D2D1::ColorF::Black, std::to_wstring(i + 1), 40,DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
		std::shared_ptr<Abstr_UI_Hitbox> testHitbox7(new UI_HitboxHoverSlideButton(400 + 250+50, 350, 100, 150, 0, 100, 0, 0, 0.8, UI_CallbackFunctions::deleteGesture, gestureID,0));
		
		testHitbox7->add_UI_Object(borders);
		testHitbox7->add_UI_Object(filling);
		testHitbox7->add_UI_Object(framesObject);
		testHitbox7->add_UI_Object(text);
		//testHitbox7->add_UI_Object(text1);
		testHitbox7->addInputJoint(JointType_HandLeft);
		testHitbox7->addInputJoint(JointType_HandRight);
		testHitbox7->setUI(UI_global);
		testHitbox7->setModel(model_global);

		hitbox->add_UI_Element(testHitbox7);
	}
}


