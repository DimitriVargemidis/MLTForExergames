#include <strsafe.h>
#include <Windows.h>
#include <Winuser.h>
#include <memory>

#include "stdafx.h"
#include "D2D_Graphics.h"
#include "resource.h"
#include "Main.h"
#include "Frame.h"

#include "UI.h"




UI::UI() :
m_pCoordinateMapper(NULL),
m_hWnd(NULL),
graphics {}
{
	
	std::shared_ptr<UI_Object> testObject = std::make_shared<UI_Object>();
	UI_Objects.push_back(testObject);
	std::shared_ptr<Abstr_UI_Hitbox> testHitbox(new UI_Hitbox());
	UI_Hitboxes.push_back(testHitbox);
	UI_Hitboxes[0]->add_UI_Object(testObject);
	UI_Hitboxes[0]->addInputJoint(JointType_HandLeft);
	UI_Hitboxes[0]->addInputJoint(JointType_HandRight);
	
	//object 2 
	std::shared_ptr<UI_Object> testObject2 = std::make_shared<UI_Object>(100.0,350.0,150.0,150.0, D2D1::ColorF::White);
	UI_Objects.push_back(testObject2);
	std::shared_ptr<Abstr_UI_Hitbox> testHitbox2(new UI_HitboxSlideButton( 100, 350, 150, 150, 0,100,0,0));
	UI_Hitboxes.push_back(testHitbox2);
	UI_Hitboxes[1]->add_UI_Object(testObject2);
	UI_Hitboxes[1]->addInputJoint(JointType_HandLeft);
	UI_Hitboxes[1]->addInputJoint(JointType_HandRight);
	

	
	//test scrollbar element 1
	std::shared_ptr<UI_Object> testObject3 = std::make_shared<UI_Object>(400, 350, 150, 150, D2D1::ColorF::White);
	
	std::shared_ptr<Abstr_UI_Hitbox> testHitbox3(new UI_HitboxHoverSlideButton(400, 350, 150, 150, 0, 100, 0, 0));
	UI_Hitboxes.push_back(testHitbox3);
	UI_Hitboxes[2]->add_UI_Object(testObject3);
	UI_Hitboxes[2]->addInputJoint(JointType_HandLeft);
	UI_Hitboxes[2]->addInputJoint(JointType_HandRight);

	//test scrollbar element 2
	std::shared_ptr<UI_Object> testObject4 = std::make_shared<UI_Object>(400, 350, 150, 150, D2D1::ColorF::White);
	
	std::shared_ptr<Abstr_UI_Hitbox> testHitbox4(new UI_HitboxHoverSlideButton(400, 350, 150, 150, 0, 100, 0, 0));
	UI_Hitboxes.push_back(testHitbox4);
	UI_Hitboxes[3]->add_UI_Object(testObject4);
	UI_Hitboxes[3]->addInputJoint(JointType_HandLeft);
	UI_Hitboxes[3]->addInputJoint(JointType_HandRight);
	

	//object 3
	std::shared_ptr<UI_Object> testObject5 = std::make_shared<UI_Object>(550, 350, 150, 500, D2D1::ColorF::Red);
	UI_Objects.push_back(testObject5);
	std::shared_ptr<UI_Object> selectedBox = std::make_shared<UI_Object>(550, 250, 150, 100, D2D1::ColorF::Blue);
	UI_Objects.push_back(selectedBox);
	std::shared_ptr<UI_Object> topfiller = std::make_shared<UI_Object>(550, 50, 150, 100, D2D1::ColorF::Black);
	std::shared_ptr<UI_Object> bottomfiller = std::make_shared<UI_Object>(550, 650, 150, 100, D2D1::ColorF::Black);
	std::shared_ptr<Abstr_UI_Hitbox> testHitbox5(new UI_HitboxLockScrolBar(550, 350, 150, 500, 0, 0, 1000, 1000));
	UI_Hitboxes.push_back(testHitbox5);
	UI_Hitboxes[4]->add_UI_Object(testObject5);
	UI_Hitboxes[4]->add_UI_Object(topfiller);
	UI_Hitboxes[4]->add_UI_Object(bottomfiller);
	UI_Hitboxes[4]->addInputJoint(JointType_HandLeft);
	UI_Hitboxes[4]->addInputJoint(JointType_HandRight);
	UI_Hitboxes[4]->add_UI_Element(testHitbox3);
	UI_Hitboxes[4]->add_UI_Element(testHitbox4);

	//test scrollbar element 3
	std::shared_ptr<UI_Object> testObject6 = std::make_shared<UI_Object>(400, 350, 150, 150, D2D1::ColorF::White);

	std::shared_ptr<Abstr_UI_Hitbox> testHitbox6(new UI_HitboxHoverSlideButton(400, 350, 150, 150, 0, 100, 0, 0));
	UI_Hitboxes.push_back(testHitbox6);
	UI_Hitboxes[5]->add_UI_Object(testObject6);
	UI_Hitboxes[5]->addInputJoint(JointType_HandLeft);
	UI_Hitboxes[5]->addInputJoint(JointType_HandRight);
	UI_Hitboxes[4]->add_UI_Element(testHitbox6);

	//test scrollbar element 4
	std::shared_ptr<UI_Object> testObject7 = std::make_shared<UI_Object>(400, 350, 150, 150, D2D1::ColorF::White);

	std::shared_ptr<Abstr_UI_Hitbox> testHitbox7(new UI_HitboxHoverSlideButton(400, 350, 150, 150, 0, 100, 0, 0));
	UI_Hitboxes.push_back(testHitbox7);
	UI_Hitboxes[6]->add_UI_Object(testObject7);
	UI_Hitboxes[6]->addInputJoint(JointType_HandLeft);
	UI_Hitboxes[6]->addInputJoint(JointType_HandRight);
	UI_Hitboxes[4]->add_UI_Element(testHitbox7);

	//test scrollbar element 5
	std::shared_ptr<UI_Object> testObject8 = std::make_shared<UI_Object>(400, 350, 150, 150, D2D1::ColorF::White);

	std::shared_ptr<Abstr_UI_Hitbox> testHitbox8(new UI_HitboxHoverSlideButton(400, 350, 150, 150, 0, 100, 0, 0));
	UI_Hitboxes.push_back(testHitbox8);
	UI_Hitboxes[7]->add_UI_Object(testObject8);
	UI_Hitboxes[7]->addInputJoint(JointType_HandLeft);
	UI_Hitboxes[7]->addInputJoint(JointType_HandRight);
	UI_Hitboxes[4]->add_UI_Element(testHitbox8);

	UI_Objects.push_back(testObject3);
	UI_Objects.push_back(testObject4);
	UI_Objects.push_back(testObject6);
	UI_Objects.push_back(testObject7);
	UI_Objects.push_back(testObject8);
	UI_Objects.push_back(bottomfiller);
	//UI_Objects.push_back(topfiller);
}


