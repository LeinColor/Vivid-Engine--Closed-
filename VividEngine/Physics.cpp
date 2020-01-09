#include "stdafx.h"
#include "Physics.h"
#include "Ray.h"
#include "Scene.h"

bool Physics::Raycast(XMVECTOR origin,
		XMVECTOR direction,
		_Out_ RaycastHit& hitInfo,
		float maxDistance,
		int layerMask)
{
	XMVECTOR vDir = XMVector3Normalize(XMVectorSubtract(direction, origin));

	XMFLOAT3 dirInv;
	XMStoreFloat3(&dirInv, vDir);
	dirInv.x = 1.0f / dirInv.x;
	dirInv.y = 1.0f / dirInv.y;
	dirInv.z = 1.0f / dirInv.z;

	for (auto& aabb : Scene::aabbs) {
		float t1 = ()
	}

	return true;
}