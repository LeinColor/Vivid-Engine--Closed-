/**
	Renderer.cpp

	Purpose: Class responsible for rendering all objects.

	@author Lein
	@version 1.0 12/20/19
*/
#include "stdafx.h"
#include "Renderer.h"
#include "Descriptor.h"
#include "Debug.h"
using namespace vivid;
using namespace std;

// Initialize all pointer
Renderer::Renderer()
{	
}

void Renderer::Initialize()
{
	m_Graphics = new GraphicsAPI();
	if (!m_Graphics)
	{
		Debug::Log("Cannot create directX device!");
		return;
	}

	bool result = m_Graphics->Initialize(AppHandle::GetWindowHandle(), true, false);

	m_Device = m_Graphics->GetDevice();
	m_DeviceContext = m_Graphics->GetDeviceContext();
	m_ScreenWidth = m_Graphics->GetScreenWidth();
	m_ScreenHeight = m_Graphics->GetScreenHeight();
	m_VSyncEnabled = m_Graphics->GetVSyncEnabled();

	ID3D11Texture2D* backBufferPtr = nullptr;
	HRESULT hr = m_Graphics->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	
	if (FAILED(hr))
	{
		Debug::Log("Cannot get back buffer pointer");
		return;
	}
	
	m_Device->CreateRenderTargetView(backBufferPtr, NULL, &m_RenderTargetView);
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

	D3D11_VIEWPORT viewport;
	viewport.Width = (float)m_ScreenWidth;
	viewport.Height = (float)m_ScreenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	m_DeviceContext->RSSetViewports(1, &viewport);

}

void Renderer::Render()
{
	BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	EndScene();
}

void Renderer::BeginScene(const float r, const float g, const float b, const float a)
{
	float color[4] = { r, g, b, a };
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, color);

	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::EndScene()
{
	if (m_VSyncEnabled)
	{
		m_Graphics->GetSwapChain()->Present(1, 0);
	}
	else
	{
		m_Graphics->GetSwapChain()->Present(0, 0);
	}
}