UI::~UI()
{
	// clean up Direct2D
	graphics.CleanD2D();

	// done with coordinate mapper
	SafeRelease(m_pCoordinateMapper);
}


/// <summary>
/// Creates the main window and begins processing
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
int UI::Run(HINSTANCE hInstance, int nCmdShow)
{
	msg = { 0 };
	
	// Dialog custom window class
	ZeroMemory(&wc, sizeof(wc));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbWndExtra = DLGWINDOWEXTRA;
	wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_APP));
	wc.lpfnWndProc = DefDlgProcW;
	wc.lpszClassName = L"BodyBasicsAppDlgWndClass";

	if (!RegisterClassW(&wc))
	{
		return 0;
	}

	// Create main application window
	HWND	hWnd = CreateDialogParamW(
			NULL,
			MAKEINTRESOURCE(IDD_APP),	
			NULL,
			(DLGPROC)UI::MessageRouter,
			reinterpret_cast<LPARAM>(this));

	hWndApp = &hWnd;

	// Show window
	ShowWindow(*hWndApp, nCmdShow);

	return 1;

}

/// <summary>
/// Handles window messages, passes most to the class instance to handle
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK UI::MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UI* pThis = NULL;

	if (WM_INITDIALOG == uMsg)
	{
		pThis = reinterpret_cast<UI*>(lParam);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else
	{
		pThis = reinterpret_cast<UI*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pThis)
	{
		return pThis->DlgProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}

void UI::checkPeekMsg()
{

	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// If a dialog message will be taken care of by the dialog proc
		if (hWndApp && IsDialogMessageW(*hWndApp, &msg))
		{
			continue;
		}

		TranslateMessage(&msg);

		//process keyinput (probably bad practise to put it here)
		if (msg.message >= 0x00000100 && msg.message <= 0x00000102 && waitForKey == true)
		{
			processKeyInput(msg);

			waitForKey = false;
			SetDlgItemText(m_hWnd, IDC_ADD_KEY_INFO, L"Key input accepted");
			
		}

		DispatchMessageW(&msg);
	}

}

