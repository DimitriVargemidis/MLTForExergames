
#include <iostream>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#include "stdafx.h"
#include <strsafe.h>
#include <iostream>
#include "resource.h"
#include "Model.h"
#include "Frame.h"
#include "UI.h"
#include "Console.h"
#include "Filewriter.h"
#include "Filereader.h"
#include "ProjectGesture.h"

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
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	Console::useConsole();

	Console::printsl("Initializing... DONE.");

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

int Main::Run(HINSTANCE hInstance, int nCmdShow)
{
	ui = std::make_shared<UI>();
	model = std::make_shared<Model>();

	std::shared_ptr<Main> shared_ptr_this(this);

	ui->setMain(shared_ptr_this);
	ui->setModel(model);
	model->setView(ui);

	int rc = ui->Run(hInstance, nCmdShow);

	/*********************************************************************

	============================= TESTS ==================================
	
	*********************************************************************/

	//test clocking code 
	/*
	auto t1 = Clock::now();
	Sleep(30);
	auto t2 = Clock::now();

	long time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

	wchar_t buffer[256];
	wsprintfW(buffer, L"%ld", time);
	OutputDebugStringW(L"time between t1 en t2 ");
	OutputDebugStringW(buffer);
	OutputDebugStringW(L" milliseconds \n");
	*/
	//end test

	/*
	std::vector<Joint> joints;
	Joint j1;
	j1.Position.X = 1.1;
	j1.Position.Y = 1.2;
	j1.Position.Z = 1.3;
	Joint j2;
	j2.Position.X = -2.1;
	j2.Position.Y = 2.2;
	j2.Position.Z = -2.3;
	joints.push_back(j1);
	joints.push_back(j2);

	Frame frame1(joints, false);
	Frame frame2(joints, false);
	Frame frame3(joints, false);
	Frame frame4(joints, false);

	Gesture gesture1;
	gesture1.addFrame(frame1);
	gesture1.addFrame(frame2);

	Gesture gesture2;
	gesture2.addFrame(frame1);
	gesture2.addFrame(frame2);

	Gesture gesture3;
	gesture3.addFrame(frame3);
	gesture3.addFrame(frame4);

	GestureClass gestureClass1;
	gestureClass1.addGesture(gesture1);
	gestureClass1.addGesture(gesture2);

	GestureClass gestureClass2;
	gestureClass2.addGesture(gesture3);

	ProjectGesture projectGesture1(gestureClass1);
	projectGesture1.setLabel(1.0);
	projectGesture1.addAction(64, true);

	ProjectGesture projectGesture2(gestureClass2);
	projectGesture2.setLabel(2.001);
	projectGesture2.addAction(65, false);
	projectGesture2.addAction(66, true);

	Project project;
	project.addProjectGesture(projectGesture1);
	project.addProjectGesture(projectGesture2);

	Console::print("Test print joint: ");
	Console::print(project.getProjectGestures().at(0).getGestureClass().getGestures().at(0).getFrames().at(0).getJoints().at(0).Position.X);
	Console::print(project.getProjectGestures().at(0).getGestureClass().getGestures().at(0).getFrames().at(0).getJoints().at(0).Position.Y);
	Console::print(project.getProjectGestures().at(0).getGestureClass().getGestures().at(0).getFrames().at(0).getJoints().at(0).Position.Z);
	Console::print(j1.Position.X);
	Console::print(j1.Position.Y);
	Console::print(j1.Position.Z);
	
	std::string fileName = "1.project";
	Project loadedProject = Filereader::readProjectFromFile(fileName);

	for (ProjectGesture pg : loadedProject.getProjectGestures())
	{
		Console::print("Label: ");
		Console::printsl(pg.getLabel());
	}
	*/
	//Filewriter::save(project);



	/*********************************************************************

	========================= END TESTS ==================================

	*********************************************************************/


	while (ui->checkQuitMsg())
	{
		Update();
		ui->checkPeekMsg();
	}
	return 0;
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
			model->ProcessBody(nTime, BODY_COUNT, ppBodies);
		}

		for (int i = 0; i < _countof(ppBodies); ++i)
		{
			SafeRelease(ppBodies[i]);
		}
	}
	SafeRelease(pBodyFrame);
}


