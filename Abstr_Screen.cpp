#include "ImageLoader.h"

#include "UI.h"

#include "Abstr_Screen.h"

extern HWND  m_hWnd;
extern D2D_Graphics graphics;
float Xoffset = 100.0;

Abstr_Screen::Abstr_Screen()
{
}


Abstr_Screen::~Abstr_Screen()
{
	SafeRelease(background);
}

void Abstr_Screen::setModel(std::shared_ptr<Model> m)
{
	model = m;
	
	for (int i = 0; i < UI_Hitboxes.size(); i++)
	{
		UI_Hitboxes[i]->setModel(m);
	}
	m ->setUpdatUI(true);
}

std::shared_ptr<Model> Abstr_Screen::getModel()
{
	return model;
}

void Abstr_Screen::setUI(std::shared_ptr<UI> ui)
{
	UI_ptr = ui;
}

void Abstr_Screen::setUI(std::shared_ptr<UI> ui, const int w, const int h, ICoordinateMapper * m_pCoordinateMap, const int cDepthW, const int cDepthH)
{
	UI_ptr = ui;
	width = w;
	height = h;
	m_pCoordinateMapper = m_pCoordinateMap;
	cDepthWidth = cDepthW;
	cDepthHeight = cDepthH;

	printf("width = %d & height = %d", width, height);

}

const std::shared_ptr<UI>& Abstr_Screen::getUI()
{
	return UI_ptr;
}

void Abstr_Screen::createScreen(int width, int height)
{
	//background = graphics.createBitmap(L"IMAGE_D", 1920, 1080);
}

void Abstr_Screen::createScrolbar()
{

}

void Abstr_Screen::standardInitHitboxes()
{
	for(auto item: UI_Hitboxes)
	{
		item->addInputJoint(JointType_HandLeft);
		item->addInputJoint(JointType_HandRight);
		item->setUI(getUI());
	}

}

void Abstr_Screen::setShowRecordScreen(bool record)
{
	
}

void Abstr_Screen::activateHitboxes(D2D1_POINT_2F jointPoint, JointType type, HandState leftHand, HandState rightHand)
{
	if (type == JointType_HandRight)
	{
		SetDlgItemInt(m_hWnd, IDC_RIGHT_X, jointPoint.x, TRUE);
		SetDlgItemInt(m_hWnd, IDC_RIGHT_Y, jointPoint.y, TRUE);
	}
	if (type == JointType_HandLeft)
	{
		SetDlgItemInt(m_hWnd, IDC_LEFT_X, jointPoint.x, TRUE);
		SetDlgItemInt(m_hWnd, IDC_LEFT_Y, jointPoint.y, TRUE);
	}

	for (int i = 0; i < UI_Hitboxes.size(); ++i)
	{
		UI_Hitboxes[i]->attemptInteraction(jointPoint, type, leftHand, rightHand);
	}
}

void Abstr_Screen::updateHitboxes()
{
	for (const auto & hitbox : UI_Hitboxes)
	{
		hitbox->updateData();
	}
}

void Abstr_Screen::setRightHandBusy(const bool & busy)
{
	rightHandBusy = busy;
}

const bool & Abstr_Screen::getRightHandBusy()
{
	return rightHandBusy;
}

void Abstr_Screen::setLeftHandBusy(const bool & busy)
{
	leftHandBusy = busy;
}

const bool & Abstr_Screen::getLeftHandBusy()
{
	return leftHandBusy;
}

void Abstr_Screen::setDisplay_UI(const bool abs)
{
	display_UI = abs;
}

const bool Abstr_Screen::getDisplay_UI()
{
	return display_UI;
}

void Abstr_Screen::setDrawAbsCoord(const bool abs)
{
	drawAbsCoord = abs;
}

const bool Abstr_Screen::getDrawAbsCoord()
{
	return drawAbsCoord;
}

void Abstr_Screen::setWidth(int w)
{
	width = w;
}

int Abstr_Screen::getWidth()
{
	return width;
}

void Abstr_Screen::setHeight(int h)
{
	height = h;
}

int Abstr_Screen::getHeight()
{
	return height;
}

void Abstr_Screen::setCoordinateMapper(ICoordinateMapper * mapper)
{
	m_pCoordinateMapper = mapper;
}

ICoordinateMapper * Abstr_Screen::getCoordinateMapper()
{
	return m_pCoordinateMapper;
}

void Abstr_Screen::setDepthWidth(int depthW)
{
	cDepthWidth = depthW;
}

int Abstr_Screen::getDepthWidth()
{
	return cDepthWidth;
}

void Abstr_Screen::setDepthHeight(int depthH)
{
	cDepthHeight = depthH;
}

int Abstr_Screen::getDepthHeight()
{
	return cDepthHeight;
}

void Abstr_Screen::setInitRecording(bool play)
{
	initRecording = play;
}

