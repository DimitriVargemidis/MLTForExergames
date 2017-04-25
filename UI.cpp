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
}


UI::~UI()
{
	// clean up Direct2D
	graphics.CleanD2D();


	// done with coordinate mapper
	SafeRelease(m_pCoordinateMapper);
}

/*
UI::UI(Main * main) :
m_pCoordinateMapper(NULL),
m_hWnd(NULL),
graphics{}
{

}
*/

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
	//MOVED TO MAIN + other functions in UI
	/*
	// Main message loop
	while (WM_QUIT != msg.message)
	{
		Update();

		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// If a dialog message will be taken care of by the dialog proc
			if (hWndApp && IsDialogMessageW(hWndApp, &msg))
			{
				continue;
			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
	


	return static_cast<int>(msg.wParam);
	*/
	//END MOVED TO MAIN + other functions in UI
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
	//MSG       msg = { 0 };

	//check keyboard inputs 

	/*
	PBYTE keyboardstate = static_cast<PBYTE>(HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,sizeof(PBYTE)*256));
	GetKeyboardState(keyboardstate);

	for (int i = 0; i < 256; i++)
	{
		if (keyboardstate[i] != 0)
		{
			wchar_t buffer[256];
			wsprintfW(buffer, L"%d", i);
			OutputDebugStringW(L"button ");
			OutputDebugStringW(buffer);
			OutputDebugStringW(L" is active \n");
		}

	}

	HeapFree(GetProcessHeap(), 0, keyboardstate);
	*/
	

	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// If a dialog message will be taken care of by the dialog proc
		if (hWndApp && IsDialogMessageW(*hWndApp, &msg))
		{
			continue;
		}

		TranslateMessage(&msg);

		//process keyinput (probably bad practise to put it here)
		if (msg.message >= 0x00000100 && msg.message <= 0x00000102)
		{
			processKeyInput(msg);
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

void UI::drawFrames(std::vector<Frame> & frames )
{
	(graphics.GetRenderTarget())->BeginDraw();
	(graphics.GetRenderTarget())->Clear();

	RECT rct;
	GetClientRect(GetDlgItem(m_hWnd, IDC_VIDEOVIEW), &rct);
	int width = rct.right;
	int height = rct.bottom;

	//printf(frames.size());

	for (int j = frames.size()-1; j > -1; --j)
	{
		std::vector<Joint> joints = frames[j].getJoints(); 
		D2D1_POINT_2F jointPoints[JointType_Count];
		Joint jointArray[JointType_Count];

		int jointCount = JointType_Count;
		int jointAmount = joints.size();

		//for (int i = 0; i < frames.size(); ++i)
		for (int i = 0; i < joints.size(); ++i)
		{
			jointArray[i] = joints[i];
			jointPoints[i] = graphics.BodyToScreen(joints[i].Position, width, height, m_pCoordinateMapper, cDepthWidth, cDepthHeight);
			if (j == 1)
			{
				ShowJointCoordinates(joints, 0);
			}
			else
			{
				ShowJointCoordinates(joints, 1);
			}
		}

		
		graphics.DrawBody(jointArray, jointPoints,j);

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
	
	//some test code
	/*
	if (a != 312 && a != 127 && a != 512 && a != 15 
	    && a != 134 && a != 6 && a != 28 && a != 20
		&& a != 310 && a != 78 && a != 792 && a != 309)
	{

		wchar_t buffer[256];
		wsprintfW(buffer, L"%d", a);
		OutputDebugStringW(L"message ");
		OutputDebugStringW(buffer);
		OutputDebugStringW(L"\n");
	}
	*/


	switch (message)
	{
	case WM_INITDIALOG:
	{
		// Bind application window handle
		m_hWnd = hWnd;

		graphics.InitD2D();

		// Get and initialize the default Kinect sensor
		//InitializeDefaultSensor();
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
	case WM_CHAR:
		//wchar_t buffer[256];
		//wsprintfW(buffer, L"%d", i);
		//OutputDebugStringW(L"button ");
		//OutputDebugStringW(buffer);
		OutputDebugStringW(L" key pressed\n");
		break;

	case WM_COMMAND:

		if (IDC_measure_4 == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
		{
			//main->setModelPredict(TRUE);
			BOOL sc;
			int label = GetDlgItemInt(m_hWnd, IDC_EDIT_LABEL, &sc, true);
			
			if (sc)
			{
				model->setActiveLabel(label);
				model->setRefresh(true);
			}

		}

		if (IDC_START_PREDICT == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
		{
			//put here the train function
			//train(6, 9, SVMInputData, SVMLabels);
			model->setPredict(TRUE);
			
		}

		if (IDC_STOP_PREDICT == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
		{
			//put here the train function
			//train(6, 9, SVMInputData, SVMLabels);
			model->setPredict(FALSE);
			model->setTrained(FALSE);
		}

		if (IDC_measure_RESET == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
		{
			//SVMInputDataIndex = 0;
			//SVMLabelsIndex = 0;
			//predict = false;
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
		int button = static_cast<int>(msg.wParam);
		wchar_t buffer[256];


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



	}
	/*
	if (msg.message == 0x00000102)
	{
		int lparam = msg.lParam;
		int wparam = msg.wParam;

		wchar_t buffer[256];
		wsprintfW(buffer, L"%d", lparam);
		OutputDebugStringW(L"time between press and release ");
		OutputDebugStringW(buffer);
		OutputDebugStringW(L" milliseconds \n");
		time = 0;


	}
	*/
	if (msg.message == 0x00000101)
	{
		
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
		

	}
}

/// <summary>
/// Show the joint coordinates on the screen
/// </summary>
/// <param name="skel">skeleton of which the joints will be shown</param>
void UI::ShowJointCoordinates(const std::vector<Joint> & joints, int tab)
{
	if (tab == 0)
	{

		//code for recalculating the coordinates of the joint to be relative to your center point (is done somewhere else)
		/*Joint still, armJoints[3];

		for (int i = 0; i < 3; i++)
		{
		still.Position.X = joints[8 + i].Position.X - joints[1].Position.X;
		still.Position.Y = joints[8 + i].Position.Y - joints[1].Position.Y;
		still.Position.Z = joints[8 + i].Position.Z - joints[1].Position.Z;
		armJoints[i] = still;
		}*/
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
		/*
		Joint still, armJoints[3];

		for (int i = 0; i < 3; i++)
		{
			still.Position.X = joints[8 + i].Position.X - joints[1].Position.X;
			still.Position.Y = joints[8 + i].Position.Y - joints[1].Position.Y;
			still.Position.Z = joints[8 + i].Position.Z - joints[1].Position.Z;
			armJoints[i] = still;
		}
		*/
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