bool UI::checkQuitMsg()
{
	UINT message = msg.message;
	if ( msg.message != WM_QUIT)
	{
		return true;
	}

	return false;
}

void UI::SetCoordinateMapper(ICoordinateMapper * m_pCoordinateMapper)
{
	this->m_pCoordinateMapper = m_pCoordinateMapper;
}

bool UI::checkResource()
{
	if (m_hWnd)
	{
		//HRESULT hr = graphics.EnsureDirect2DResources();
		HRESULT hr = graphics.EnsureDirect2DResources(m_hWnd);
		if (SUCCEEDED(hr) && (graphics.GetRenderTarget()) && m_pCoordinateMapper) //!! this is changed too
		{
			return true;
		}
	}
	return false;
}

void UI::drawFrames(std::vector<Frame> & relframes, std::vector<Frame> & absframes)
{
	(graphics.GetRenderTarget())->BeginDraw();
	(graphics.GetRenderTarget())->Clear();

	GetClientRect(GetDlgItem(m_hWnd, IDC_VIDEOVIEW), &rct);
	width = rct.right;
	height = rct.bottom;

	std::vector<Frame> * frames = &absframes;

	

	if(drawAbsCoord)
	{
		drawUI();
	}
	else
	{
		frames = &relframes;
	}


	for (int j = frames->size()-1; j > -1; --j)
	{
		std::vector<Joint> joints = (*frames)[j].getJoints(); 
		std::vector<D2D1_POINT_2F> jointPoints(JointType_Count);
		
		for (int i = 0; i < joints.size(); ++i)
		{
			jointPoints[i] = graphics.BodyToScreen(joints[i].Position, width, height, m_pCoordinateMapper, cDepthWidth, cDepthHeight);
			
			if (j == 0)
			{
				activateHitboxes(jointPoints[i], static_cast<JointType>(i), (*frames)[j].getLeftHand(), (*frames)[j].getRightHand());
			}
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
		
		graphics.DrawBody(joints, jointPoints,j);

	}

	

	HRESULT hr = graphics.GetRenderTarget()->EndDraw();

	// Device lost, need to recreate the render target
	// We'll dispose it now and retry drawing
	if (D2DERR_RECREATE_TARGET == hr)
	{
		hr = S_OK;
		graphics.DiscardDirect2DResources();
	}

}


void UI::setMain(std::shared_ptr<Main>  m)
{
	main = m;
}


void UI::setModel(std::shared_ptr<Model> m)
{
	model = m;

	for (int i = 0; i < UI_Hitboxes.size(); i++)
	{
		UI_Hitboxes[i]->setModel(m);
	}
}

/// <summary>
/// Handle windows messages for the class instance
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK UI::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	
	int a = static_cast<int>(message);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		// Bind application window handle
		m_hWnd = hWnd;

		graphics.InitD2D();

		// Get and initialize the default Kinect sensor
		main->mainCanInitializeKinectSensor();
		
	}
	break;

	// If the titlebar X is clicked, destroy app
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		// Quit the main message pump
		PostQuitMessage(0);
		break;

	case WM_COMMAND:

		if (IDC_REL_ABS == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))	//Start to wait for key input
		{

			if (drawAbsCoord)
			{
				drawAbsCoord = false;
			}
			else
			{
				drawAbsCoord = true;
			}
			

		}
		if (IDC_ADD_KEY == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))	//Start to wait for key input
		{
	
				SetDlgItemText(m_hWnd, IDC_ADD_KEY_INFO, L"waiting for key input");
				waitForKey = true;

		}
		if (IDC_SELECT == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
		{
			BOOL sc;
			int label = GetDlgItemInt(m_hWnd, IDC_EDIT_LABEL, &sc, true);

			if (sc)
			{
				model->setActiveLabel(label);
				SetDlgItemInt(m_hWnd, IDC_ACTIVE_LABEL, label, TRUE);

			}

		}
		if (IDC_measure_4 == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))	//add the current gesture to the hover gestureclass
		{
				model->setRefresh(true);

		}

		if (IDC_START_PREDICT == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
		{
			model->setPredict(TRUE);
			
		}

		if (IDC_STOP_PREDICT == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
		{
			model->setPredict(FALSE);
			model->setTrained(FALSE);
		}
		break;
	}

	return FALSE;
}

