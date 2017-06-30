#pragma once

#include <vector>
#include <memory>

#include "Model.h"
#include "UI.h"

class Frame;


class Main
{
public:
	Main();
	~Main();

	//Initialize the Kinect sensor.
	void mainCanInitializeKinectSensor();
	//Update the visuals and sensor values read.
	void Update();

	//Contains the main loop of the application.
	int Run(HINSTANCE hInstance, int nCmdShow);

private:

	std::shared_ptr<Model> model;
	std::shared_ptr<UI> ui;

	IBodyFrame* pBodyFrame;
	IBodyFrameSource* pBodyFrameSource;

	// Current Kinect
	IKinectSensor*          m_pKinectSensor;
	//ICoordinateMapper*      m_pCoordinateMapper;

	// Body reader
	IBodyFrameReader*       m_pBodyFrameReader;

	/// <summary>
	/// Initializes the default Kinect sensor
	/// </summary>
	/// <returns>indicates success or failure</returns>
	HRESULT InitializeDefaultSensor();



};

