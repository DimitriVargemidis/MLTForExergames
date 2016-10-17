
#include "stdafx.h"
#include "D2D_Graphics.h"

static const float c_JointThickness = 3.0f;
static const float c_TrackedBoneThickness = 6.0f;
static const float c_InferredBoneThickness = 1.0f;
static const float c_HandSize = 30.0f;


D2D_Graphics::D2D_Graphics():

m_pD2DFactory(NULL),
m_pRenderTarget(NULL),
m_pBrushJointTracked(NULL),
m_pBrushJointInferred(NULL),
m_pBrushBoneTracked(NULL),
m_pBrushBoneInferred(NULL),
m_pBrushHandClosed(NULL),
m_pBrushHandOpen(NULL),
m_pBrushHandLasso(NULL)

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
void D2D_Graphics::DrawBody(const Joint* pJoints, const D2D1_POINT_2F* pJointPoints)
{
	// Draw the bones

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

	//SELFMADE CODE
	//simple button 
	D2D_RECT_F measureButton = D2D1::RectF(100, 0, 200, 200);
	m_pRenderTarget->FillRectangle(measureButton, m_pBrushJointInferred);




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
void D2D_Graphics::DrawBone(const Joint* pJoints, const D2D1_POINT_2F* pJointPoints, JointType joint0, JointType joint1)
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
	// Init Direct2D
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
}

void D2D_Graphics::CleanD2D()
{
	DiscardDirect2DResources();
	SafeRelease(m_pD2DFactory);
}


//SELFMADE FUNCTIONS