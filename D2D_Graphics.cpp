#include <vector>
#include <dwrite.h>

#include "stdafx.h"
#include "resource.h"
#include "ImageLoader.h"

#include "D2D_Graphics.h"

static const float c_JointThickness = 3.0f;
static const float c_TrackedBoneThickness = 6.0f;
static const float c_InferredBoneThickness = 1.0f;
static const float c_HandSize = 30.0f;


D2D_Graphics::D2D_Graphics():
m_pD2DFactory(NULL),
m_pDWriteFactory(NULL),
m_pTextFormat(NULL),
m_pRenderTarget(NULL),
m_pBrushJointTracked(NULL),
m_pBrushJointInferred(NULL),
m_pBrushBoneTracked(NULL),
m_pBrushBoneInferred(NULL),
m_pBrushHandClosed(NULL),
m_pBrushHandOpen(NULL),
m_pBrushHandLasso(NULL),
m_pBrushUpButton(NULL),
m_pBrushDownButton(NULL),
m_pBrushRightButton(NULL),
m_pBrushLeftButton(NULL),
m_pBrushAButton(NULL)
{

}


D2D_Graphics::~D2D_Graphics()
{
}


ID2D1HwndRenderTarget * D2D_Graphics::GetRenderTarget()
{
	return m_pRenderTarget;
}

void D2D_Graphics::SetRenderTarget(ID2D1HwndRenderTarget * renderTarget)
{
	m_pRenderTarget = renderTarget;
}

/// <summary>
/// Ensure necessary Direct2d resources are created
/// </summary>
/// <returns>S_OK if successful, otherwise an error code</returns>
HRESULT D2D_Graphics::EnsureDirect2DResources(HWND m_hWnd)
{
	HRESULT hr = S_OK;

	if (m_pD2DFactory && !m_pRenderTarget)
	{
		RECT rc;
		GetWindowRect(GetDlgItem(m_hWnd, IDC_VIDEOVIEW), &rc);

		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;
		D2D1_SIZE_U size = D2D1::SizeU(width, height);
		D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
		rtProps.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
		rtProps.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;

		// Create a Hwnd render target, in order to render to the window set in initialize
		hr = m_pD2DFactory->CreateHwndRenderTarget(
			rtProps,
			D2D1::HwndRenderTargetProperties(GetDlgItem(m_hWnd, IDC_VIDEOVIEW), size),
			&m_pRenderTarget
		);

		if (FAILED(hr))
		{
			//SetStatusMessage(L"Couldn't create Direct2D render target!", 10000, true);
			return hr;
		}

		// light green
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.27f, 0.75f, 0.27f), &m_pBrushJointTracked);

		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow, 1.0f), &m_pBrushJointInferred);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 1.0f), &m_pBrushBoneTracked);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray, 1.0f), &m_pBrushBoneInferred);

		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 0.5f), &m_pBrushHandClosed);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, 0.5f), &m_pBrushHandOpen);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushHandLasso);

		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushUpButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushDownButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushLeftButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushRightButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushAButton);

		for (int i = 0; i < 5; ++i)
		{
			//buttonC
		}


	}

	return hr;
}

/// <summary>
/// Dispose Direct2d resources 
/// </summary>
void D2D_Graphics::DiscardDirect2DResources()
{
	SafeRelease(m_pRenderTarget);

	SafeRelease(m_pBrushJointTracked);
	SafeRelease(m_pBrushJointInferred);
	SafeRelease(m_pBrushBoneTracked);
	SafeRelease(m_pBrushBoneInferred);

	SafeRelease(m_pBrushHandClosed);
	SafeRelease(m_pBrushHandOpen);
	SafeRelease(m_pBrushHandLasso);
	SafeRelease(m_pBrushUpButton);
	SafeRelease(m_pBrushDownButton);
	SafeRelease(m_pBrushRightButton);
	SafeRelease(m_pBrushLeftButton);
	SafeRelease(m_pBrushAButton);
}



