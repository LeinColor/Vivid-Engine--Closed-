#include "stdafx.h"
#include "Physics.h"
#include "Ray.h"
#include "AABB.h"
#include "Scene.h"

bool Physics::Raycast(XMVECTOR origin,
		XMVECTOR direction,
		_Out_ RaycastHit& hitInfo,
		float maxDistance,
		int layerMask)
{
	float distance = 0.0f;
	XMVECTOR vDir = XMVector3Normalize(XMVectorSubtract(direction, origin));

	XMFLOAT3 org;
	XMFLOAT3 dirInv;
	XMStoreFloat3(&org, origin);
	XMStoreFloat3(&dirInv, vDir);
	dirInv.x = 1.0f / dirInv.x;
	dirInv.y = 1.0f / dirInv.y;
	dirInv.z = 1.0f / dirInv.z;

	for (int i = 0; i < Scene::aabbs.size(); i++) {
		XMFLOAT3& lb = Scene::aabbs[i]->minPos;
		XMFLOAT3& rt = Scene::aabbs[i]->maxPos;

		float t1 = (lb.x - org.x) * dirInv.x;
		float t2 = (rt.x - org.x) * dirInv.x;
		float t3 = (lb.y - org.y) * dirInv.y;
		float t4 = (rt.y - org.y) * dirInv.y;
		float t5 = (lb.z - org.z) * dirInv.z;
		float t6 = (rt.z - org.z) * dirInv.z;

		float tMin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
		float tMax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

		if (tMax < 0)
		{
			distance = tMax;
			return false;
		}

		if (tMin > tMax)
		{
			distance = tMax;
			return false;
		}

		if (distance > maxDistance)
		{
			return false;
		}

		distance = tMin;
		return true;
	}
	return false;
}