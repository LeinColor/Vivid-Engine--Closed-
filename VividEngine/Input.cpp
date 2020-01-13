#include "stdafx.h"
#include "Input.h"
#include "AppHandle.h"
#include "Debug.h"

using namespace vivid;

float Input::lZ;
void Input::Initialize()
{
	HWND hWnd = AppHandle::GetWindowHandle();
	HINSTANCE hInst = AppHandle::GetInstanceHandle();

	RECT rect = RECT();
	GetClientRect(hWnd, &rect);
	screenWidth = rect.right - rect.left;
	screenHeight = rect.bottom - rect.top;

	HRESULT hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, NULL);
	
	directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	keyboard->SetDataFormat(&c_dfDIKeyboard);
	keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	keyboard->Acquire();

	directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	mouse->SetDataFormat(&c_dfDIMouse);
	mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	mouse->Acquire();
}

void Input::ReadInput()
{
	if (!ReadKeyboard())
	{
		vivid::Debug::Log("Input keyboard error occured!");
		return;
	}
	if (!ReadMouse())
	{
		vivid::Debug::Log("Input mouse error occured!");
		return;
	}

	ProcessInput();
}

bool Input::ReadKeyboard()
{
	HRESULT hr = keyboard->GetDeviceState(sizeof(keys), (LPVOID)&keys);
	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			keyboard->Acquire();
		}
		else {
			return false;
		}
	}
	return true;
}

bool Input::ReadMouse()
{
	HRESULT hr = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			mouse->Acquire();
		}
		else {
			return false;
		}
	}
	return true;
}

void Input::ProcessInput()
{
	mouseX += mouseState.lX;
	mouseY += mouseState.lY;
}

LONG Input::GetMouseDx()
{
	return mouseState.lX;
}

LONG Input::GetMouseDy()
{
	return mouseState.lY;
}

LONG Input::GetMouseDz()
{
	return mouseState.lZ;
}

// calls once when mouse button is pressed
bool Input::GetMouseButtonDown(BYTE code)
{
	if (!isMousePressed[code] && (mouseState.rgbButtons[code] & 0x80)) {
		isMousePressed[code] = true;
		return true;
	}
	if (isMousePressed[code] && !(mouseState.rgbButtons[code] & 0x80))
		isMousePressed[code] = false;

	return false;
}

// calls every frame while holding the button 
bool Input::GetMouseButton(BYTE code)
{
	if (mouseState.rgbButtons[code] & 0x80) {
		isMousePressed[code] = true;
		return true;
	}
	isMousePressed[code] = false;

	return false;
}

// calls once when mouse button is released
bool Input::GetMouseButtonUp(BYTE code)
{
	if (isMousePressed[code] && !(mouseState.rgbButtons[code] & 0x80)) {
		isMousePressed[code] = false;
		return true;
	}
	if (!isMousePressed[code] && (mouseState.rgbButtons[code] & 0x80))
		isMousePressed[code] = true;

	return false;
}

// calls once when key button is pressed
bool Input::GetKeyDown(BYTE code)
{
	if (!isKeyPressed[code] && (keys[code] & 0x80)) {
		isKeyPressed[code] = true;
		return true;
	}
	if (isKeyPressed[code] && !(keys[code] & 0x80))
		isKeyPressed[code] = false;

	return false;
}

// calls every frame while holding key
bool Input::GetKey(BYTE code)
{
	if (keys[code] & 0x80) {
		isKeyPressed[code] = true;
		return true;
	}
	isKeyPressed[code] = false;

	return false;
}

// calls once when key button is released
bool Input::GetKeyUp(BYTE code)
{
	if (isKeyPressed[code] && !(keys[code] & 0x80)) {
		isKeyPressed[code] = false;
		return true;
	}
	if (!isKeyPressed[code] && (keys[code] & 0x80))
		isKeyPressed[code] = true;

	return false;
}

XMFLOAT2 Input::GetMouseLocation()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(AppHandle::GetWindowHandle(), &p);
	return XMFLOAT2((float)p.x, (float)p.y);
}