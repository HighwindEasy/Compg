#include "Camera2.h"
#include "Application.h"
#include "Mtx44.h"

Camera2::Camera2()
{
}

Camera2::~Camera2()
{
}

void Camera2::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
	SetCursorPos(1600 / 2, 900 / 2);
}

void Camera2::Update(double dt)
{
	//Mouse shit
	POINT currentposition_;
	GetCursorPos(&currentposition_);
	static const float CAMERA_SPEED = 40.f;
	//Move front
	if (Application::IsKeyPressed('W'))
	{
		Vector3 view = (target - position).Normalized();
		view.y = 0;
		position += (view * dt * 10.f);
		target += (view * dt * 10.f);
	}

	//Move Back
	if (Application::IsKeyPressed('S'))
	{
		Vector3 view = (target - position).Normalized();
		view.y = 0;
		position -= (view * dt * 10.f);
		target -= (view * dt * 10.f);
	}

	//Move Right
	if (Application::IsKeyPressed('D'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right=view.Cross(up);
		right.y = 0;
		position += (right * dt * 10.f);
		target += (right * dt * 10.f);
	}
	//Move Left
	if (Application::IsKeyPressed('A'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		position -= (right * dt * 10.f);
		target -= (right * dt * 10.f);
	}
	//Rotate up
	if (Application::IsKeyPressed(VK_UP)) //Pitch
	{
		//Original
		float pitch = (float)(CAMERA_SPEED * dt);
		/*Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		position = rotation * position;*/
		// FOr FP view
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;

	}
	//Rotate Down
	if (Application::IsKeyPressed(VK_DOWN)) //Pitch, down
	{
		float pitch = (float)(-CAMERA_SPEED * dt);
		//Original
		/*Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		position = rotation * position;*/
		//Fp Camera
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	//Rotate Right
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		float yaw = (float)(-CAMERA_SPEED * dt);
		//Original
		/*	Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		position = rotation * position;
		up = rotation * up;*/
		//FP camera
		Vector3 view = (target - position).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		up = rotation*up;

	}
	//Rotate Left
	if (Application::IsKeyPressed(VK_LEFT))
	{
		float yaw = (float)(CAMERA_SPEED * dt);
		//Original
		/*Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		position = rotation * position;
		up = rotation * up;*/
		//FP camera
		Vector3 view = (target - position).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		
		view = rotation * view;
		target = position + view;
		up = rotation*up;
	}
	if (currentposition_.x > (1600/2))
	{
		float yaw = (float)(-CAMERA_SPEED * dt);
		Vector3 view = (target - position).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		up = rotation*up;
	}
	if (currentposition_.x < (1600 / 2))
	{
		float yaw = (float)(CAMERA_SPEED * dt);
		Vector3 view = (target - position).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);

		view = rotation * view;
		target = position + view;
		up = rotation*up;
	}
	if (currentposition_.y > (900 / 2))
	{
		float pitch = (float)(-CAMERA_SPEED * dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	if (currentposition_.y < (900 / 2))
	{
		float pitch = (float)(CAMERA_SPEED * dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	if (Application::IsKeyPressed('R'))
	{
		Reset();
	}
	SetCursorPos(1600 / 2, 900 / 2);


}

void Camera2::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}