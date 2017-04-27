#pragma once

#include <vector>
#include <memory>

#include "Model.h"
#include "UI.h"

//class UI;
//class Model;
class Frame;

class Main
{
public:
	Main();
	~Main();

	void mainCanInitializeKinectSensor();


	/// <summary>
	/// Main processing function
	/// </summary>
	void Update();

	int Run(HINSTANCE hInstance, int nCmdShow);

private:

	//Model model;
	//UI ui;

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

