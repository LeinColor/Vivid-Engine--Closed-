#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
// C runtime header file
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <algorithm>
// Windows header file
#include <windows.h>

// DirectX link library
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
// DirectX header file and namespace
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <dinput.h>
#include "AppHandle.h"
#include "Descriptor.h"
#include "ConstantBufferStructs.h"
using namespace DirectX;
using namespace DirectX::PackedVector;

#define ALIGNED_ALLOC_16 void* operator new(size_t size) { return _aligned_malloc(size, 16); } void operator delete(void* p) { _aligned_free(p); }
#define SAFE_RELEASE(p) { if( (p) ) { (p)->Release(); (p) = 0; } }
#define SAFE_DELETE(a) { delete(a); (a) = nullptr; }
#define SAFE_DELETE_ARRAY(a) { delete[] (a); (a) = nullptr; }
#define NOMINMAX

static const XMFLOAT4X4 IDENTITYMATRIX = XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
static const XMVECTOR axisX = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
static const XMVECTOR axisY = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
static const XMVECTOR axisZ = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

namespace vivid {
	// Define structs here
	struct Rect {
		int32_t xMin;
		int32_t xMax;
		int32_t yMin;
		int32_t yMax;
	};

	struct EventArgs {
		XMFLOAT2 clickPos;
		XMFLOAT2 startPos;
		XMFLOAT2 deltaPos;
		XMFLOAT2 endPos;
	};
}