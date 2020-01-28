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

class MeshComponent;

class GraphicsAPI {
private:
	ID3D11Device*			 device = nullptr;
	ID3D11DeviceContext*	 deviceContext = nullptr;
	D3D_DRIVER_TYPE			 driverType;
	D3D_FEATURE_LEVEL		 featureLevel;
	IDXGISwapChain*			 swapChain = nullptr;
	ID3D11Texture2D*		 backBuffer = nullptr;
	ID3D11RenderTargetView*  renderTargetView = nullptr;

	ID3D11Texture2D*		 depthStencilBuffer = nullptr;
	ID3D11DepthStencilState* depthStencilState = nullptr;
	ID3D11DepthStencilView*	 depthStencilView = nullptr;
	ID3D11RasterizerState*	 rasterState = nullptr;
	ID3D11DepthStencilState* depthDisabledStencilState = nullptr;

	int screenWidth;
	int screenHeight;
	bool VSyncEnabled = false;
	bool fullScreen = false;

public:
	ALIGNED_ALLOC_16
	GraphicsAPI(HWND hWnd, bool fullScreenFlag);
	~GraphicsAPI() {}

	inline int GetScreenWidth() const { return screenWidth; }
	inline int GetScreenHeight() const { return screenHeight; }

	inline ID3D11Device* GetDevice() const { return device; }
	inline ID3D11DeviceContext* GetContext() const { return deviceContext; }

	void CreateBuffer(D3D11_BUFFER_DESC* pDesc, D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer);
	void CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT numElements, const void* pShaderByteCode , SIZE_T byteCodeLength, ID3D11InputLayout** ppInputLayout);
	void CreateVertexShader(const void *pShaderByteCode, SIZE_T byteCodeLength, ID3D11VertexShader** ppVertexShader);
	void CreatePixelShader(const void *pShaderByteCode, SIZE_T byteCodeLength, ID3D11PixelShader** ppPixelShader);
	void CreateGeometryShader(const void *pShaderByteCode, SIZE_T byteCodeLength, ID3D11GeometryShader** ppGeometryShader);
	void CreateHullShader(const void *pShaderByteCode, SIZE_T byteCodeLength, ID3D11HullShader** ppHullShader);
	void CreateDomainShader(const void *pShaderByteCode, SIZE_T byteCodeLength, ID3D11DomainShader** ppDomainShader);
	void CreateComputeShader(const void *pShaderByteCode, SIZE_T byteCodeLength, ID3D11ComputeShader** ppComputeShader);

	void UpdateConstantBuffer(const ID3D11Buffer* buffer, const void* data, int dataLength);

	void BeginScene(float, float, float, float);
	void EndScene();
};