#include "stdafx.h"
#include "Physics.h"
#include "Ray.h"

bool Physics::Raycast(XMVECTOR origin,
		XMVECTOR direction,
		_Out_ RaycastHit hitInfo,
		float maxDistance,
		int layerMask)
{
	XMVECTOR dir = XMVector3Normalize(direction - origin);
	XMVECTOR rayVector = maxDistance * dir;


}