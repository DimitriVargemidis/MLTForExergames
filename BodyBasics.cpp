//------------------------------------------------------------------------------
// <copyright file="BodyBasics.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <strsafe.h>
#include "D2D_Graphics.h"
#include "resource.h"
#include "BodyBasics.h"
//#include "svm_interface.h"
#include "Model.h"

//endInterface
/// <summary>
/// Entry point for the application
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="hPrevInstance">always 0</param>
/// <param name="lpCmdLine">command line arguments</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
/// <returns>status</returns>
int APIENTRY wWinMain(    
	_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    CBodyBasics application;
    application.Run(hInstance, nShowCmd);
}

/// <summary>
/// Constructor
/// </summary>
CBodyBasics::CBodyBasics() :
	m_hWnd(NULL),
	m_nStartTime(0),
	m_nLastCounter(0),
	m_nFramesSinceUpdate(0),
	m_fFreq(0),
	m_nNextStatusTime(0LL),
	m_pKinectSensor(NULL),
	m_pCoordinateMapper(NULL),
	m_pBodyFrameReader(NULL),
	graphics{}

	
{
	//graphics = D2D_Graphics();
    LARGE_INTEGER qpf = {0};
    if (QueryPerformanceFrequency(&qpf))
    {
        m_fFreq = double(qpf.QuadPart);
    }

	//SELFMADE parameters that are meant to stay
	//model = new Model;


	//initialising SELFMADE PARAMETERS
	savedJointPoints = new D2D1_POINT_2F [25];
	savedJoints = new Joint[25];

	savedJointPoints_2 = new D2D1_POINT_2F[25];
	savedJoints_2 = new Joint[25];
	
}
  

/// <summary>
/// Destructor
/// </summary>
CBodyBasics::~CBodyBasics()
{

	// clean up Direct2D
	graphics.CleanD2D();
	
    // done with body frame reader
    SafeRelease(m_pBodyFrameReader);

    // done with coordinate mapper
    SafeRelease(m_pCoordinateMapper);

    // close the Kinect Sensor
    if (m_pKinectSensor)
    {
        m_pKinectSensor->Close();
    }

    SafeRelease(m_pKinectSensor);

	
}

//moved to UI

//UI
/*
/// <summary>
/// Creates the main window and begins processing
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
int CBodyBasics::Run(HINSTANCE hInstance, int nCmdShow)
{
    MSG       msg = {0};
    WNDCLASS  wc;

    // Dialog custom window class
    ZeroMemory(&wc, sizeof(wc));
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.cbWndExtra    = DLGWINDOWEXTRA;
    wc.hCursor       = LoadCursorW(NULL, IDC_ARROW);
    wc.hIcon         = LoadIconW(hInstance, MAKEINTRESOURCE(IDI_APP));
    wc.lpfnWndProc   = DefDlgProcW;
    wc.lpszClassName = L"BodyBasicsAppDlgWndClass";

    if (!RegisterClassW(&wc))
    {
        return 0;
    }

    // Create main application window
    HWND hWndApp = CreateDialogParamW(
        NULL,
        MAKEINTRESOURCE(IDD_APP),
        NULL,
        (DLGPROC)CBodyBasics::MessageRouter, 
        reinterpret_cast<LPARAM>(this));

    // Show window
    ShowWindow(hWndApp, nCmdShow);

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
}
*/
//END MOVE TO UI

//endinterface
/// <summary>
/// Main processing function
/// </summary>
void CBodyBasics::Update()
{
    if (!m_pBodyFrameReader)
    {
        return;
    }

    IBodyFrame* pBodyFrame = NULL;

    HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

    if (SUCCEEDED(hr))
    {
        INT64 nTime = 0;

        hr = pBodyFrame->get_RelativeTime(&nTime);

        IBody* ppBodies[BODY_COUNT] = {0};

        if (SUCCEEDED(hr))
        {
            hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
        }

        if (SUCCEEDED(hr))
        {
            ProcessBody(nTime, BODY_COUNT, ppBodies);
        }

        for (int i = 0; i < _countof(ppBodies); ++i)
        {
            SafeRelease(ppBodies[i]);
        }
    }

    SafeRelease(pBodyFrame);
}

