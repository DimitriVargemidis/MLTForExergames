//------------------------------------------------------------------------------
// <copyright file="BodyBasics.h" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#pragma once

#include "resource.h"
#include "Model.h"

class CBodyBasics
{
    static const int        cDepthWidth  = 512;
    static const int        cDepthHeight = 424;

public:
    /// <summary>
    /// Constructor
    /// </summary>
    CBodyBasics();

    /// <summary>
    /// Destructor
    /// </summary>
    ~CBodyBasics();

    /// <summary>
    /// Handles window messages, passes most to the class instance to handle
    /// </summary>
    /// <param name="hWnd">window message is for</param>
    /// <param name="uMsg">message</param>
    /// <param name="wParam">message data</param>
    /// <param name="lParam">additional message data</param>
    /// <returns>result of message processing</returns>
    static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    /// <summary>
    /// Handle windows messages for a class instance
    /// </summary>
    /// <param name="hWnd">window message is for</param>
    /// <param name="uMsg">message</param>
    /// <param name="wParam">message data</param>
    /// <param name="lParam">additional message data</param>
    /// <returns>result of message processing</returns>
    LRESULT CALLBACK        DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	//moved to UI
	
    /// <summary>
    /// Creates the main window and begins processing
    /// </summary>
    /// <param name="hInstance"></param>
    /// <param name="nCmdShow"></param>
    int                     Run(HINSTANCE hInstance, int nCmdShow);
	
	//END MOVE TO UI

private:
    HWND                    m_hWnd;
    INT64                   m_nStartTime;
    INT64                   m_nLastCounter;
    double                  m_fFreq;
    INT64                   m_nNextStatusTime;
    DWORD                   m_nFramesSinceUpdate;

    // Current Kinect
    IKinectSensor*          m_pKinectSensor;
    ICoordinateMapper*      m_pCoordinateMapper;

    // Body reader
    IBodyFrameReader*       m_pBodyFrameReader;	

	//SELFMADE PARAMETERS meant to stay here
	Model model;
	D2D_Graphics graphics;

		//SELFMADE PARAMETERS
	bool refresh = false;
	bool start_1 = true;
	bool start_2 = true;
	bool predict = false;

	D2D1_POINT_2F*	savedJointPoints;
	Joint*			savedJoints;

	D2D1_POINT_2F*	savedJointPoints_2;
	Joint*			savedJoints_2;

			
	double			SVMInputData[54];
	int				SVMInputDataIndex = 0;

	double			SVMLabels[6];
	int				SVMLabelsIndex = 0;

		//END

    /// <summary>
    /// Main processing function
    /// </summary>
    void                    Update();

    /// <summary>
    /// Initializes the default Kinect sensor
    /// </summary>
    /// <returns>S_OK on success, otherwise failure code</returns>
    HRESULT                 InitializeDefaultSensor();
    
    /// <summary>
    /// Handle new body data
    /// <param name="nTime">timestamp of frame</param>
    /// <param name="nBodyCount">body data count</param>
    /// <param name="ppBodies">body data in frame</param>
    /// </summary>
    void                    ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies);

    /// <summary>
    /// Set the status bar message
    /// </summary>
    /// <param name="szMessage">message to display</param>
    /// <param name="nShowTimeMsec">time in milliseconds for which to ignore future status messages</param>
    /// <param name="bForce">force status update</param>
    bool                    SetStatusMessage(_In_z_ WCHAR* szMessage, DWORD nShowTimeMsec, bool bForce);


	//SELFMADE FUNCTIONS

	/// <summary>
	/// Draws one bone of a body (joint to joint)
	/// </summary>
	/// <param name="joints">joint data array</param>
	/// <param name="tab">is 0 if current body data needs to be set, is 1 if saved body data needs to be set</param>
	void CBodyBasics::ShowJointCoordinates(const Joint* joints, int tab);


	/// <summary>
	/// Adds de joint data of the right arm to the SVMInputData and the SVMLabel array
	/// </summary>
	/// <param name="joints">joint data array</param>
	/// <param name="tab">label of the gesture to which the joint data belongs</param>
	void CBodyBasics::AddDataToSVMInputData(const Joint* joints, int label);

	/// <summary>
	/// enters the joint data in to de predict function of the SVM, return the class in which it falls
	/// </summary>
	/// <param name="joints">joint data array</param>
	int CBodyBasics::MakePrediction(const Joint* joints);
	//END
};

