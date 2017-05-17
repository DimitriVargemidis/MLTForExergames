#include <string>

#include "UI.h"

#include "recordScreen.h"

extern D2D_Graphics graphics;

recordScreen::recordScreen(): 
	Abstr_Screen(), showRecordVisual{false}, gestureClassID{1}
{
}

recordScreen::recordScreen(int ID) : 
	Abstr_Screen(), showRecordVisual{ false }, gestureClassID{ ID }
{
}


recordScreen::~recordScreen()
{
}

void recordScreen::createScreen(int width, int height)
{
	Abstr_Screen::createScreen(width, height);
	std::shared_ptr<Abstr_UI_Hitbox> backgroundHitbox(new UI_Hitbox());
	std::shared_ptr<UI_Object> groundLine = std::make_shared<UI_Object>(width/2, height*0.85, width, height*0.3, D2D1::ColorF::Black, D2D1::ColorF::White,10);
	backgroundHitbox->add_UI_Object(groundLine);

	std::shared_ptr<UI_Object> largeBanner = std::make_shared<UI_Object>(550, 50, 1000, 100, D2D1::ColorF::RoyalBlue);
	std::shared_ptr<Abstr_UI_Hitbox> stationairyObjects(new UI_Hitbox());
	stationairyObjects->add_UI_Object(largeBanner);

	//example of regular button
	std::shared_ptr<Abstr_UI_Hitbox> testHitbox(new UI_Hitbox(100 + 450, 200, 150, 150,UI_CallbackFunctions::updateHitboxes));
	std::shared_ptr<UI_Object> testObject = std::make_shared<UI_Object>(100.0 + 450, 200.0, 150.0, 150.0, D2D1::ColorF::Crimson);
	testHitbox->add_UI_Object(testObject);
	

	//example of slidebutton 
	std::shared_ptr<Abstr_UI_Hitbox> testHitbox2(new UI_HitboxSlideButton(100 + 450, 350, 150, 150, 0, 100, 0, 0, 0.5, UI_CallbackFunctions::recordGesture));
	std::shared_ptr<UI_Object> testObject2 = std::make_shared<UI_Object>(100.0 + 450, 350.0, 150.0, 150.0, D2D1::ColorF::Crimson);
	testHitbox2->add_UI_Object(testObject2);
	testHitbox2->setUpdateFunction(UI_Update_CallbackFunctions::stopRecord);

	//example of slidebutton with bitmap
	std::shared_ptr<Abstr_UI_Hitbox> testHitbox3(new UI_HitboxSlideButton(100 + 450, 300, 75, 150, 0, 0, 0, 200, 0.5, UI_CallbackFunctions::recordGesture));
	//std::shared_ptr<UI_Object> testObject3 = std::make_shared<UI_Object>(100.0 + 450, 300.0,50.0, 150.0, D2D1::ColorF::Crimson);
	std::shared_ptr<UI_Object> ropeImage = std::make_shared<UI_BitmapObject>(100.0 + 450, 300.0 - 100.0, 360.0*0.6, 780.0*0.6, D2D1::ColorF::Crimson, graphics.createBitmap(L"Rope",360,780));
	testHitbox3->add_UI_Object(ropeImage);
	//testHitbox3->add_UI_Object(testObject3);
	testHitbox3->setUpdateFunction(UI_Update_CallbackFunctions::stopRecord);


	//putting the hitboxes in the vector: this is also the order of drawing the UI_objects!!
	get_UI_Hitboxes().push_back(backgroundHitbox);
	//get_UI_Hitboxes().push_back(testHitbox);
	//get_UI_Hitboxes().push_back(testHitbox2);
	get_UI_Hitboxes().push_back(testHitbox3);
	createScrolbar();
	get_UI_Hitboxes().push_back(stationairyObjects);

	standardInitHitboxes();

	//make the visuals for the special record action visuals 
	std::shared_ptr<UI_Object> boxOutline = std::make_shared<UI_Object>(width/2, height/2, 700, 700, D2D1::ColorF::Black);
	std::shared_ptr<UI_Object> boxInside = std::make_shared<UI_Object>(	width/2, height / 2, 660, 660, D2D1::ColorF::White);
	std::shared_ptr<UI_TextObject> counter = std::make_shared<UI_TextObject>(width / 2, height / 2 -300, 700, 700, D2D1::ColorF::Red, L"Get Ready", 70);

	recordVisual.add_UI_Object(boxOutline);
	recordVisual.add_UI_Object(boxInside);

	recordVisual.setUI(getUI());

	recordCountDown.add_UI_Object(counter);

	recordCountDown.setUI(getUI());
}