//MOVED TO UI
/*
/// <summary>
/// Handles window messages, passes most to the class instance to handle
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK CBodyBasics::MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CBodyBasics* pThis = NULL;
    
    if (WM_INITDIALOG == uMsg)
    {
        pThis = reinterpret_cast<CBodyBasics*>(lParam);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else
    {
        pThis = reinterpret_cast<CBodyBasics*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis)
    {
        return pThis->DlgProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}



//UI
/// <summary>
/// Handle windows messages for the class instance
/// </summary>
/// <param name="hWnd">window message is for</param>
/// <param name="uMsg">message</param>
/// <param name="wParam">message data</param>
/// <param name="lParam">additional message data</param>
/// <returns>result of message processing</returns>
LRESULT CALLBACK CBodyBasics::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
            InitializeDefaultSensor();
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
				refresh = TRUE;
			}

			if (IDC_START_PREDICT == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
			{
				//put here the train function
				//train(6, 9, SVMInputData, SVMLabels);
				predict = true;
			}

			if (IDC_measure_RESET == LOWORD(wParam) && BN_CLICKED == HIWORD(wParam))
			{
				SVMInputDataIndex = 0;
				SVMLabelsIndex = 0;
				//predict = false;


			}

			break;
    }

    return FALSE;
}

*/
//END MOVED TO UI


//Main
/// <summary>
/// Initializes the default Kinect sensor
/// </summary>
/// <returns>indicates success or failure</returns>
HRESULT CBodyBasics::InitializeDefaultSensor()
{
    HRESULT hr;

    hr = GetDefaultKinectSensor(&m_pKinectSensor);
    if (FAILED(hr))
    {
        return hr;
    }

    if (m_pKinectSensor)
    {
        // Initialize the Kinect and get coordinate mapper and the body reader
        IBodyFrameSource* pBodyFrameSource = NULL;

        hr = m_pKinectSensor->Open();

        if (SUCCEEDED(hr))
        {
            hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
        }

        if (SUCCEEDED(hr))
        {
            hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
        }

        if (SUCCEEDED(hr))
        {
            hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
        }

        SafeRelease(pBodyFrameSource);
    }

    if (!m_pKinectSensor || FAILED(hr))
    {
        SetStatusMessage(L"No ready Kinect found!", 10000, true);
        return E_FAIL;
    }

    return hr;
}

