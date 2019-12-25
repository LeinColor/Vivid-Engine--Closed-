/**
	DirectX11Wrapper.h

	Purpose: Wrapped to make it easier to use DirectX functions.

	@author Lein
	@version 1.0 12/18/19
*/

#pragma once
#include "stdafx.h"
#include "AppHandle.h"

namespace vivid {
	class DirectX11Wrapper {
	private:
		ID3D11Device*			device = nullptr;
		ID3D11DeviceContext*	deviceContext = nullptr;
		D3D_DRIVER_TYPE			driverType;
		D3D_FEATURE_LEVEL		featureLevel;
		IDXGISwapChain*			swapChain = nullptr;
		ID3D11Texture2D*		backBuffer = nullptr;
		ID3D11RenderTargetView* renderTargetView = nullptr;

		ID3D11Texture2D* depthStencilBuffer = nullptr;
		ID3D11DepthStencilState* depthStencilState = nullptr;
		ID3D11DepthStencilView* depthStencilView = nullptr;
		ID3D11RasterizerState* rasterState = nullptr;
		ID3D11DepthStencilState* depthDisabledStencilState = nullptr;

		int screenWidth;
		int screenHeight;
		bool VSyncEnabled = false;
		bool fullScreen = false;

	public:
		ALIGNED_ALLOC_16
		DirectX11Wrapper(HWND hWnd, bool fullScreenFlag);
		~DirectX11Wrapper() {}

		inline int GetScreenWidth() const { return screenWidth; }
		inline int GetScreenHeight() const { return screenHeight; }
		inline ID3D11DeviceContext* GetDeviceContext() const { return deviceContext; }
		inline ID3D11Device* GetDevice() const { return device; }

		HRESULT CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer);
		HRESULT CreateTexture1D(const D3D11_TEXTURE1D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture1D** ppTexture1D);
		HRESULT CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D);
		HRESULT CreateTexture3D(const D3D11_TEXTURE3D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture3D** ppTexture3D);
		HRESULT CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView);
		HRESULT CreateUnorderedAccessView(ID3D11Resource* pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView);
		HRESULT CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView);
		HRESULT CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout);
		HRESULT CreateVertexShader(ID3D10Blob* pShaderBytecode, ID3D11VertexShader** ppVertexShader);
		HRESULT CreatePixelShader(ID3D10Blob* pShaderBytecode, SIZE_T BytecodeLength, ID3D11PixelShader** ppPixelShader);
		HRESULT CreateGeometryShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11GeometryShader** ppGeometryShader);

		HRESULT CreateHullShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11HullShader** ppHullShader);
		HRESULT CreateDomainShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11DomainShader** ppDomainShader);
		HRESULT CreateComputeShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ComputeShader** ppComputeShader);
		HRESULT CreateBlendState(const D3D11_BLEND_DESC* pBlendStateDesc, ID3D11BlendState** ppBlendState);
		HRESULT CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState);
		HRESULT CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState);
		HRESULT CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState);
		HRESULT CreateQuery(const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery);
		HRESULT CreatePredicate(const D3D11_QUERY_DESC* pPredicateDesc, ID3D11Predicate** ppPredicate);
		HRESULT CreateCounter(const D3D11_COUNTER_DESC* pCounterDesc, ID3D11Counter** ppCounter);
		HRESULT CreateDeferredContext(UINT ContextFlags, ID3D11DeviceContext** ppDeferredContext);

		// IA Stage
		void IASetVertexBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets);
		void IASetIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset);
		void IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);

		void BeginScene(float, float, float, float);
		void EndScene();
		void TurnZBufferOn();
		void TurnZBufferOff();
	};
}