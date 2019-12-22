#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
// Windows header file
#include <windows.h>
// C runtime header file
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// DirectX link library
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

// DirectX header file and namespace
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
using namespace DirectX;
using namespace DirectX::PackedVector;

#define ALIGNED_ALLOC_16 void* operator new(size_t size) { return _aligned_malloc(size, 16); } void operator delete(void* p) { _aligned_free(p); }
#define SAFE_RELEASE(p) { if( (p) ) { (p)->Release(); (p) = 0; } }
#define SAFE_DELETE(a) { delete(a); (a) = nullptr; }
#define SAFE_DELETE_ARRAY(a) { delete[] (a); (a) = nullptr; }

static const XMFLOAT4X4 IDENTITYMATRIX = XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

#define COMPONENTS Object, Component<Transform>, Component<Camera>, Component<SpriteRenderer>

namespace vivid {
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