#include "Camera.h"


DirectX::XMMATRIX Camera::GetMatrix(Vector3 pos, Vector3 scale, Vector3 rotate, float ratio)
{
	DirectX::XMVECTOR lookDir = DirectX::XMVector3Transform(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), DirectX::XMMatrixRotationRollPitchYaw(LookAngles.x, LookAngles.y, LookAngles.z));
	return DirectX::XMMatrixTranspose
	(
		DirectX::XMMatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z)*
		DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)*
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z)*
		DirectX::XMMatrixLookToLH(DirectX::XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 0.0f), lookDir, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))*
		DirectX::XMMatrixPerspectiveLH(1.0f, ratio, zNear, zFar)
	);
}