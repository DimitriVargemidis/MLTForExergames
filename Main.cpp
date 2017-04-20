
#include <iostream>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#include "stdafx.h"
#include <strsafe.h>
#include "resource.h"
#include "Model.h"
#include "Frame.h"
#include "UI.h"

#include "Main.h"

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

	Main application;
	application.Run(hInstance, nShowCmd);
}


Main::Main() :
	m_pKinectSensor(NULL),
	m_pBodyFrameReader(NULL)
{
	
	running = true;
}


Main::~Main()
{
	// done with body frame reader
	SafeRelease(m_pBodyFrameReader);

	

	// close the Kinect Sensor
	if (m_pKinectSensor)
	{
		m_pKinectSensor->Close();
	}

	SafeRelease(m_pKinectSensor);

}

void Main::mainCanInitializeKinectSensor()
{
	Main::InitializeDefaultSensor();
}


/// <summary>
/// Initializes the default Kinect sensor
/// </summary>
/// <returns>indicates success or failure</returns>
HRESULT Main::InitializeDefaultSensor()
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
			ICoordinateMapper*      m_pCoordinateMapper;
			hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
			ui->SetCoordinateMapper(m_pCoordinateMapper);
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
		//SetStatusMessage(L"No ready Kinect found!", 10000, true);
		return E_FAIL;
	}

	return hr;
}

/// <summary>
/// Main processing function
/// </summary>
void Main::Update()
{
	if (!m_pBodyFrameReader)
	{
		return;
	}

	IBodyFrame* pBodyFrame = NULL;


	IBodyFrameSource* pBodyFrameSource = NULL;
	int bodies = -1;
	m_pBodyFrameReader->get_BodyFrameSource(&pBodyFrameSource);
	pBodyFrameSource->get_BodyCount(&bodies);


	HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);


	if (SUCCEEDED(hr))
	{
		INT64 nTime = 0;

		hr = pBodyFrame->get_RelativeTime(&nTime);

		IBody* ppBodies[BODY_COUNT] = { 0 };

		if (SUCCEEDED(hr))
		{
			hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
		}

		if (SUCCEEDED(hr))
		{
			model->ProcessBody(nTime, BODY_COUNT, ppBodies);

		}

		for (int i = 0; i < _countof(ppBodies); ++i)
		{
			SafeRelease(ppBodies[i]);
		}
	}

	SafeRelease(pBodyFrame);
}

int Main::Run(HINSTANCE hInstance, int nCmdShow)
{
	ui = std::make_shared<UI>();
	model = std::make_shared<Model>();

	std::shared_ptr<Main> shared_ptr_this(this);

	ui->setMain(shared_ptr_this);
	ui->setModel(model);

	model->setView(ui);

	int rc = ui->Run(hInstance, nCmdShow);

	//test clocking code 
	auto t1 = Clock::now();
	Sleep(30);
	auto t2 = Clock::now();

	long time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	wchar_t buffer[256];
	wsprintfW(buffer, L"%ld", time );
	OutputDebugStringW(L"time between t1 en t2 ");
	OutputDebugStringW(buffer);
	OutputDebugStringW(L" milliseconds \n");
	//end test

	while (ui->checkQuitMsg())
	{

		Update();
		ui->checkPeekMsg();
	}

	return 0;
}

void Main::drawFrames(std::vector<Frame> frames)
{
	if (ui->checkResource())
	{
		ui->drawFrames(frames);
	}
}

void Main::setModelRefresh(bool refresh)
{
	model->setRefresh(refresh);
}

bool Main::getModelRefresh()
{
	return model->getRefresh();
}

void Main::setModelPredict(bool predict)
{
	model->setPredict(predict);
}

bool Main::getModelPredict()
{
	return model->getPredict();
}
