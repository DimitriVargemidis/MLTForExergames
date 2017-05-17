#include <strsafe.h>
#include <Windows.h>
#include <Winuser.h>
#include <memory>
#include <string>

#include "stdafx.h"
#include "D2D_Graphics.h"
#include "resource.h"
#include "Main.h"
#include "Frame.h"
#include "Gesture.h"

#include "UI.h"

D2D_Graphics			graphics;
HWND                    m_hWnd;

UI::UI() :
m_pCoordinateMapper(NULL)//,
//m_hWnd(NULL)
//graphics {}
{
	//const static std::shared_ptr<D2D_Graphics> graphicStaticPtr(&graphics);
	//const static std::shared_ptr<UI> UI_static_ptr(this);
	
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

	Screen->drawFrames(relframes, absframes);
	/*
	std::vector<Frame> * frames = &absframes;

	drawUI();

	if(!drawAbsCoord)
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
			
			

			if (j == 0 && drawAbsCoord)
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
			
		}


		if(!drawAbsCoord)
		{
			scaleSkeleton(jointPoints, 0.8);
		}

		graphics.DrawBody(joints, jointPoints,j);

	}
*/
	

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
	Screen->setModel(model);
	/*
	

	for (int i = 0; i < UI_Hitboxes.size(); i++)
	{
		UI_Hitboxes[i]->setModel(m);
	}
	updateHitboxes();
	*/
}

void UI::createScreen()
{
	GetClientRect(GetDlgItem(m_hWnd, IDC_VIDEOVIEW), &rct);
	width = rct.right;
	height = rct.bottom;

	graphics.EnsureDirect2DResources(m_hWnd);

	Screen = std::make_shared<recordScreen>(1);
	std::shared_ptr<UI> shared_ptr_this(this);

	Screen->setUI(shared_ptr_this, width, height, m_pCoordinateMapper,cDepthWidth, cDepthHeight);
	Screen->createScreen(width, height);
}

void UI::scaleSkeleton(std::vector<D2D1_POINT_2F>& jointPoints, float multiplier)
{
	/*
	D2D1_POINT_2F center;
	center.x = jointPoints[JointType_SpineMid].x;
	center.y = jointPoints[JointType_SpineMid].y;

	for (int j = 0; j < jointPoints.size(); ++j)
	{
		//Convert to coordinates relative to the spine
		if(j != JointType_SpineMid)
		{
			jointPoints[j].x = center.x + (jointPoints[j].x - center.x)*multiplier;
			jointPoints[j].y = center.y + (jointPoints[j].y - center.y)*multiplier;
		}
	}
	*/
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
		//Screen->initBitmaps();

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

			if (Screen->getDrawAbsCoord())
			{
				Screen->setDrawAbsCoord( false);
			}
			else
			{
				Screen->setDrawAbsCoord(true);
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
				Screen->setGestureClassID(label);
				model->setUpdatUI(true);
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



/*
void UI::fillScrolbar(int ID)
{
	std::shared_ptr<UI> shared_ptr_this(this);
	std::shared_ptr<GestureClass> ActiveGestureClass = model->getGestureClassByID(ID);
	std::vector<Gesture> gestures = ActiveGestureClass->getGestures();

		std::shared_ptr<UI_Object> background = std::make_shared<UI_Object>(550 + 250, 350, 150, 500, D2D1::ColorF::Red);
		//UI_Objects.push_back(testObject5);
		std::shared_ptr<UI_Object> selectedBox = std::make_shared<UI_Object>(550 + 250, 250, 150, 100, D2D1::ColorF::Blue);
		//UI_Objects.push_back(selectedBox);
		
		//scrollbar->add_UI_Object(background);
		//scrollbar->add_UI_Object(selectedBox);
		


		int gestureID;

	//for (int i = gestures.size()-1; i >= 0; i--)
	for (int i =0 ; i < gestures.size(); i++)
	{
		gestureID = gestures[i].getGestureID();


		std::shared_ptr<UI_Object> testObject7 = std::make_shared<UI_Object>(400 + 250, 350, 150, 150, D2D1::ColorF::White);
		std::shared_ptr<UI_Object> text1 = std::make_shared<UI_TextObject>(400 + 250 - 30, 350, 75, 150, D2D1::ColorF::Black, std::to_wstring(i+1), 20);
		//std::shared_ptr<UI_Object> text = std::make_shared<UI_TextObject>(400 + 250+30, 350, 75, 150, D2D1::ColorF::Black,(std::to_wstring(gestureID)),20);
		std::shared_ptr<Abstr_UI_Hitbox> testHitbox7(new UI_HitboxHoverSlideButton(400 + 250, 350, 150, 150, 0, 100, 0, 0, 0.5, UI_CallbackFunctions::deleteGesture, gestures[i].getGestureID()));

		testHitbox7->add_UI_Object(testObject7);
		//testHitbox7->add_UI_Object(text);
		testHitbox7->add_UI_Object(text1);
		testHitbox7->addInputJoint(JointType_HandLeft);
		testHitbox7->addInputJoint(JointType_HandRight);
		testHitbox7->setUI(shared_ptr_this);

		//scrollbar->add_UI_Element(testHitbox7);
	}

	//UI_Hitboxes.push_back(scrollbar);

}
*/

void UI::setRightHandBusy(const bool & busy)
{
	Screen->setRightHandBusy(busy);
}

const bool & UI::getRightHandBusy()
{
	return Screen->getRightHandBusy();
}

void UI::setLeftHandBusy(const bool & busy)
{
	Screen->setLeftHandBusy(busy);
}

const bool & UI::getLeftHandBusy()
{
	return Screen->getLeftHandBusy();
}

void UI::setScreen(const std::shared_ptr<Abstr_Screen>& scr)
{
	Screen = scr;
}

const std::shared_ptr<Abstr_Screen>& UI::getScreen()
{
	return Screen;
}

void UI::changeButtonColor(int state)
{
	graphics.changeButtonColor(state);
}

void UI::drawHandState(HandState handState, const D2D1_POINT_2F & handPosition)
{

}

void UI::updateHitboxes()
{
	Screen->updateHitboxes();
	/*
	for (const auto & hitbox : UI_Hitboxes)
	{
		hitbox->updateData();
	}
	*/
		
}




