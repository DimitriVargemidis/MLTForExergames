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

	void setModelRefresh(bool refresh);
	bool getModelRefresh();

	void setModelPredict(bool refresh);
	bool getModelPredict();

	/// <summary>
	/// Main processing function
	/// </summary>
	void Update();

	int Run(HINSTANCE hInstance, int nCmdShow);

	void drawFrames(std::vector<Frame> frames);

private:

	//Model model;
	//UI ui;

	std::shared_ptr<Model> model;
	std::shared_ptr<UI> ui;
	

	boolean running;

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