//model??
/// <summary>
/// Handle new body data
/// <param name="nTime">timestamp of frame</param>
/// <param name="nBodyCount">body data count</param>
/// <param name="ppBodies">body data in frame</param>
/// </summary>
void CBodyBasics::ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies)
{
    if (m_hWnd)
    {
        //HRESULT hr = graphics.EnsureDirect2DResources();
		HRESULT hr = graphics.EnsureDirect2DResources(m_hWnd);
        if (SUCCEEDED(hr) && (graphics.GetRenderTarget()) && m_pCoordinateMapper) //!! this is changed too
        {
			/*
            m_pRenderTarget->BeginDraw();
            m_pRenderTarget->Clear();
			*/

			//SELFMADE MEANT TO STAY
			(graphics.GetRenderTarget())->BeginDraw();
			(graphics.GetRenderTarget())->Clear();


            RECT rct;
            GetClientRect(GetDlgItem(m_hWnd, IDC_VIDEOVIEW), &rct);
            int width = rct.right;
            int height = rct.bottom;

            for (int i = 0; i < nBodyCount; ++i)
            {
                IBody* pBody = ppBodies[i];
                if (pBody)
                {
                    BOOLEAN bTracked = false;
                    hr = pBody->get_IsTracked(&bTracked);

                    if (SUCCEEDED(hr) && bTracked)
                    {
                        Joint joints[JointType_Count]; 
						Joint transformedJoints[JointType_Count];
                        D2D1_POINT_2F jointPoints[JointType_Count];
                        HandState leftHandState = HandState_Unknown;
                        HandState rightHandState = HandState_Unknown;

                        pBody->get_HandLeftState(&leftHandState);
                        pBody->get_HandRightState(&rightHandState);

                        hr = pBody->GetJoints(_countof(joints), joints);
                        if (SUCCEEDED(hr))
                        {
                            for (int j = 0; j < _countof(joints); ++j)
                            {
								//SELFMADE code to convert to coordinates relative to the spine
								CameraSpacePoint still = joints[j].Position;

								if (j == 1)
								{
									still.X = 0.0;
									still.Y = 0.3;
									still.Z = 1.8;

								}
								else
								{
									still.X = joints[j].Position.X - joints[1].Position.X;//+ 0.05;
									still.Y = joints[j].Position.Y - joints[1].Position.Y + 0.3;
									still.Z = joints[j].Position.Z - joints[1].Position.Z + 1.8;
								}
								transformedJoints[j] = joints[j];
								transformedJoints[j].Position = still;
								jointPoints[j] = graphics.BodyToScreen(still, width, height, m_pCoordinateMapper, cDepthWidth, cDepthHeight);
								ShowJointCoordinates(transformedJoints, 0);
								
                                //jointPoints[j] = BodyToScreen(joints[j].Position, width, height);
                            }

							if ((refresh || start_1 || start_2) && !predict)
							{
									BOOL sc;
									int label = GetDlgItemInt(m_hWnd, IDC_EDIT_LABEL,&sc,true);

								
								for (int j = 0; j < _countof(joints); ++j)
								{
							
									if (sc || start_1 || start_2)
									{
										SetDlgItemText(m_hWnd, IDC_measure_4, (LPCWSTR)L"Measure");

										if (label == 1 || start_1)
										{
											savedJointPoints[j] = jointPoints[j];
											savedJoints[j] = transformedJoints[j];				
										}
										else if (label == 2 || start_2)
										{
											savedJointPoints_2[j] = jointPoints[j];
											savedJoints_2[j] = transformedJoints[j];
										}
										else
										{
											SetDlgItemText(m_hWnd, IDC_measure_4, (LPCWSTR)L"label not 1 or 2");
										}

										
									}
									else
									{
										SetDlgItemText(m_hWnd, IDC_measure_4, (LPCWSTR) L"label incorrect");
									}
								}
									
								if (!start_1 && !start_2)
								{
									AddDataToSVMInputData(transformedJoints, label);
				
								}

								ShowJointCoordinates(transformedJoints, 1);
								start_1 = false;
								start_2 = false;
								refresh = false;	
								SetDlgItemInt(m_hWnd, IDC_PREDICTION, 8, true);
							}

							if (predict)
							{
								int pred = MakePrediction(transformedJoints);
								SetDlgItemInt(m_hWnd, IDC_PREDICTION,pred, true);
							}
							//END SELFMADE

							//DrawBody(joints, jointPoints);

							graphics.DrawBody(transformedJoints, jointPoints); //SELFMADE
							
						    //SELFMADE CODE
							graphics.DrawBody(savedJoints, savedJointPoints);
							graphics.DrawBody(savedJoints_2, savedJointPoints_2);
							//END

							graphics.DrawHand(leftHandState, jointPoints[JointType_HandLeft]);
							graphics.DrawHand(rightHandState, jointPoints[JointType_HandRight]);
                        }
                    }
                }
            }

            //hr = m_pRenderTarget->EndDraw();

			//SELFMADE MEANT TO STAY
			graphics.GetRenderTarget()->EndDraw();

            // Device lost, need to recreate the render target
            // We'll dispose it now and retry drawing
            if (D2DERR_RECREATE_TARGET == hr)
            {
                hr = S_OK;
				graphics.DiscardDirect2DResources();
            }
        }

        if (!m_nStartTime)
        {
            m_nStartTime = nTime;
        }

        double fps = 0.0;

        LARGE_INTEGER qpcNow = {0};
        if (m_fFreq)
        {
            if (QueryPerformanceCounter(&qpcNow))
            {
                if (m_nLastCounter)
                {
                    m_nFramesSinceUpdate++;
                    fps = m_fFreq * m_nFramesSinceUpdate / double(qpcNow.QuadPart - m_nLastCounter);
                }
            }
        }

        WCHAR szStatusMessage[64];
        StringCchPrintf(szStatusMessage, _countof(szStatusMessage), L" FPS = %0.2f    Time = %I64d", fps, (nTime - m_nStartTime));

        if (SetStatusMessage(szStatusMessage, 1000, false))
        {
            m_nLastCounter = qpcNow.QuadPart;
            m_nFramesSinceUpdate = 0;
        }
    }
}

