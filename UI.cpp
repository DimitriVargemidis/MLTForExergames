#include "stdafx.h"
#include <strsafe.h>
#include "D2D_Graphics.h"
#include "resource.h"
#include "Main.h"

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

UI::UI(Main * main) :
m_pCoordinateMapper(NULL),
m_hWnd(NULL),
graphics{}
{

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
		hWndApp = CreateDialogParamW(
		NULL,
		MAKEINTRESOURCE(IDD_APP),
		NULL,
		(DLGPROC)UI::MessageRouter,
		reinterpret_cast<LPARAM>(this));

	// Show window
	ShowWindow(hWndApp, nCmdShow);

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
	MSG       msg = { 0 };

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

bool UI::checkQuitMsg()
{
	if (WM_QUIT == msg.message)
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

void UI::drawFrames(std::vector<Frame> frames)
{
	(graphics.GetRenderTarget())->BeginDraw();
	(graphics.GetRenderTarget())->Clear();

	RECT rct;
	GetClientRect(GetDlgItem(m_hWnd, IDC_VIDEOVIEW), &rct);
	int width = rct.right;
	int height = rct.bottom;

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

	case WM_COMMAND:

		if (IDC_measure_4 == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
		{
			//main->setModelPredict(TRUE);
		}

		if (IDC_START_PREDICT == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
		{
			//put here the train function
			//train(6, 9, SVMInputData, SVMLabels);
			main->setModelPredict(TRUE);
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

