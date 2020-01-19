/**
	GraphicsAPI.cpp

	Purpose: Wrapped to make it easier to use DirectX functions.

	@author Lein
	@version 1.0 12/20/19
*/
#include "stdafx.h"
#include "GraphicsAPI.h"
#include "Debug.h"
using namespace vivid;

GraphicsAPI::GraphicsAPI() :
	m_VSyncEnabled(true),
	m_ScreenWidth(0),
	m_ScreenHeight(0),
	m_SwapChain(nullptr),
	m_Device(nullptr),
	m_DeviceContext(nullptr)
{}

GraphicsAPI::~GraphicsAPI() {}

bool GraphicsAPI::Initialize(HWND hWnd, const bool VSyncFlag, const bool fullScreenFlag)
{
	RECT rect = RECT();
	GetClientRect(hWnd, &rect);
	m_ScreenWidth = rect.right - rect.left;
	m_ScreenHeight = rect.bottom - rect.top;

	HRESULT result;
	IDXGIFactory* pFactory;
	IDXGIAdapter* pAdapter;
	IDXGIOutput* pAdapterOutput;
	uint32_t numModes;

	// Store the vsync setting.
	m_VSyncEnabled = VSyncFlag;

	// Store the full screen setting.
	m_FullScreen = fullScreenFlag;

	// Get System Information
	//----------------------------------------------------------------------------------

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = pFactory->EnumAdapters(0, &pAdapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = pAdapter->EnumOutputs(0, &pAdapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	DXGI_MODE_DESC* displayModeList;
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = pAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	uint32_t numerator = 0;
	uint32_t denominator = 0;
	for (size_t i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (uint32_t)m_ScreenWidth)
		{
			if (displayModeList[i].Height == (uint32_t)m_ScreenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	if (numerator == 0 && denominator == 0)
	{
		char info[127];
		numerator = displayModeList[numModes / 2].RefreshRate.Numerator;
		denominator = displayModeList[numModes / 2].RefreshRate.Denominator;
		sprintf_s(info, "Specified resolution (%ux%u) not found: Using (%ux%u) instead\n",
			m_ScreenWidth, m_ScreenHeight,
			displayModeList[numModes / 2].Width, displayModeList[numModes / 2].Height);
		m_ScreenWidth = displayModeList[numModes / 2].Width;
		m_ScreenHeight = displayModeList[numModes / 2].Height;

		// also resize window
		//SetWindowPos(hWnd, 0, 10, 10, width, height, SWP_NOZORDER);
	}

	// Get the adapter (video card) description.
	DXGI_ADAPTER_DESC adapterDesc;
	result = pAdapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	m_VRAM = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	size_t stringLength;
	int error;
	error = wcstombs_s(&stringLength, m_GPUDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	// Release memory
	delete[] displayModeList;		displayModeList = nullptr;
	pAdapterOutput->Release();		pAdapterOutput = nullptr;
	pAdapter->Release();			pAdapter = nullptr;
	pFactory->Release();			pFactory = nullptr;

	if (!InitSwapChain(hWnd, numerator, denominator))
	{
		return false;
	}
	return true;
}

bool GraphicsAPI::InitSwapChain(HWND hWnd, uint32_t numerator, uint32_t denominator)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 3;
	swapChainDesc.BufferDesc.Width = m_ScreenWidth;
	swapChainDesc.BufferDesc.Height = m_ScreenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb173064(v=vs.85).aspx
	if (m_VSyncEnabled)
	{	// Set the refresh rate of the back buffer.
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	swapChainDesc.OutputWindow = hWnd;	// Set the handle for the window to render to.
	swapChainDesc.Windowed = !m_FullScreen;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = 0;
	featureLevel = D3D_FEATURE_LEVEL_11_1;

	UINT flags = 0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		flags,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_SwapChain,
		&m_Device, NULL, &m_DeviceContext);

	if (FAILED(result))
	{
		Debug::Log("Cannot create swap chain!");
		return false;
	}
	return true;
}

void GraphicsAPI::Shutdown()
{
	if (m_SwapChain)
		m_SwapChain->SetFullscreenState(false, NULL);

	SAFE_RELEASE(m_SwapChain);
	SAFE_RELEASE(m_Device);
	SAFE_RELEASE(m_DeviceContext);
}

