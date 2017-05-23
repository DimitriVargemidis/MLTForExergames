#include <string>
#include <vector>

#include "UI.h"

#include "recordScreen.h"

extern D2D_Graphics				graphics;
extern std::shared_ptr<Model>	model_global;
extern float Xoffset;

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

void recordScreen::drawFrames(std::vector<Frame>& relframes, std::vector<Frame>& absframes)
{
	if (model_global->getGestureClassByID(gestureClassID) != nullptr)
	{
		//here the 2 guys that are placed behind the active person during recording are added
		if (model_global->getGestureClassByID(gestureClassID)->getGestures().front()->getFrames().size() != 0) 
		{
			relframes.push_back(model_global->getGestureClassByID(gestureClassID)->getGestures().front()->getFrames().front());
			relframes.push_back(model_global->getGestureClassByID(gestureClassID)->getGestures().front()->getFrames().back());
		}
	}

	Abstr_Screen::drawFrames(relframes, absframes);
}

void recordScreen::activateHitboxes(D2D1_POINT_2F jointPoint, JointType type, HandState leftHand, HandState rightHand)
{
	scrollbar->attemptInteraction(jointPoint, type,  leftHand,  rightHand);
	Abstr_Screen::activateHitboxes(jointPoint, type, leftHand,  rightHand);
}

void recordScreen::updateHitboxes()
{
	scrollbar->updateData();
	Abstr_Screen::updateHitboxes();
}