/// <summary>
/// Converts a body point to screen space
/// </summary>
/// <param name="bodyPoint">body point to tranform</param>
/// <param name="width">width (in pixels) of output buffer</param>
/// <param name="height">height (in pixels) of output buffer</param>
/// <returns>point in screen-space</returns>
D2D1_POINT_2F D2D_Graphics::BodyToScreen(const CameraSpacePoint& bodyPoint, int width, int height, ICoordinateMapper* m_pCoordinateMapper, int cDepthWidth, int cDepthHeight)
{
	// Calculate the body's position on the screen
	DepthSpacePoint depthPoint = { 0 };
	m_pCoordinateMapper->MapCameraPointToDepthSpace(bodyPoint, &depthPoint);

	float screenPointX = static_cast<float>(depthPoint.X * width) / cDepthWidth;
	float screenPointY = static_cast<float>(depthPoint.Y * height) / cDepthHeight;

	return D2D1::Point2F(screenPointX, screenPointY);
}

/// <summary>
/// Draws a body 
/// </summary>
/// <param name="pJoints">joint data</param>
/// <param name="pJointPoints">joint positions converted to screen space</param>
void D2D_Graphics::DrawBody(const std::vector<Joint> pJoints, const std::vector<D2D1_POINT_2F> pJointPoints, const int colorID)
{
	// Draw the bones
	changeColor(colorID);

	// Torso
	DrawBone(pJoints, pJointPoints, JointType_Head, JointType_Neck);
	DrawBone(pJoints, pJointPoints, JointType_Neck, JointType_SpineShoulder);
	DrawBone(pJoints, pJointPoints, JointType_SpineShoulder, JointType_SpineMid);
	DrawBone(pJoints, pJointPoints, JointType_SpineMid, JointType_SpineBase);
	DrawBone(pJoints, pJointPoints, JointType_SpineShoulder, JointType_ShoulderRight);
	DrawBone(pJoints, pJointPoints, JointType_SpineShoulder, JointType_ShoulderLeft);
	DrawBone(pJoints, pJointPoints, JointType_SpineBase, JointType_HipRight);
	DrawBone(pJoints, pJointPoints, JointType_SpineBase, JointType_HipLeft);

	// Right Arm    
	DrawBone(pJoints, pJointPoints, JointType_ShoulderRight, JointType_ElbowRight);
	DrawBone(pJoints, pJointPoints, JointType_ElbowRight, JointType_WristRight);
	DrawBone(pJoints, pJointPoints, JointType_WristRight, JointType_HandRight);
	DrawBone(pJoints, pJointPoints, JointType_HandRight, JointType_HandTipRight);
	DrawBone(pJoints, pJointPoints, JointType_WristRight, JointType_ThumbRight);

	// Left Arm
	DrawBone(pJoints, pJointPoints, JointType_ShoulderLeft, JointType_ElbowLeft);
	DrawBone(pJoints, pJointPoints, JointType_ElbowLeft, JointType_WristLeft);
	DrawBone(pJoints, pJointPoints, JointType_WristLeft, JointType_HandLeft);
	DrawBone(pJoints, pJointPoints, JointType_HandLeft, JointType_HandTipLeft);
	DrawBone(pJoints, pJointPoints, JointType_WristLeft, JointType_ThumbLeft);

	// Right Leg
	DrawBone(pJoints, pJointPoints, JointType_HipRight, JointType_KneeRight);
	DrawBone(pJoints, pJointPoints, JointType_KneeRight, JointType_AnkleRight);
	DrawBone(pJoints, pJointPoints, JointType_AnkleRight, JointType_FootRight);

	// Left Leg
	DrawBone(pJoints, pJointPoints, JointType_HipLeft, JointType_KneeLeft);
	DrawBone(pJoints, pJointPoints, JointType_KneeLeft, JointType_AnkleLeft);
	DrawBone(pJoints, pJointPoints, JointType_AnkleLeft, JointType_FootLeft);

	// Draw the joints
	for (int i = 0; i < JointType_Count; ++i)
	{
		D2D1_ELLIPSE ellipse = D2D1::Ellipse(pJointPoints[i], c_JointThickness, c_JointThickness);

		if (pJoints[i].TrackingState == TrackingState_Inferred)
		{
			m_pRenderTarget->FillEllipse(ellipse, m_pBrushJointInferred);
		}
		else if (pJoints[i].TrackingState == TrackingState_Tracked)
		{
			m_pRenderTarget->FillEllipse(ellipse, m_pBrushJointTracked);
		}
	}

	/*

	//SELFMADE CODE
	//simple button 
	int Xoffset = 100;
	int size = 50;
	int Xpos = 400;
	int Ypos = 450;

	D2D_RECT_F measureButton = D2D1::RectF(1000, 1000, 1200, 1200);
	D2D_RECT_F up = D2D1::RectF(Xpos + size + Xoffset, Ypos, Xpos + size + size + Xoffset, Ypos + size);
	D2D_RECT_F left = D2D1::RectF(Xpos + Xoffset, Ypos + size, Xpos + size + Xoffset, Ypos + size + size);
	D2D_RECT_F down = D2D1::RectF(Xpos + size + Xoffset, Ypos + size + size, Xpos + size + size + Xoffset, Ypos + size + size + size);
	D2D_RECT_F right = D2D1::RectF(Xpos + size + size + Xoffset, Ypos + size, Xpos + size + size + size + Xoffset, Ypos + size + size);
	D2D1_POINT_2F APos;
	APos.x = 150;
	APos.y = 550;
	D2D1_ELLIPSE Abutton = D2D1::Ellipse(APos, 75, 75);


	m_pRenderTarget->FillRectangle(up, m_pBrushUpButton);
	m_pRenderTarget->FillRectangle(down, m_pBrushDownButton);
	m_pRenderTarget->FillRectangle(left, m_pBrushLeftButton);
	m_pRenderTarget->FillRectangle(right, m_pBrushRightButton);
	m_pRenderTarget->FillEllipse(Abutton, m_pBrushAButton);

	*/


	//END SELFMADE
}

