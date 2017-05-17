#include <iostream>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#include "stdafx.h"
#include <strsafe.h>
#include <iostream>
#include "resource.h"
#include "Model.h"
#include "UI.h"
#include "Console.h"
#include "Filewriter.h"
#include "Filereader.h"
#include "FilenameChecker.h"

#include "Main.h"

std::shared_ptr<UI> UI_global;
std::shared_ptr<Model> model_global;

/// <summary>
/// Entry point for the application
/// </summary>
/// <param name="hInstance">handle to the application instance</param>
/// <param name="hPrevInstance">always 0</param>
/// <param name="lpCmdLine">command line arguments</param>
/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
/// <returns>status</returns>
int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	Console::useConsole();
	
	//Create data directory if it does not exist.
	std::wstring dirTemp = FilenameChecker::stringToWstring(Filewriter::subDirectoryNameString);
	LPCWSTR dir = dirTemp.c_str();
	CreateDirectory(dir, NULL);

	//Initialize the IDs depending on the files saved in the DATA directory.
	appGestureID = FilenameChecker::getHighestIDStored(Filewriter::gestureExtension);
	appProjectID = FilenameChecker::getHighestIDStored(Filewriter::projectExtension);
	appGestureClassID = FilenameChecker::getHighestIDStored(Filewriter::gestureClassExtension);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	Main application;
	application.Run(hInstance, nShowCmd);
}


Main::Main() : m_pKinectSensor(NULL), m_pBodyFrameReader(NULL)
{
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
		return E_FAIL;
	}

	return hr;
}

int Main::Run(HINSTANCE hInstance, int nCmdShow)
{
	ui = std::make_shared<UI>();
	model = std::make_shared<Model>();

	UI_global = ui;
	model_global = model;

	printf("start \n");

	std::shared_ptr<Main> shared_ptr_this(this);

	ui->setMain(shared_ptr_this);
	model->setView(ui);
	
	int rc = ui->Run(hInstance, nCmdShow);

	ui->createScreen();
	ui->setModel(model);



	while (ui->checkQuitMsg())
	{
		Update();
		ui->checkPeekMsg();
	}
	return 0;
}

void Main::Update()
{
	if (!m_pBodyFrameReader)
	{
		return;
	}

	pBodyFrame = NULL;
	pBodyFrameSource = NULL;

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
			model->processBody(nTime, BODY_COUNT, ppBodies);
		}

		for (int i = 0; i < _countof(ppBodies); ++i)
		{
			SafeRelease(ppBodies[i]);
		}
	}
	SafeRelease(pBodyFrame);
}