void recordScreen::createScreen(int width, int height)
{
	

	Abstr_Screen::createScreen(width, height);
	std::shared_ptr<Abstr_UI_Hitbox> backgroundHitbox(new UI_Hitbox());
	std::shared_ptr<UI_Object> groundLine = std::make_shared<UI_Object>(width/2, height*0.85, width, height*0.3, D2D1::ColorF::Black, D2D1::ColorF::White,10);
	backgroundHitbox->add_UI_Object(groundLine);

	std::shared_ptr<UI_Object> largeBanner = std::make_shared<UI_Object>(550, 50, 1000, 100, D2D1::ColorF::Black, D2D1::ColorF::White , 10);
	std::shared_ptr<UI_Object> title = std::make_shared<UI_TextObject>(550, 50, 1000-20, 100-20, D2D1::ColorF::Black,L"OpnameScherm",60, DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	std::shared_ptr<Abstr_UI_Hitbox> stationairyObjects(new UI_Hitbox());
	stationairyObjects->add_UI_Object(largeBanner);
	stationairyObjects->add_UI_Object(title);

	//record rope in slidebutton with bitmap
	std::shared_ptr<Abstr_UI_Hitbox> testHitbox3(new UI_HitboxSlideButton(100 + 450 + Xoffset, 300, 75, 150, 0, 0, 0, 200, 0.5, UI_CallbackFunctions::recordGesture));
	std::shared_ptr<UI_BitmapObject> ropeImage = std::make_shared<UI_BitmapObject>(100.0 + 450 + Xoffset, 300.0 - 100.0, 360.0*0.6, 780.0*0.6, D2D1::ColorF::Crimson, graphics.createBitmap(L"Rope",360,780));
	ropeImage->setHoverBitmap(graphics.createBitmap(L"RopeHover", 360, 780));
	ropeImage->setHandActiveBitmap(graphics.createBitmap(L"RopeHandActive", 360, 780));
	testHitbox3->add_UI_Object(ropeImage);
	testHitbox3->setUpdateFunction(UI_Update_CallbackFunctions::stopRecord);


	//putting the hitboxes in the vector: this is also the order of drawing the UI_objects!!
	get_UI_Hitboxes().push_back(backgroundHitbox);
	get_UI_Hitboxes().push_back(testHitbox3);
	createScrolbar();
	get_UI_Hitboxes().push_back(stationairyObjects);

	standardInitHitboxes();

	float recordWidth = getRecordingWidth()*1.3;
	float recordHeight = getRecordingHeight()*1.3;

	//make the visual for the record box visual
	std::shared_ptr<UI_Object> boxOutline = std::make_shared<UI_Object>(width/2 + Xoffset+100, height/2, recordWidth, recordHeight, D2D1::ColorF::Black, D2D1::ColorF::White,15);
	
	//add UI_Object to UI_Hitbox recordvisual
	recordVisual.add_UI_Object(boxOutline);
	recordVisual.setUI(getUI());
	//recordVisual.setUpdateFunction(UI_Update_CallbackFunctions::updateScrolbarGestures);

	//make the counter for the record visual 
	std::shared_ptr<UI_TextObject> counter = std::make_shared<UI_TextObject>(width / 2 + Xoffset+100, height / 2 - recordHeight/2 + 50, recordWidth, recordHeight, D2D1::ColorF::Black, L"Get Ready", 70, DWRITE_TEXT_ALIGNMENT_CENTER);

	//add UI_Object to UI_Hitbox recordCountDown
	recordCountDown.add_UI_Object(counter);
	recordCountDown.setUI(getUI());

	//make the visual for the play box
	std::shared_ptr<UI_Object> playbox = std::make_shared<UI_Object>(playCenterpoint.x, playCenterpoint.y,playWidth, playHeight, D2D1::ColorF::LightGray, D2D1::ColorF::White, 15);
	std::shared_ptr<UI_Object> progressBar = std::make_shared<UI_Object>(playCenterpoint.x, playCenterpoint.y + playWidth/2 - 100/2, playWidth-30, 100-30, D2D1::ColorF::LightBlue);
	std::shared_ptr<UI_Object> UI_GestureID_Text = std::make_shared<UI_TextObject>(playCenterpoint.x, playCenterpoint.y, playWidth-30, playHeight-30, D2D1::ColorF::Black,L"0",100, DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	
	progressBar->setHorFillPercen(0.0F);
	UI_GestureID_Text->setVisible(false);

	//add UI_Object to UI_Hitbox recordCountDown
	playVisual = std::make_shared<UI_Hitbox>();
	playVisual->add_UI_Object(playbox);				//0
	playVisual->add_UI_Object(progressBar);			//1
	playVisual->add_UI_Object(UI_GestureID_Text);	//2
	playVisual->setUI(getUI());
	playVisual->setUpdateFunction(UI_Update_CallbackFunctions::updatePlayVisual);

}

void recordScreen::createScrolbar()
{
	float Xpos = 1000;
	float Ypos = 450;
	float width = 150;
	float height = 500;
	float deleteBoxXoffset = 115;
	float deleteBoxBorders = 20;

	//std::shared_ptr<UI> shared_ptr_this(this);
	scrollbar = std::make_shared<UI_HitboxLockScrolBar>(Xpos + Xoffset, Ypos, width, height, 0, 0, 1000000, 1000000, 0.5, UI_CallbackFunctions::deleteGesture, 1);
	scrollbar->addInputJoint(JointType_HandLeft);
	scrollbar->addInputJoint(JointType_HandRight);
	scrollbar->setUI(getUI());
	scrollbar->setUpdateFunction(UI_Update_CallbackFunctions::updateScrolbarGestures);
	
	//scrollbar visual elements
	std::shared_ptr<UI_Object> background = std::make_shared<UI_Object>(Xpos + Xoffset, Ypos, width, height, D2D1::ColorF::Black, D2D1::ColorF::White,10);
	std::shared_ptr<UI_Object> selectedBox = std::make_shared<UI_Object>(Xpos + Xoffset, Ypos- scrollbar->getHeight_UI_element(), width, scrollbar->getHeight_UI_element(), D2D1::ColorF(255.0/255.0, 102.0/255.0, 0));
	
	float extraWidth = 75;

	//deletebox elements
	std::shared_ptr<UI_Object> borders = std::make_shared<UI_Object>(Xpos+ deleteBoxXoffset + Xoffset + extraWidth/2, Ypos - scrollbar->getHeight_UI_element(), scrollbar->getWidth_UI_element() + extraWidth, scrollbar->getHeight_UI_element(), D2D1::ColorF::Black);
	std::shared_ptr<UI_Object> filling = std::make_shared<UI_Object>(Xpos+ deleteBoxXoffset + Xoffset + extraWidth/2, Ypos - scrollbar->getHeight_UI_element(), scrollbar->getWidth_UI_element()- deleteBoxBorders + extraWidth, scrollbar->getHeight_UI_element()- deleteBoxBorders, D2D1::ColorF::White);
	std::shared_ptr<UI_Object> indicator = std::make_shared<UI_Object>(Xpos + deleteBoxXoffset + Xoffset + extraWidth/2, Ypos - scrollbar->getHeight_UI_element(), scrollbar->getWidth_UI_element() - deleteBoxBorders + extraWidth, scrollbar->getHeight_UI_element() - deleteBoxBorders, D2D1::ColorF::Red);
	indicator->setHorFillPercen(0.0F);
	std::shared_ptr<UI_TextObject> text = std::make_shared<UI_TextObject>(Xpos+ deleteBoxXoffset + Xoffset + extraWidth/2, Ypos - scrollbar->getHeight_UI_element(), scrollbar->getWidth_UI_element()- deleteBoxBorders + extraWidth, scrollbar->getHeight_UI_element()- deleteBoxBorders, D2D1::ColorF::Black,L"DEL", 40);
	
	//add deleeteBox layout
	scrollbar->add_UI_Object(borders);
	scrollbar->add_UI_Object(filling);
	//scrollbar->add_UI_Object(text);

	//add scrollbar layout
	scrollbar->add_UI_Object(background);
	//scrollbar->add_UI_Object(selectedBox);
	scrollbar->setSelectionBox(selectedBox);
	scrollbar->setActionText(text);
	scrollbar->setActionIndicator(indicator);

	//get_UI_Hitboxes().push_back(scrollbar);
}

void recordScreen::setShowRecordScreen(bool record)
{
	showRecordVisual = record;
}

void recordScreen::drawUI()
{
	Abstr_Screen::drawUI();

	scrollbar->draw();

	if (showRecordVisual)
	{
		recordVisual.draw();
	}
	
}

void recordScreen::drawTopUI()
{
	playVisual->draw();

	if (playGesture)
	{
		if (AutoPlayGesture != nullptr)
		{
			playVisual->get_UI_Objects()[2]->setVisible(true);
			if (playGestureIndex < AutoPlayGesture->getFrames().size()) //read violation op deze plaats!
			{
				//draws the scaled version of the frame
				drawScaledFrame((AutoPlayGesture->getFrames())[playGestureIndex], getHeight(), getHeight(), playWidth*0.8, playHeight*0.8, playCenterpoint.x, playCenterpoint.y - 25);

				//calculate how far in percentage (float 0 -> 1) the playGestureIndex is to reaching the end to draw the progessbar
				float playPercentage = (static_cast<float>(playGestureIndex)) / (static_cast<float>(AutoPlayGesture->getFrames().size() - 1));
				playVisual->get_UI_Objects()[1]->setHorFillPercen(playPercentage);

				playGestureIndex++;
				playGestureTimer = Clock::now();
			}
			else
			{
				drawScaledFrame((AutoPlayGesture->getFrames()).back(), getHeight(), getHeight(), playWidth*0.8, playHeight*0.8, playCenterpoint.x, playCenterpoint.y - 25);
				/*
				auto t2 = Clock::now();
				long time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - playGestureTimer).count();
				if (time >= 1000)
				{
					//StopPlayGesture();
					playGestureIndex = 0;
				}
				*/
			}
		}
	}
	else
	{
		playVisual->get_UI_Objects()[1]->setHorFillPercen(0.0F);
		playVisual->get_UI_Objects()[2]->setVisible(false);
	}
	if (showRecordVisual)
	{
		recordCountDown.draw();
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
		recordVisual.get_UI_Objects()[0]->changeBorderColor(D2D1::ColorF::Red);
	}
	if (count == -1)
	{
		recordCountDown.get_UI_Objects()[0]->setText(L"Done!");
		recordVisual.get_UI_Objects()[0]->changeBorderColor(D2D1::ColorF::Black);
	}

}

void recordScreen::setGestureClassID(int ID)
{
	gestureClassID = ID;
	model_global->setUpdatUI(true);
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
		if (AutoPlayGesture == nullptr)
		{
			if (!playGesture)
			{
				AutoPlayGesture = gestureClass->getGestureWithID(ID);
				playGesture = true;
				playGestureIndex = 0;
			}
		}
		else
		{
			if (!playGesture || AutoPlayGesture->getGestureID() != ID)
			{
				AutoPlayGesture = gestureClass->getGestureWithID(ID);
				playGesture = true;
				playGestureIndex = 0;
			}
		}
		
	}
	else
	{
		//printf("no gestureClass with that ID !!\n");
	}
}

void recordScreen::StopPlayGesture()
{
	playGesture = false;
	playGestureIndex = 0;
}

void recordScreen::set_UI_GestureID(int ID)
{
	UI_GestureID = ID;
	playVisual->get_UI_Objects()[2]->setText(std::to_wstring(ID));
}

int recordScreen::get_UI_GestureID()
{
	return UI_GestureID;
}

void recordScreen::setRecordVisualCenter(D2D1_POINT_2F center)
{
	std::vector<std::shared_ptr<UI_Object>> & objects = recordVisual.get_UI_Objects();

	for (int i = 0; i< objects.size(); ++i)
	{	
			objects[i]->setCenter(center);
	}
	center.y -= (getRecordingHeight()*1.3)/2 - 50;
	recordCountDown.get_UI_Objects()[0]->setCenter(center);
}

bool recordScreen::getShowRecordVisual()
{
	return showRecordVisual;
}

void recordScreen::setPlayGesture(bool play)
{
	playGesture = play;
}

bool recordScreen::getPlayGesture()
{
	return playGesture;
}

void recordScreen::setPlayVisual(std::shared_ptr<UI_Hitbox> play)
{
	playVisual = play;
}

std::shared_ptr<UI_Hitbox> recordScreen::getPlayVisual()
{
	return playVisual;
}

std::shared_ptr<UI_HitboxLockScrolBar> recordScreen::getScrollbar()
{
	return scrollbar;
}