//UI
/// <summary>
/// Set the status bar message
/// </summary>
/// <param name="szMessage">message to display</param>
/// <param name="showTimeMsec">time in milliseconds to ignore future status messages</param>
/// <param name="bForce">force status update</param>
bool CBodyBasics::SetStatusMessage(_In_z_ WCHAR* szMessage, DWORD nShowTimeMsec, bool bForce)
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


//SELFMADE FUNCTIONS

//UI
/// <summary>
/// Show the joint coordinates on the screen
/// </summary>
/// <param name="skel">skeleton of which the joints will be shown</param>
void CBodyBasics::ShowJointCoordinates(const Joint * joints, int tab)
{
	if (tab == 0)
	{
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
		Joint still, armJoints[3];

		for (int i = 0; i < 3; i++)
		{
			still.Position.X = joints[8 + i].Position.X - joints[1].Position.X;
			still.Position.Y = joints[8 + i].Position.Y - joints[1].Position.Y;
			still.Position.Z = joints[8 + i].Position.Z - joints[1].Position.Z;
			armJoints[i] = still;
		}

		//right shoulder
		SetDlgItemInt(m_hWnd, IDC_JOINT1_X2, (UINT32)(joints[9].Position.X*100.0), TRUE);
		SetDlgItemInt(m_hWnd, IDC_JOINT1_Y2, (UINT32)((joints[9].Position.Y - 0.3)*100.0 ), TRUE);
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

//SVMinterface
void CBodyBasics::AddDataToSVMInputData(const Joint * joints, int label)
{
	if (SVMLabelsIndex < 6)
	{
		SVMLabels[SVMLabelsIndex] = label;
		SVMLabelsIndex++;
		SetDlgItemInt(m_hWnd, IDC_MEAS_COUNT, SVMLabelsIndex, true);

		for (int i = 0; i < 3; i++)
		{
			SVMInputData[SVMInputDataIndex + 0] = joints[9 + i].Position.X;
			SVMInputData[SVMInputDataIndex + 1] = joints[9 + i].Position.Y;
			SVMInputData[SVMInputDataIndex + 2] = joints[9 + i].Position.Z;
			SVMInputDataIndex = SVMInputDataIndex + 3;
		}
	}

	if (SVMInputDataIndex == 54)
	{
		SetDlgItemText(m_hWnd, IDC_PREDICT_READY, (LPCWSTR)L"READY");
		//predict = true;
	}

}


//model
//needs to be changed! transformation happens already in SVMinterface, this needs to make a Frame object
int CBodyBasics::MakePrediction(const Joint * joints)
{
	
	double	SVMpredictInput[9];
	int		SVMpredictInputIndex = int{ 0 };
	int		result = int{ 0 };
	

	for (int i = 0; i < 3; i++)
	{
		SVMpredictInput[SVMpredictInputIndex + 0] = joints[9 + i].Position.X;
		SVMpredictInput[SVMpredictInputIndex + 1] = joints[9 + i].Position.Y;
		SVMpredictInput[SVMpredictInputIndex + 2] = joints[9 + i].Position.Z;
		SVMpredictInputIndex = SVMpredictInputIndex + 3;
	}

	//result = test(9,SVMpredictInput);

	return result; //put the test method from SVM here
}

//END
