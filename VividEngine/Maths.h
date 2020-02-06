#pragma once

namespace viMath
{
	inline XMFLOAT3 Max(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return XMFLOAT3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
	}
	
	inline XMFLOAT3 Min(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return XMFLOAT3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
	}
}