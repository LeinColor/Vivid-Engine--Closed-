#include "stdafx.h"
#include "AABB.h"
#include "Maths.h"
using namespace vivid;

AABB AABB::Transform(const XMMATRIX& mat)
{
	XMFLOAT3 vertices[8];
	for (int i = 0; i < 8; i++) {
		XMVECTOR vertex = XMVector3Transform(XMLoadFloat3(&GetVertex(i)), mat);
		XMStoreFloat3(&vertices[i], vertex);
	}

	XMFLOAT3 min = vertices[0];
	XMFLOAT3 max = vertices[6];
	for (int i = 0; i < 8; i++) {
		const XMFLOAT3& p = vertices[i];

		if (p.x < min.x) min.x = p.x;
		if (p.y < min.y) min.y = p.y;
		if (p.z < min.z) min.z = p.z;

		if (p.x > max.x) max.x = p.x;
		if (p.y > max.y) max.y = p.y;
		if (p.z > max.z) max.z = p.z;
	}
	return AABB(min, max);
}

bool AABB::Overlaps(const AABB& other) const
{
	return maxPos.x > other.minPos.x &&
		   minPos.x < other.maxPos.x &&
		   maxPos.y > other.minPos.y &&
		   minPos.y < other.maxPos.y &&
		   maxPos.z > other.minPos.z &&
		   minPos.z < other.maxPos.z;
}

bool AABB::Contains(const AABB& other) const
{
	return minPos.x <= other.minPos.x &&
		   maxPos.x >= other.maxPos.x &&
		   minPos.y <= other.minPos.y &&
		   maxPos.y >= other.maxPos.y &&
		   minPos.z <= other.minPos.z &&
		   maxPos.z >= other.maxPos.z;

}