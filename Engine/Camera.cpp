#include "Camera.h"

Camera::Camera()
{
	_positionX = 0.0f;
	_positionY = 0.0f;
	_positionZ = 0.0f;

	_rotationX = 0.0f;
	_rotationY = 0.0f;
	_rotationZ = 0.0f;
}

Camera::Camera(const Camera& other)
{}

Camera::~Camera()
{}

void Camera::SetPosition(float x, float y, float z)
{
	_positionX = x;
	_positionY = y;
	_positionZ = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	_rotationX = x;
	_rotationY = y;
	_rotationZ = z;
}

D3DXVECTOR3 Camera::GetPosition()
{
	return D3DXVECTOR3(_positionX, _positionY, _positionZ);
}

D3DXVECTOR3 Camera::GetRotation()
{
	return D3DXVECTOR3(_rotationX, _rotationY, _rotationZ);
}

void Camera::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;

	//Set the vector that points upwards
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//Setup the position of the camera in the world
	position.x = _positionX;
	position.y = _positionY;
	position.z = _positionZ;

	//Setup where the camera is looking by default
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	//Set the yaw (Y axis), pitch (X axis) and roll (Z axis) rotation in radians
	pitch = _rotationX*0.0174532925f;
	yaw = _rotationY*0.0174532925f;
	roll = _rotationZ*0.0174532925f;

	//Create the rotation matrix for the yaw, pitch and roll values
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	//Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	//Translate the rotated camera position to the location of the viewer
	lookAt = position + lookAt;

	//Finally create the view matrix from the three updated vectors
	D3DXMatrixLookAtLH(&_viewMatrix, &position, &lookAt, &up);
}

void Camera::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = _viewMatrix;
}