/// <summary>
/// Draws one bone of a body (joint to joint)
/// </summary>
/// <param name="pJoints">joint data</param>
/// <param name="pJointPoints">joint positions converted to screen space</param>
/// <param name="pJointPoints">joint positions converted to screen space</param>
/// <param name="joint0">one joint of the bone to draw</param>
/// <param name="joint1">other joint of the bone to draw</param>
void D2D_Graphics::DrawBone(const std::vector<Joint> pJoints, const std::vector<D2D1_POINT_2F> pJointPoints, JointType joint0, JointType joint1)
{

	TrackingState joint0State = pJoints[joint0].TrackingState;
	TrackingState joint1State = pJoints[joint1].TrackingState;

	// If we can't find either of these joints, exit
	if ((joint0State == TrackingState_NotTracked) || (joint1State == TrackingState_NotTracked))
	{
		return;
	}

	// Don't draw if both points are inferred
	if ((joint0State == TrackingState_Inferred) && (joint1State == TrackingState_Inferred))
	{
		return;
	}

	// We assume all drawn bones are inferred unless BOTH joints are tracked
	if ((joint0State == TrackingState_Tracked) && (joint1State == TrackingState_Tracked))
	{
		m_pRenderTarget->DrawLine(pJointPoints[joint0], pJointPoints[joint1], m_pBrushBoneTracked, c_TrackedBoneThickness);
	}
	else
	{
		m_pRenderTarget->DrawLine(pJointPoints[joint0], pJointPoints[joint1], m_pBrushBoneInferred, c_InferredBoneThickness);
	}
}


/// <summary>
/// Draws a hand symbol if the hand is tracked: red circle = closed, green circle = opened; blue circle = lasso
/// </summary>
/// <param name="handState">state of the hand</param>
/// <param name="handPosition">position of the hand</param>
void D2D_Graphics::DrawHand(HandState handState, const D2D1_POINT_2F& handPosition)
{
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(handPosition, c_HandSize, c_HandSize);

	switch (handState)
	{
	case HandState_Closed:
		m_pRenderTarget->FillEllipse(ellipse, m_pBrushHandClosed);
		break;

	case HandState_Open:
		m_pRenderTarget->FillEllipse(ellipse, m_pBrushHandOpen);
		break;

	case HandState_Lasso:
		m_pRenderTarget->FillEllipse(ellipse, m_pBrushHandLasso);
		break;
	}
}


