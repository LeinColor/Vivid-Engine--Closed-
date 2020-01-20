/**
	GraphicsAPI.cpp

	Purpose: Wrapped to make it easier to use DirectX functions.

	@author Lein
	@version 1.0 12/20/19
*/
#include "GraphicsAPI.h"
#include "Debug.h"
#include "Shader.h"
#include "Manager.h"
using namespace vivid;


GraphicsAPI::GraphicsAPI(HWND hWnd, bool fullScreenFlag)
{
	m_FullScreen = fullScreenFlag;

	// Get screen size of the window to store in width, height variable.
	RECT rect = RECT();
	GetClientRect(hWnd, &rect);
	m_ScreenWidth = rect.right - rect.left;
	m_ScreenHeight = rect.bottom - rect.top;

	// declare HRESULT to store function return value.
	HRESULT hr = E_FAIL;

	// DirectX elements are initialized from below.
	// Get driver types and feature level window supports.
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	// Buffer desc -> Swap chain -> Back buffer -> Render target
	// Describe about buffer
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width = m_ScreenWidth;
	bufferDesc.Height = m_ScreenHeight;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Describe about swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;	// not multi-sampling
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = !m_FullScreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Create swap chain
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_DriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, m_DriverType, NULL, NULL, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext);

		if (SUCCEEDED(hr))
			break;
	}

	// Create back buffer
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_BackBuffer);

	// Create render target
	hr = m_Device->CreateRenderTargetView(m_BackBuffer, NULL, &m_RenderTargetView);

	// Bind the render target view to the output merger stage of the pipeline.
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, NULL);

	// depthBufferDesc
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = m_ScreenWidth;
	depthBufferDesc.Height = m_ScreenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	hr = m_Device->CreateTexture2D(&depthBufferDesc, NULL, &m_DepthStencilBuffer);

	// depthStencilDesc
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	hr = m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState);
	m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState, 1);

	// depthStencilView Desc
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = m_Device->CreateDepthStencilView(m_DepthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilView);
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	// rasterDesc
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	m_Device->CreateRasterizerState(&rasterDesc, &m_RasterState);
	m_DeviceContext->RSSetState(m_RasterState);

	// viewport
	D3D11_VIEWPORT viewport;
	viewport.Width = (float)m_ScreenWidth;
	viewport.Height = (float)m_ScreenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	m_DeviceContext->RSSetViewports(1, &viewport);

	// depthStencil Z-buffer off
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	hr = m_Device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_DepthDisabledStencilState);

	MessageBox(hWnd, L"Device Created", L"Notice", MB_OK);
}

void GraphicsAPI::UpdateBuffer(const ID3D11Buffer* buffer, const void* data, int dataLength)
{
	// Lock constant buffer to write description.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = m_DeviceContext->Map((ID3D11Resource*)buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (!SUCCEEDED(hr)) {
		Debug::Log("problem occured while updating buffer!");
	}
	memcpy(mappedResource.pData, data, dataLength);
	m_DeviceContext->Unmap((ID3D11Resource*)buffer, 0);
}

void GraphicsAPI::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4] = { red, green, blue, alpha };

	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, color);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void GraphicsAPI::CompileShaderFromFile(ShaderType type, const wchar_t* fileName, const char* entryPoint, ID3D10Blob** ppBlob)
{
	char target[7] = {};

	switch (type) {
	case VS:
		strcpy(target, "vs_5_0");
		break;
	case PS:
		strcpy(target, "ps_5_0");
		break;
	case GS:
		strcpy(target, "gs_5_0");
		break;
	case HS:
		strcpy(target, "hs_5_0");
		break;
	case DS:
		strcpy(target, "ds_5_0");
		break;
	case CS:
		strcpy(target, "cs_5_0");
		break;
	}

	HRESULT hr = D3DCompileFromFile(fileName, nullptr, nullptr, entryPoint, target, D3D10_SHADER_ENABLE_STRICTNESS, 0, ppBlob, nullptr);
	if (FAILED(hr))
	{
		Debug::Log("Shader compile failed!: %s", fileName);
		return;
	}
}

void GraphicsAPI::CreateVertexShader(const wchar_t* fileName, const char* entryPoint)
{
	VertexShader* vs = new VertexShader();
	ID3D10Blob* blob = nullptr;

	CompileShaderFromFile(ShaderType::VS, fileName, entryPoint, &blob);
	m_Device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vs->Get());
	Manager::vertexShaders.push_back(vs);

	blob->Release();
}

void GraphicsAPI::CreatePixelShader(const wchar_t* fileName, const char* entryPoint)
{
	PixelShader* ps = new PixelShader();
	ID3D10Blob* blob = nullptr;

	CompileShaderFromFile(ShaderType::PS, fileName, entryPoint, &blob);
	m_Device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &ps->Get());
	Manager::pixelShaders.push_back(ps);

	blob->Release();
}

void GraphicsAPI::EndScene()
{
	if (m_VSyncEnabled)
	{
		m_SwapChain->Present(1, 0);
	}
	else
	{
		m_SwapChain->Present(0, 0);
	}
}