void UI::processKeyInput(MSG msg)
{

	//test code to check how long a human presses a button
	if (msg.message == 0x00000100)
	{
		//DEBUG CODE
		/*
		int button = static_cast<int>(msg.wParam);
		wchar_t buffer[256];

		/* code for time measuring
		if (time != 0)
		{
			long diff = static_cast<long>(msg.time) - time;
		
			wchar_t buffer[256];
			wsprintfW(buffer, L"%ld", diff);
			OutputDebugStringW(L"auto release after ");
			OutputDebugStringW(buffer);
			wsprintfW(buffer, L"%d", button);
			OutputDebugStringW(L" milliseconds and pressed button ");
			OutputDebugStringW(buffer);
			OutputDebugStringW(L" \n");
		}
		

		time = static_cast<long>(msg.time);
		OutputDebugStringW(L" button pressed it was ");
		wsprintfW(buffer, L"%d", button);
		OutputDebugStringW(buffer);
		OutputDebugStringW(L" \n");
		*/

		int button = static_cast<int>(msg.wParam);
		wchar_t buffer[256];
		OutputDebugStringW(L" button pressed it was ");
		wsprintfW(buffer, L"%d", button);
		OutputDebugStringW(buffer);
		OutputDebugStringW(L" \n");

		//model->addActionToActive(msg.wParam, true); //standard holding for now
		model->addActionToActive(msg.wParam, false); //standard holding for now

	}
	
	if (msg.message == 0x00000102)
	{

		int button = static_cast<int>(msg.wParam);
		wchar_t buffer[256];
		OutputDebugStringW(L" button pressed it was ");
		wsprintfW(buffer, L"%d", button);
		OutputDebugStringW(buffer);
		OutputDebugStringW(L" \n");



	}
	
	if (msg.message == 0x00000101)
	{
		//DEBUG CODE
		/* 
		long diff = static_cast<long>(msg.time) - time;
		int button = static_cast<int>(msg.wParam);

		wchar_t buffer[256];
		wsprintfW(buffer, L"%ld", diff);
		OutputDebugStringW(L"time between press and release ");
		OutputDebugStringW(buffer);
		wsprintfW(buffer, L"%d", button);
		OutputDebugStringW(L" milliseconds it was button ");
		OutputDebugStringW(buffer);
		OutputDebugStringW(L" \n");
		time = 0;
		*/

	}
}

