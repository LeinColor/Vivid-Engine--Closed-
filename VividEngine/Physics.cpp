#include "stdafx.h"
#include "Physics.h"
#include "Ray.h"
#include "AABB.h"
#include "Object.h"
#include "Mesh.h"
#include "Transform.h"
#include "Renderer3D.h"
#include "Scene.h"

bool Physics::Raycast(XMVECTOR origin,
		XMVECTOR direction,
		_Out_ RaycastHit& hitInfo,
		float maxDistance,
		int layerMask)
{
	float distance = 0.0f;
	XMFLOAT3 org;
	XMFLOAT3 dirInv;
	XMStoreFloat3(&org, origin);
	XMStoreFloat3(&dirInv, direction);
	dirInv.x = 1.0f / dirInv.x;
	dirInv.y = 1.0f / dirInv.y;
	dirInv.z = 1.0f / dirInv.z;
	for (int i = 0; i < Scene::objects.size(); i++) {
		auto& renderer3D = Scene::objects[i]->GetComponent<Renderer3D>();
		if (&renderer3D == nullptr)
			continue;

		auto& mesh = renderer3D.mesh;
		if (mesh == nullptr)
			continue;

		AABB& aabb = renderer3D.mesh->aabb.Transform(Scene::objects[i]->GetComponent<Transform>().GetWorldMatrix());
		XMFLOAT3& lb = aabb.minPos;
		XMFLOAT3& rt = aabb.maxPos;

		float t1 = (lb.x - org.x) * dirInv.x;
		float t2 = (rt.x - org.x) * dirInv.x;
		float t3 = (lb.y - org.y) * dirInv.y;
		float t4 = (rt.y - org.y) * dirInv.y;
		float t5 = (lb.z - org.z) * dirInv.z;
		float t6 = (rt.z - org.z) * dirInv.z;

		float tMin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
		float tMax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

		if (tMax < 0 || tMin > tMax || tMin > maxDistance)
		{
			continue;
		}

		hitInfo.index = i;
		return true;
	}
	return false;
}