void D2D_Graphics::InitD2D()
{
	static const WCHAR msc_fontName[] = L"Verdana";
	static const FLOAT msc_fontSize = 50;
	HRESULT hr;

	// Init Direct2D
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

	//code from https://msdn.microsoft.com/en-us/library/windows/desktop/dd756692(v=vs.85).aspx
	if (SUCCEEDED(hr))
	{

		// Create a DirectWrite factory.
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(m_pDWriteFactory),
			reinterpret_cast<IUnknown **>(&m_pDWriteFactory)
		);
	}
	if (SUCCEEDED(hr))
	{
		// Create a DirectWrite text format object.
		hr = m_pDWriteFactory->CreateTextFormat(
			msc_fontName,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			msc_fontSize,
			L"", //locale
			&m_pTextFormat
		);
	}
	if (SUCCEEDED(hr))
	{
		// Center the text horizontally and vertically.
		m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

		m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


	}



}

void D2D_Graphics::CleanD2D()
{
	DiscardDirect2DResources();
	SafeRelease(m_pD2DFactory);

	//Direct2D text resources
	SafeRelease(m_pTextFormat);
	SafeRelease(m_pDWriteFactory);
			
		

}

//limited to 15 different color
void D2D_Graphics::changeColor(int colorID)
{
	
	switch (colorID)
	{
	case 0:
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.25f, 0.75f, 0.25f), &m_pBrushJointTracked);

		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0.0f), &m_pBrushJointInferred);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 1.0f, 0.0f), &m_pBrushBoneTracked);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray, 1.0f), &m_pBrushBoneInferred);

		break;
	case 1:
		

		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f , 0.0f , 0.0f), &m_pBrushJointTracked);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.0f, 0.0f ), &m_pBrushBoneTracked);
		break;
	case 2:
		

		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.0f, 1.0f), &m_pBrushJointTracked);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.0f, 1.0f), &m_pBrushBoneTracked);
		break;
	case 3:


		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 1.0f), &m_pBrushJointTracked);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 1.0f), &m_pBrushBoneTracked);
		break;
	case 4:


		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0.0f), &m_pBrushJointTracked);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0.0f), &m_pBrushBoneTracked);
		break;
	case 5:


		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f), &m_pBrushJointTracked);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f), &m_pBrushBoneTracked);
		break;

	case 6:
	

		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 1.0f, 1.0f), &m_pBrushJointTracked);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 1.0f, 1.0f), &m_pBrushBoneTracked);
		break;
	case 7:


		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.5f, 0.5f, 0.5f), &m_pBrushJointTracked);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.5f, 0.5f, 0.5f), &m_pBrushBoneTracked);
		break;
	}
	

	/*
	if (colorID < 15)
	{
		float c = colorID*0.05; //the change in the color from skelet to skelet

		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.25f + c, 0.75f - c, 0.25f + c), &m_pBrushJointTracked);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f + c, 1.0f - c, 0.0f + c), &m_pBrushBoneTracked);
	}
	else
	{
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.25f, 0.75f, 0.25f), &m_pBrushJointTracked);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 1.0f, 0.0f), &m_pBrushBoneTracked);
	}
	*/
}

void D2D_Graphics::changeButtonColor(int state)
{


	ID2D1SolidColorBrush** buttonColor;

	switch (state)
	{
	case(1):
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushUpButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushDownButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushLeftButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushRightButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushAButton);
		break;
	case 3:
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushUpButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushDownButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red , 0.5f), &m_pBrushLeftButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushRightButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushAButton);
		break;
	case 5:
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushUpButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushDownButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushLeftButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 0.5f), &m_pBrushRightButton); //activated
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushAButton);
		break;
	case 7:
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushUpButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushDownButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 0.5f), &m_pBrushLeftButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushRightButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 0.5f), &m_pBrushAButton);
		break;
	case 8:
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushUpButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushDownButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushLeftButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 0.5f), &m_pBrushRightButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 0.5f), &m_pBrushAButton);
		break;
	case 9:
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushUpButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushDownButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushLeftButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &m_pBrushRightButton);
		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red, 0.5f), &m_pBrushAButton);
		break;


	}

}