/*
/// <summary>
/// Show the joint coordinates on the screen
/// </summary>
/// <param name="skel">skeleton of which the joints will be shown</param>
void UI::ShowJointCoordinates(const std::vector<Joint> & joints, int tab)
{
	if (tab == 0)
	{

		//code for recalculating the coordinates of the joint to be relative to your center point (is done somewhere else)
	
		if (joints[9].Position.X*100.0 < -10000)
		{

		}

		if (joints[9].Position.X*100.0 > -10000 && joints[9].Position.Y*100.0 > -10000 && joints[9].Position.Z*100.0 > -10000 && joints[10].Position.X*100.0 > -10000 && joints[10].Position.Y*100.0 > -10000 && joints[10].Position.Z*100.0 > -10000 && joints[11].Position.X*100.0 > -10000 && joints[11].Position.Y*100.0 > -10000 && joints[11].Position.Z*100.0 > -10000) //sometimes empty jointarrays are given, this if will make sure these are not displayed
		{

			//right shoulder
			SetDlgItemInt(m_hWnd, IDC_JOINT1_X, (UINT32)(joints[9].Position.X*100.0), TRUE);
			SetDlgItemInt(m_hWnd, IDC_JOINT1_Y, (UINT32)((joints[9].Position.Y - 0.3)*100.0), TRUE);
			SetDlgItemInt(m_hWnd, IDC_JOINT1_Z, (UINT32)((joints[9].Position.Z - 1.8)*100.0), TRUE);

			//right elbow
			SetDlgItemInt(m_hWnd, IDC_JOINT2_X, (UINT32)(joints[10].Position.X*100.0), TRUE);
			SetDlgItemInt(m_hWnd, IDC_JOINT2_Y, (UINT32)((joints[10].Position.Y - 0.3)*100.0), TRUE);
			SetDlgItemInt(m_hWnd, IDC_JOINT2_Z, (UINT32)((joints[10].Position.Z - 1.8)*100.0), TRUE);

			//right wrist
			SetDlgItemInt(m_hWnd, IDC_JOINT3_X, (UINT32)(joints[11].Position.X*100.0), TRUE);
			SetDlgItemInt(m_hWnd, IDC_JOINT3_Y, (UINT32)((joints[11].Position.Y - 0.3)*100.0), TRUE);
			SetDlgItemInt(m_hWnd, IDC_JOINT3_Z, (UINT32)((joints[11].Position.Z - 1.8)*100.0), TRUE);

		}
	}
	else if (tab == 1)
	{
		//code for recalculating the coordinates of the joint to be relative to your center point (is done somewhere else)
	

		if (joints[9].Position.X*100.0 < -10000)
		{
		}

		if (joints[9].Position.X*100.0 > -10000 && joints[9].Position.Y*100.0 > -10000 && joints[9].Position.Z*100.0 > -10000 && joints[10].Position.X*100.0 > -10000 && joints[10].Position.Y*100.0 > -10000 && joints[10].Position.Z*100.0 > -10000 && joints[11].Position.X*100.0 > -10000 && joints[11].Position.Y*100.0 > -10000 && joints[11].Position.Z*100.0 > -10000) //sometimes empty jointarrays are given, this if will make sure these are not displayed
		{
			//right shoulder
			SetDlgItemInt(m_hWnd, IDC_JOINT1_X2, (UINT32)(joints[9].Position.X*100.0), TRUE);
			SetDlgItemInt(m_hWnd, IDC_JOINT1_Y2, (UINT32)((joints[9].Position.Y - 0.3)*100.0), TRUE);
			SetDlgItemInt(m_hWnd, IDC_JOINT1_Z2, (UINT32)((joints[9].Position.Z - 1.8)*100.0), TRUE);

			//right elbow
			SetDlgItemInt(m_hWnd, IDC_JOINT2_X2, (UINT32)(joints[10].Position.X*100.0), TRUE);
			SetDlgItemInt(m_hWnd, IDC_JOINT2_Y2, (UINT32)((joints[10].Position.Y - 0.3)*100.0), TRUE);
			SetDlgItemInt(m_hWnd, IDC_JOINT2_Z2, (UINT32)((joints[10].Position.Z - 1.8)*100.0), TRUE);

			//right wrist
			SetDlgItemInt(m_hWnd, IDC_JOINT3_X2, (UINT32)(joints[11].Position.X*100.0), TRUE);
			SetDlgItemInt(m_hWnd, IDC_JOINT3_Y2, (UINT32)((joints[11].Position.Y - 0.3)*100.0), TRUE);
			SetDlgItemInt(m_hWnd, IDC_JOINT3_Z2, (UINT32)((joints[11].Position.Z - 1.8)*100.0), TRUE);
		}
	}
}

*/

