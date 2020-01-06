#pragma once

class AABB {
private:

public:
	XMFLOAT3 minPos;
	XMFLOAT3 maxPos;

	AABB() {}
	AABB(const XMFLOAT3& minValue, const XMFLOAT3& maxValue) : minPos(minValue), maxPos(maxValue) {}

	AABB Transform(const XMMATRIX& matrix);
	bool Overlaps(const AABB& other) const;
	bool Contains(const AABB& other) const;

	inline XMFLOAT3 GetVertex(int index) const
	{
		switch (index)
		{
		case 0:
			return minPos;
		case 1:
			return XMFLOAT3(minPos.x, maxPos.y, minPos.z);
		case 2:
			return XMFLOAT3(minPos.x, maxPos.y, maxPos.z);
		case 3:
			return XMFLOAT3(minPos.x, minPos.y, maxPos.z);
		case 4:
			return XMFLOAT3(maxPos.x, minPos.y, minPos.z);
		case 5:
			return XMFLOAT3(maxPos.x, maxPos.y, minPos.z);
		case 6:
			return maxPos;
		case 7:
			return XMFLOAT3(maxPos.x, minPos.y, maxPos.z);
		}
		return XMFLOAT3(0, 0, 0);
	}
};