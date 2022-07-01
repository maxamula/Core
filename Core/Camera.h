#pragma once
#include "Vector.h"
#include <DirectXMath.h>

class Camera
{
public:
	DirectX::XMMATRIX GetMatrix(Vector3 pos, Vector3 scale, Vector3 rotate, float ratio);
	Vector3 m_pos = {0.0f, 0.0f, 0.0f};
	Vector3 LookAngles = { 0.0f, 0.0f, 0.0f };
	float zNear = 0.5f;
	float zFar = 30.0f;
};

