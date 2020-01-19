/**
	GraphicsAPI.h

	Purpose: Wrapped to make it easier to use DirectX functions.

	@author Lein
	@version 1.0 12/18/19
*/

#pragma once
#include "stdafx.h"
#include "AppHandle.h"


// 1. 서브쉐이더 전부 통합해서 하나로 만드는 작업
// 2. Vertex Buffer, Index Buffer 처리
// 3. Input Layout 처리
// 4. Constant Buffer 처리
// 11.0 우선 작업. 11.1은 추후
class GraphicsAPI {
public:
	GraphicsAPI();
	~GraphicsAPI();
	bool Initialize(HWND hWnd, const bool VSyncFlag, const bool fullScreenFlag);
	void Shutdown();
	inline IDXGISwapChain*		GetSwapChain() { return m_SwapChain; }
	inline ID3D11Device*		GetDevice() { return m_Device; }
	inline ID3D11DeviceContext* GetDeviceContext() { return m_DeviceContext; }
	inline bool					GetVSyncEnabled() { return m_VSyncEnabled; }
	inline uint32_t				GetScreenWidth() { return m_ScreenWidth; }
	inline uint32_t				GetScreenHeight() { return m_ScreenHeight; }
private:
	bool InitSwapChain(HWND hWnd, uint32_t numerator, uint32_t denominator);
private:
	bool				 m_VSyncEnabled;
	bool				 m_FullScreen;
	uint32_t			 m_ScreenWidth;
	uint32_t			 m_ScreenHeight;
	int					 m_VRAM;
	char				 m_GPUDescription[128];

	IDXGISwapChain*		 m_SwapChain;

	ID3D11Device*		 m_Device;
	ID3D11DeviceContext* m_DeviceContext;
};