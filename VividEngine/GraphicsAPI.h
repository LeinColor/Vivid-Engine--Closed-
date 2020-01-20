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
private:
	ID3D11Device*			 m_Device = nullptr;
	ID3D11DeviceContext*	 m_DeviceContext = nullptr;
	D3D_DRIVER_TYPE			 m_DriverType;
	D3D_FEATURE_LEVEL		 m_FeatureLevel;
	IDXGISwapChain*			 m_SwapChain = nullptr;
	ID3D11Texture2D*		 m_BackBuffer = nullptr;
	ID3D11RenderTargetView*  m_RenderTargetView = nullptr;

	ID3D11Texture2D*		 m_DepthStencilBuffer = nullptr;
	ID3D11DepthStencilState* m_DepthStencilState = nullptr;
	ID3D11DepthStencilView*	 m_DepthStencilView = nullptr;
	ID3D11RasterizerState*	 m_RasterState = nullptr;
	ID3D11DepthStencilState* m_DepthDisabledStencilState = nullptr;

	int m_ScreenWidth;
	int m_ScreenHeight;
	bool m_VSyncEnabled = false;
	bool m_FullScreen = false;

public:
	ALIGNED_ALLOC_16
	GraphicsAPI(HWND hWnd, bool fullScreenFlag);
	~GraphicsAPI() {}

	inline int GetScreenWidth() const { return m_ScreenWidth; }
	inline int GetScreenHeight() const { return m_ScreenHeight; }

	inline ID3D11Device* GetDevice() const { return m_Device; }
	inline ID3D11DeviceContext* GetContext() const { return m_DeviceContext; }

	void CreateVertexShader(const wchar_t* fileName, const char* entryPoint);
	void CreatePixelShader(const wchar_t* fileName, const char* entryPoint);

	void CompileShaderFromFile(ShaderType, const wchar_t* fileName, const char* entryPoint, ID3D10Blob** ppBlob);
	void UpdateBuffer(const ID3D11Buffer* buffer, const void* data, int dataLength);

	void BeginScene(float, float, float, float);
	void EndScene();
};