void recordScreen::createScrolbar()
{
	float Xpos = 1000;
	float Ypos = 450;
	float width = 150;
	float height = 500;
	float deleteBoxXoffset = 150;
	float deleteBoxBorders = 20;

	//std::shared_ptr<UI> shared_ptr_this(this);
	std::shared_ptr<UI_HitboxScrolBar> scrollbar(new UI_HitboxLockScrolBar(Xpos, Ypos, width, height, 0, 0, 1000, 1000, 0.5, UI_CallbackFunctions::deleteGesture, 1));
	scrollbar->addInputJoint(JointType_HandLeft);
	scrollbar->addInputJoint(JointType_HandRight);
	scrollbar->setUI(getUI());
	scrollbar->setUpdateFunction(UI_Update_CallbackFunctions::updateScrolbarGestures);

	std::shared_ptr<UI_Object> background = std::make_shared<UI_Object>(Xpos, Ypos, width, height, D2D1::ColorF::Black, D2D1::ColorF::White,10);
	//UI_Objects.push_back(testObject5);
	std::shared_ptr<UI_Object> selectedBox = std::make_shared<UI_Object>(Xpos, Ypos- scrollbar->getHeight_UI_element(), width, scrollbar->getHeight_UI_element(), D2D1::ColorF(255.0/255.0, 102.0/255.0, 0));
	
	std::shared_ptr<UI_Object> borders = std::make_shared<UI_Object>(Xpos+ deleteBoxXoffset, Ypos - scrollbar->getHeight_UI_element(), scrollbar->getWidth_UI_element(), scrollbar->getHeight_UI_element(), D2D1::ColorF::Black);
	std::shared_ptr<UI_Object> filling = std::make_shared<UI_Object>(Xpos+ deleteBoxXoffset, Ypos - scrollbar->getHeight_UI_element(), scrollbar->getWidth_UI_element()- deleteBoxBorders, scrollbar->getHeight_UI_element()- deleteBoxBorders, D2D1::ColorF::White);
	std::shared_ptr<UI_Object> text = std::make_shared<UI_TextObject>(Xpos+ deleteBoxXoffset, Ypos - scrollbar->getHeight_UI_element(), scrollbar->getWidth_UI_element()- deleteBoxBorders, scrollbar->getHeight_UI_element()- deleteBoxBorders, D2D1::ColorF::Black,L"DEL", 40);
	
	
	//add scrollbar layout
	scrollbar->add_UI_Object(background);
	scrollbar->add_UI_Object(selectedBox);

	//add deleeteBox layour
	scrollbar->add_UI_Object(borders);
	scrollbar->add_UI_Object(filling);
	scrollbar->add_UI_Object(text);

	get_UI_Hitboxes().push_back(scrollbar);
}

void recordScreen::setShowRecordScreen(bool record)
{
	showRecordVisual = record;
}

void recordScreen::drawUI()
{
	Abstr_Screen::drawUI();
	if (showRecordVisual)
	{
		recordVisual.draw();
		recordCountDown.draw();
	}

}

void recordScreen::drawTopUI()
{
	if (playGesture)
	{
		recordVisual.draw();
		if (playGestureIndex < AutoPlayGesture->getFrames().size())
		{
			//very inefficient!!
			//drawScaledFrame((AutoPlayGesture->getFrames())[0], getWidth(), getHeight(), 575, 425);// , 1000, 300);
			//drawScaledFrame((AutoPlayGesture->getFrames())[(AutoPlayGesture->getFrames().size() - 1)], getWidth(), getHeight(), 575, 425, 1000, 600);

			//drawFrame((AutoPlayGesture->getFrames())[playGestureIndex]);
			drawScaledFrame((AutoPlayGesture->getFrames())[playGestureIndex], getWidth(), getHeight(), 660, 660);
			playGestureIndex++;
		}
		else
		{
			StopPlayGesture();
		}

	}
}

void recordScreen::setRecordCountDown(int count)
{
	if (count > 3)
	{
		recordCountDown.get_UI_Objects()[0]->setText(L"Get Ready");
	}
	else if (count <= 3 && count > 0)
	{
		recordCountDown.get_UI_Objects()[0]->setText(std::to_wstring(count));
	}
	if (count == 0)
	{
		recordCountDown.get_UI_Objects()[0]->setText(L"Go!");
	}

}

void recordScreen::setGestureClassID(int ID)
{
	gestureClassID = ID;
}

int recordScreen::getGestureClassID()
{
	return gestureClassID;
}

void recordScreen::autoplayGesture(int ID)
{
	std::shared_ptr<GestureClass> gestureClass = getModel()->getGestureClassByID(gestureClassID);
	if (gestureClass != nullptr)
	{
		AutoPlayGesture = gestureClass->getGestureWithID(ID);
		playGesture = true;
		playGestureIndex = 0;
	}
	else
	{
		printf("no gestureClass with that ID !!\n");
	}
}

void recordScreen::StopPlayGesture()
{
	playGesture = false;
	playGestureIndex = 0;
}


