//author: Christiaan Vanbergen 

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
m_pCoordinateMapper(NULL)
{

	
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
		if (IDC_CANC_ADD_KEY2 == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))	//Cancel the wait for key input
		{

			waitForKey = false;
			SetDlgItemText(m_hWnd, IDC_ADD_KEY_INFO, L"Idle");

		}
		if (IDC_SINGLE_CONT == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))	//Toggle the mode key that is added
		{
			if (holdPress)
			{
				holdPress = false;
				SetDlgItemText(m_hWnd, IDC_SINGLE_CONT_INFO, L"SINGLE PRESS");
			}
			else
			{
				holdPress = true;
				SetDlgItemText(m_hWnd, IDC_SINGLE_CONT_INFO, L"HOLD PRESS");
			}
			

		}

		if (IDC_SELECT == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
		{
			BOOL sc;
			int label = GetDlgItemInt(m_hWnd, IDC_EDIT_LABEL, &sc, true);

			if (sc)
			{
				
				model->setActiveLabel(label*10);
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
			Screen->setDisplay_UI(false);
			
		}

		if (IDC_STOP_PREDICT == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
		{
			model->setPredict(FALSE);
			model->setTrained(FALSE);
			Screen->setDisplay_UI(true);
		}
		break;
	}

	return FALSE;
}

void UI::processKeyInput(MSG msg)
{

	if (msg.message == 0x00000100)
	{

		int button = static_cast<int>(msg.wParam);
		wchar_t buffer[256];
		OutputDebugStringW(L" button pressed it was ");
		wsprintfW(buffer, L"%d", button);
		OutputDebugStringW(buffer);
		OutputDebugStringW(L" \n");

		model->addActionToActive(msg.wParam, holdPress); 

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
		
}