/// <summary>
/// Set the status bar message
/// </summary>
/// <param name="szMessage">message to display</param>
/// <param name="showTimeMsec">time in milliseconds to ignore future status messages</param>
/// <param name="bForce">force status update</param>
bool UI::SetStatusMessage(_In_z_ WCHAR* szMessage, DWORD nShowTimeMsec, bool bForce)
{
	INT64 now = GetTickCount64();

	if (m_hWnd && (bForce || (m_nNextStatusTime <= now)))
	{
		SetDlgItemText(m_hWnd, IDC_STATUS, szMessage);
		m_nNextStatusTime = now + nShowTimeMsec;

		return true;
	}

	return false;
}

void UI::setPredictedLabel(int label)
{
	SetDlgItemInt(m_hWnd, IDC_PREDICTION, label, true);
}

void UI::changeButtonColor(int state)
{
	graphics.changeButtonColor(state);
}

void UI::drawHandState(HandState handState, const D2D1_POINT_2F & handPosition)
{

}

void UI::drawUI()
{
	for (int i = 0; i < UI_Objects.size(); ++i)
	{
		if (UI_Objects[i]->getVisibele())
		{
			graphics.drawRectangle(UI_Objects[i]->getCenter(), UI_Objects[i]->getWidth(), UI_Objects[i]->getHeight(), UI_Objects[i]->getColor());
		}
		
	}
}

void UI::activateHitboxes(D2D1_POINT_2F jointPoint, JointType type, HandState leftHand, HandState rightHand)
{
	//debug code to show the coordinates of each had in screen coordinates
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
		if (UI_Hitboxes[i]->checkInputJointType(type) && !(UI_Hitboxes[i]->getTotalLock()))
		{
			if (UI_Hitboxes[i]->checkCoordInside(jointPoint))
			{
				//if (!(UI_Hitboxes[i].getHover()))
				//{
				//NOW only works for right and left hand !
				if (type == JointType_HandRight && !rightHandBusy)
				{
					UI_Hitboxes[i]->setHover(true,type, rightHand, jointPoint);
					rightHandBusy = UI_Hitboxes[i]->isRightHandActive();
					

				}
				else if(type == JointType_HandLeft && !leftHandBusy)
				{
					UI_Hitboxes[i]->setHover(true, type, leftHand, jointPoint);
					leftHandBusy = UI_Hitboxes[i]->isLeftHandActive();
				}
					
					//UI_Hitboxes[i].setHoverJoint(type);
					//printf("activate");
				//}
				
			}
			else// if(UI_Hitboxes[i]->getHover())
			{
				//if (UI_Hitboxes[i].getHover() && UI_Hitboxes[i].getHoverJoint() == type)
				//{
					//UI_Hitboxes[i]->setHover(false,type,leftHand, jointPoint);
					if (type == JointType_HandRight)
					{
						UI_Hitboxes[i]->setHover(false, type, rightHand, jointPoint);
						rightHandBusy = UI_Hitboxes[i]->isRightHandActive();
					}
					else
					{
						UI_Hitboxes[i]->setHover(false, type, leftHand, jointPoint);
						leftHandBusy = UI_Hitboxes[i]->isLeftHandActive();
					}
					//printf("DEactivate!!!");
				//}
			}
			
		}
		
	}
}




