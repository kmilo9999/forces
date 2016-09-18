/* Start Header *****************************************************************/
/*!
\file Camera.cpp
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#include "Camera.h"
#include <cmath>


Camera::Camera() :Transform(), FieldOfView(50.0f), NearPlane(0.1f), FarPlane(1000.0f), OrthoScale(1.0f),
					ViewPortAspect(4.0f / 3.0f), Projection(ProjectionType::Perspective)
{
}

Camera::Camera(Camera& other) : Transform(other.Transform), FieldOfView(other.FieldOfView), OrthoScale(1.0f),
								NearPlane(other.NearPlane), FarPlane(other.FarPlane),
								ViewPortAspect(other.ViewPortAspect), Projection(other.Projection)
{

}

Camera::~Camera()
{
	
}

/*
	Camera look at a position and using the Up vector
*/
void Camera::LookAt(Vector3& position, Vector3& up)
{
	//Vector3 pos = Transform.Position;
	float similar = 0.001f;
	if (Math::Length(position - Transform.Position) < similar)
		return;

	Vector3 f = Math::Normalize(position - Transform.Position);
	float cosTheta = Math::Dot(Vector3(0.0f, 0.0f, -1.0f), f);
	float angleInRadians = acosf(cosTheta);
	Vector3 axis = Math::Cross(Vector3(0.0f, 0.0f, -1.0f), f);
	Transform.Orientation = Math::AngleRadiansAxis(angleInRadians,axis);


}

/*
	Set orientation of the camera
*/
void Camera::offSetOrientation(float yaw, float pitch)
{
	Quaternion yawRotation = Math::AngleRadiansAxis(yaw, Vector3(0,1,0));
	Quaternion pitchRotation = Math::AngleRadiansAxis(pitch, Right());

	Transform.Orientation = Transform.Orientation  * pitchRotation * yawRotation;
}

/*
	Moves the camera forward
*/
Vector3 Camera::Forward()
{
	return Transform.Orientation * Vector3(0, 0, -1);
}

/*
Moves the camera Backward
*/
Vector3 Camera::Backward()
{
	return Transform.Orientation * Vector3(0, 0, 1);
}

/*
Move the camera Rigth
*/
Vector3 Camera::Right()
{
	return Transform.Orientation * Vector3(1, 0, 0);
}

/*
Moves the camera Left
*/
Vector3 Camera::Left()
{
	return Transform.Orientation * Vector3(-1, 0, 0);
}

/*
Rotates the camera Up
*/
Vector3 Camera::Up()
{
	return Transform.Orientation * Vector3(0, 1, 0);
}

/*
Rotates the camera Down
*/
Vector3 Camera::Down()
{
	return Transform.Orientation * Vector3(0, -1, 0);
}

/*
	Returns Projection * View Matrix
*/
Matrix4 Camera::GetMatrix() 
{
	Matrix4 proj = GetProjection();
	Matrix4 view = GetView();
	
	Matrix4 result = proj * view;
	return result;
}

/*
Returns Projection Matrix
*/
Matrix4 Camera::GetProjection()
{
	Matrix4 proj;
	if (Projection == ProjectionType::Perspective)
	{
		proj = Math::Perspective(FieldOfView, ViewPortAspect, NearPlane, FarPlane);
	}
	else if (Projection == ProjectionType::Orthographic) {
		float distance = 0.5f * (FarPlane - NearPlane);
		proj = Math::Ortho(-OrthoScale * ViewPortAspect, OrthoScale * ViewPortAspect,
			-OrthoScale, OrthoScale,-distance, distance);
	}
	return proj;
}

/*
Returns View Matrix
*/
Matrix4 Camera::GetView()
{
	Matrix4 view;

	view = Math::Scale(Vector3(1, 1, 1) / Transform.Scale)
		* Math::QuaterinionToMatrix(Math::conjugate(Transform.Orientation))
	* Math::Translate((-1)* Transform.Position);


	return view;
}

/*
Update the position of the Camera according to keyboard events
*/
void Camera::UpdateKey(bool* keys, float dt)
{
	float camVel = 50.0f;
	Vector3 velDir;
	if (keys[GLFW_KEY_W])
	{
		velDir += Forward();
	}
	if (keys[GLFW_KEY_S] )
	{
		velDir += Backward();
	}
	if (keys[GLFW_KEY_A] )
	{
		velDir += Left();
	}
	if (keys[GLFW_KEY_D] )
	{
		velDir += Right();
	}

	if (Math::Length(velDir) > 0)
	{
		velDir = Math::Normalize(velDir);
	}

	Transform.Position += camVel * velDir * dt;
}

/*
	Update the Rotation axis of the camera according to mouse events
*/
void Camera::UpdateMouse(GLFWwindow* window,float dt)
{
	int stateButtonMiddle = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
	int stateButtonLeftMiddle = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	
	if (stateButtonLeftMiddle == GLFW_PRESS)
	{
		double posiX, posiY;
		glfwGetCursorPos(window, &posiX, &posiY);

	}


	if (stateButtonMiddle == GLFW_PRESS)
	{
		const float mouseSensitivity = 1.5f;

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		double posiX, posiY;
		glfwGetCursorPos(window, &posiX, &posiY);
		posiX -= width / 2;
		posiY -= height / 2;

		float xToradians = ((float)posiX * dt) * (float)(M_PI / 180);
		float yToradians = ((float)posiY * dt) * (float)(M_PI / 180);

		offSetOrientation(mouseSensitivity * (-xToradians),
			mouseSensitivity * (-yToradians));



		glfwSetCursorPos(window, width / 2, height / 2);
	}
	
	

}