bool Abstr_Screen::getInitRecording()
{
	return initRecording;
}

void Abstr_Screen::setRecordingWidth(float width)
{
	recordWidth = width;
}

float Abstr_Screen::getRecordingWidth()
{
	return recordWidth;
}

void Abstr_Screen::setRecordingHeight(float height)
{
	recordHeight = height;
}

float Abstr_Screen::getRecordingHeight()
{
	return recordHeight;
}


void Abstr_Screen::setRecordVisualCenter(D2D1_POINT_2F center)
{
	
}

std::vector<std::shared_ptr<Abstr_UI_Hitbox>>& Abstr_Screen::get_UI_Hitboxes()
{
	return UI_Hitboxes;
}

void Abstr_Screen::setRecordCountDown(int count)
{
}

int Abstr_Screen::getGestureClassID()
{
	return 0;
}

void Abstr_Screen::setGestureClassID(int ID)
{

}

void Abstr_Screen::set_UI_GestureID(int ID)
{
}

int Abstr_Screen::get_UI_GestureID()
{
	return 0;
}

void Abstr_Screen::autoplayGesture(int ID)
{
}

void Abstr_Screen::StopPlayGesture()
{

}

std::shared_ptr<UI_HitboxLockScrolBar> Abstr_Screen::getScrollbar()
{
	return std::shared_ptr<UI_HitboxLockScrolBar>();
}

bool Abstr_Screen::getShowRecordVisual()
{
	return false;
}

void Abstr_Screen::setPlayGesture(bool play)
{
}

bool Abstr_Screen::getPlayGesture()
{
	return false;
}

void Abstr_Screen::setPlayVisual(std::shared_ptr<UI_Hitbox> play)
{
}

std::shared_ptr<UI_Hitbox> Abstr_Screen::getPlayVisual()
{
	return nullptr;
}



void Abstr_Screen::drawFrames(std::vector<Frame>& relframes, std::vector<Frame>& absframes)
					
{
	std::vector<Frame> * frames = &absframes;
	D2D1_POINT_2F		absCenter;

	if (frames->size())
	{
		absCenter = graphics.BodyToScreen(frames->front().getJoints()[JointType_SpineMid].Position, width, height, m_pCoordinateMapper, cDepthWidth, cDepthHeight);
	}
	if(display_UI)
		drawUI();

	if (!drawAbsCoord)
	{
		frames = &relframes;

	}


	for (int j = frames->size() - 1; j > -1; --j)
	{
		std::vector<Joint> joints = (*frames)[j].getJoints();
		std::vector<D2D1_POINT_2F> jointPoints(JointType_Count);

		for (int i = 0; i < joints.size(); ++i)
		{
			jointPoints[i] = graphics.BodyToScreen(joints[i].Position, width, height, m_pCoordinateMapper, cDepthWidth, cDepthHeight);


			/*
			if (j == 0 && drawAbsCoord)
			{
				activateHitboxes(jointPoints[i], static_cast<JointType>(i), (*frames)[j].getLeftHand(), (*frames)[j].getRightHand());
			}
			*/
			//code to show coordinates of 3 joints
			/*
			if (j == 1)
			{
			ShowJointCoordinates(joints, 0);
			}
			else
			{
			ShowJointCoordinates(joints, 1);
			}
			*/
		}
		
		float boneThickness;

		if (!drawAbsCoord)		//when the relative coordinates are drawn and not the absolute
		{
			if (initRecording)	//when a recording is initiated
			{
				center.x = absCenter.x + Xoffset;
				center.y = absCenter.y;
				initRecording = false;
				UI_ptr->getScreen()->setRecordVisualCenter(center);
			}
			graphics.scaleSkeleton(jointPoints,height,height,recordWidth,recordHeight, center.x, center.y);

			boneThickness = 25.0 * (recordHeight/(height*0.8));
			if (j != 0)
				boneThickness = boneThickness*0.8;
			
			
		}
		else
		{
			graphics.scaleSkeleton(jointPoints, height, height, height, height, absCenter.x + Xoffset);
			//center.x = 350;
			//center.y = 400;
			boneThickness = 25.0;
		}



		

		for (int i = 0; i < joints.size(); ++i)
		{
			if (j == 0 && drawAbsCoord && display_UI)
			{
				activateHitboxes(jointPoints[i], static_cast<JointType>(i), (*frames)[j].getLeftHand(), (*frames)[j].getRightHand());
			}
		}
		
		graphics.DrawBody(joints, jointPoints, j, (*frames)[j].getLeftHand(), (*frames)[j].getRightHand(), boneThickness);
	}
	if (display_UI)
		drawTopUI();
}