void D2D_Graphics::drawRectangle(D2D1_POINT_2F center, float width, float height, D2D1::ColorF color)
{
	ID2D1SolidColorBrush* colorBrush;
	D2D_RECT_F rect = D2D1::RectF(center.x-width/2, center.y - height / 2, center.x + width / 2, center.y + height / 2);
	m_pRenderTarget->CreateSolidColorBrush(color, &colorBrush);
	m_pRenderTarget->FillRectangle(rect, colorBrush);
	SafeRelease(colorBrush);
}


void D2D_Graphics::drawText(std::wstring text, D2D1_POINT_2F center, float width, float height, D2D1::ColorF color, float fontSize, DWRITE_TEXT_ALIGNMENT alignment)
{
	IDWriteTextFormat*		customTextFormat;

	bool hr;
	const WCHAR msc_fontName[] = L"Verdana";


	// Create a DirectWrite text format object.
	hr = m_pDWriteFactory->CreateTextFormat(
		msc_fontName,
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"", //locale
		&customTextFormat
	);

	if (SUCCEEDED(hr))
	{
		// Center the text horizontally and vertically.
		customTextFormat->SetTextAlignment(alignment);

		customTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


	}


	const wchar_t* chartext = text.c_str();
	ID2D1SolidColorBrush* m_pBlackBrush(NULL);

	m_pRenderTarget->CreateSolidColorBrush(color, &m_pBlackBrush);

	static bool writen = false;

	if (!writen)
	{
		printf(" string size = %d \n", (text.size()));
		//	printf("arreay size = %d\  ", (ARRAYSIZE(sc_helloWorld) - 1));
		writen = true;
	}



	m_pRenderTarget->DrawText(
		chartext,
		text.size(),
		customTextFormat,
		D2D1::RectF(center.x - width / 2, center.y - height, center.x + width / 2, center.y + height),
		m_pBlackBrush
	);

	SafeRelease(customTextFormat);
}

ID2D1Bitmap * D2D_Graphics::createBitmap(std::wstring filename, int srcWidth, int srcHeight)
{
	RGBQUAD*			RGBimage = new RGBQUAD[srcWidth * srcHeight];
	ID2D1Bitmap*		bitmap;

	const wchar_t* chartext = filename.c_str();

	ImageLoader::LoadResourceImage(chartext, L"Image", srcWidth, srcHeight, RGBimage);

	

	D2D1_SIZE_U size = D2D1::SizeU(srcWidth, srcHeight);

	//m_pRenderTarget->CreateBitmap(m_pRenderTarget->GetPixelSize(), NULL, (UINT32)(1920 * 1080), D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)), &bitmap);
	m_pRenderTarget->CreateBitmap(size, NULL, (UINT32)100/*(1920 * 1080)*/, D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)), &bitmap);

	//convert image to bitmaps

	if (RGBimage)
	{
		//int size = strlen((char*)image);
		bitmap->CopyFromMemory(NULL, RGBimage, srcWidth * sizeof(RGBQUAD));
	}

	free(RGBimage);

	return bitmap;
}

