#pragma once

class Physics {
public:
	static bool Raycast(XMVECTOR origin, XMVECTOR direction, _Out_ RaycastHit hitInfo, float maxDistance, int layerMask);

private:
};