void Abstr_Screen::drawFrame(const Frame & relframes)
{
		std::vector<Joint> joints = relframes.getJoints();
		std::vector<D2D1_POINT_2F> jointPoints(JointType_Count);

		for (int i = 0; i < joints.size(); ++i)
		{
			jointPoints[i] = graphics.BodyToScreen(joints[i].Position, width, height, m_pCoordinateMapper, cDepthWidth, cDepthHeight);			
		}
		graphics.scaleSkeleton(jointPoints, 0.8);
		graphics.DrawBody(joints, jointPoints, 0);

}

void Abstr_Screen::drawScaledFrame(const Frame & relframes, const float startWidth, const float startHeight, const float endWidth, const float endHeight, const float absXpos , const float absYpos)
{
	std::vector<Joint> joints = relframes.getJoints();
	std::vector<D2D1_POINT_2F> jointPoints(JointType_Count);

	for (int i = 0; i < joints.size(); ++i)
	{
		jointPoints[i] = graphics.BodyToScreen(joints[i].Position, width, height, m_pCoordinateMapper, cDepthWidth, cDepthHeight);
	}
	graphics.scaleSkeleton(jointPoints, startWidth, startHeight, endWidth, endHeight, absXpos, absYpos);
	//graphics.DrawBody(joints, jointPoints, 0);
	graphics.DrawBody(joints, jointPoints, 0, HandState::HandState_Open, HandState::HandState_Open, 25*(endHeight/(startHeight*0.8)));
	//graphics.DrawBody(joints, jointPoints, 0, HandState::HandState_Open, HandState::HandState_Open);
}

void Abstr_Screen::drawUI()
{
	D2D1_POINT_2F centerScreen = D2D1::Point2F(width / 2, height / 2);
	/*

	RGBQUAD*			test = new RGBQUAD[721 * 925];

	ImageLoader::LoadResourceImage(L"TEST_IMAGE", L"Image", 721, 925, test);

	graphics.drawBitmap(background, 1920,1080, centerScreen, width,height);
	graphics.drawBitmap(test, 721, 925, centerScreen, 721,925);
	*/

	//ID2D1Bitmap* testbitmap = graphics.createBitmap(L"TEST_IMAGE", 721, 925);
	//if(background)
	if (background != nullptr)
	{	
		graphics.drawBitmap(background, centerScreen, width, height);
	}
	else
	{
		graphics.drawRectangle(centerScreen, width, height, D2D1::ColorF::White);
	}



	//SafeRelease(testbitmap);

	for (int i = 0; i < UI_Hitboxes.size(); ++i)
	{
		//graphics.drawRectangle(UI_Objects[i]->getCenter(), UI_Objects[i]->getWidth(), UI_Objects[i]->getHeight(), UI_Objects[i]->getColor());
		UI_Hitboxes[i]->draw();
	}
}

void Abstr_Screen::drawTopUI()
{

}
/*
void Abstr_Screen::scaleSkeleton(std::vector<D2D1_POINT_2F>& jointPoints, float multiplier)
{
	D2D1_POINT_2F center;
	center.x = jointPoints[JointType_SpineMid].x;
	center.y = jointPoints[JointType_SpineMid].y;

	for (int j = 0; j < jointPoints.size(); ++j)
	{
		//Convert to coordinates relative to the spine
		if (j != JointType_SpineMid)
		{
			jointPoints[j].x = center.x + (jointPoints[j].x - center.x)*multiplier;
			jointPoints[j].y = center.y + (jointPoints[j].y - center.y)*multiplier;
		}
	}
}

void Abstr_Screen::scaleSkeleton(std::vector<D2D1_POINT_2F>& jointPoints, const float startWidth, const float startHeight, const float endWidth, const float endHeight, const float absXpos, const float absYpos)
{
	D2D1_POINT_2F center;
	center.x = jointPoints[JointType_SpineMid].x;
	center.y = jointPoints[JointType_SpineMid].y;

	float absXposition;
	float absYposition;

	if (absXpos == -1)
	{
		absXposition = center.x;
	}
	else
	{
		absXposition = absXpos;
	}

	if (absYpos == -1)
	{
		absYposition = center.y;
	}
	else
	{
		absYposition = absYpos;
	}

	float Xmultiplier = endWidth / startWidth;
	float Ymultiplier = endHeight / startHeight;

	for (int j = 0; j < jointPoints.size(); ++j)
	{
		//Convert to coordinates relative to the spine
		//if (j != JointType_SpineMid)
		//{
			//jointPoints[j].x = center.x + (jointPoints[j].x - center.x)*Xmultiplier;
			//jointPoints[j].y = center.y + (jointPoints[j].y - center.y)*Ymultiplier;

			jointPoints[j].x = absXpos + (jointPoints[j].x - center.x)*Xmultiplier;
			jointPoints[j].y = absYpos + (jointPoints[j].y - center.y)*Ymultiplier;
	//	}
	}
	
}
*/