void D2D_Graphics::drawBitmap(RGBQUAD * image, int srcWidth, int srcHeight, D2D1_POINT_2F center, float width, float height)
{
	ID2D1Bitmap*				bitmap;

	D2D1_SIZE_U size = D2D1::SizeU(srcWidth,srcHeight);
	

	//m_pRenderTarget->CreateBitmap(m_pRenderTarget->GetPixelSize(), NULL, (UINT32)(1920 * 1080), D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)), &bitmap);
	m_pRenderTarget->CreateBitmap(size, NULL, (UINT32)100/*(1920 * 1080)*/, D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)), &bitmap);

	//convert image to bitmaps

	if (image)
	{
		//int size = strlen((char*)image);
		bitmap->CopyFromMemory(NULL, image, srcWidth * sizeof(RGBQUAD));
	}


	D2D1_RECT_F destRect;
	destRect.top = center.y - height/2;
	destRect.left = center.x - width/2;
	destRect.right= center.x + width/2;
	destRect.bottom = center.y + height/2;

	//m_pRenderTarget->CreateBitmap(m_pRenderTarget->GetPixelSize(), image,(UINT32)(1920 * 1080), &bitmap);
	//m_pRenderTarget->DrawBitmap(bitmap,destRect,1.0, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
	m_pRenderTarget->DrawBitmap(bitmap, destRect, 1.0, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);


	

	/*
	//D2D1_ELLIPSE ellipse = D2D1::Ellipse(handPosition, c_HandSize, c_HandSize);
	D2D1_RECT_F destinationRect = D2D1::RectF(80,120, 160, 240);
	//D2D1_RECT_F destinationRect = D2D1::RectF(handPosition.x - 60, handPosition.y - 40, handPosition.x + 60, handPosition.y + 40);
	D2D1_RECT_F sourceRect = D2D1::RectF(0, 0, 80, 120);

	//destinationRect.top = destinationRect.bottom + sin(rightHandRotation) * 120;
	//destinationRect.left = destinationRect.right + cos(rightHandRotation) * 80;

		//  m_pRenderTarget->FillEllipse(ellipse, m_pBrushHandClosed);	
		//m_pRenderTarget->DrawBitmap(bitmapClosed,destinationRect);
		m_pRenderTarget->DrawBitmap(bitmap, destinationRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, sourceRect);
	*/

	SafeRelease(bitmap);
}

void D2D_Graphics::drawBitmap(ID2D1Bitmap * bitmap, D2D1_POINT_2F center, float width, float height)
{
	D2D1_RECT_F destRect;
	destRect.top = center.y - height / 2;
	destRect.left = center.x - width / 2;
	destRect.right = center.x + width / 2;
	destRect.bottom = center.y + height / 2;

	m_pRenderTarget->DrawBitmap(bitmap, destRect, 1.0, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}

void D2D_Graphics::scaleSkeleton(std::vector<D2D1_POINT_2F>& jointPoints, float multiplier)
{
	D2D1_POINT_2F center;
	center.x = jointPoints[JointType_SpineMid].x;
	center.y = jointPoints[JointType_SpineMid].y;

	for (int j = 0; j < jointPoints.size(); ++j)
	{
		//Convert to coordinates relative to the spine
		if (j != JointType_SpineMid)
		{
			jointPoints[j].x = center.x + (jointPoints[j].x - center.x)*multiplier;
			jointPoints[j].y = center.y + (jointPoints[j].y - center.y)*multiplier;
		}
	}
}

void D2D_Graphics::scaleSkeleton(std::vector<D2D1_POINT_2F>& jointPoints, const float startWidth, const float startHeight, const float endWidth, const float endHeight, const float absXpos, const float absYpos)
{
	
	D2D1_POINT_2F center;
	center.x = jointPoints[JointType_SpineMid].x;
	center.y = jointPoints[JointType_SpineMid].y;

	float absXposition;
	float absYposition;

	if (absXpos == -1)
	{
		absXposition = center.x;
	}
	else
	{
		absXposition = absXpos;
	}

	if (absYpos == -1)
	{
		absYposition = center.y;
	}
	else
	{
		absYposition = absYpos;
	}

	float Xmultiplier = endWidth / startWidth;
	float Ymultiplier = endHeight / startHeight;

	//printf("Xmultiplier = %f Ymultiplier = %f", Xmultiplier, Ymultiplier);

	for (int j = 0; j < jointPoints.size(); ++j)
	{
		jointPoints[j].x = absXposition + (jointPoints[j].x - center.x)*Xmultiplier;
		jointPoints[j].y = absYposition + (jointPoints[j].y - center.y)*Ymultiplier;
	}
}


//SELFMADE FUNCTIONS