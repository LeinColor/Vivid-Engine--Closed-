#include "stdafx.h"
#include "Input.h"
#include "AppHandle.h"
#include "Debug.h"

using namespace vivid;

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
}

bool Input::ReadKeyboard()
{
	HRESULT hr = keyboard->GetDeviceState(sizeof(key), (LPVOID)&key);
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

	if (mouseX < 0) mouseX = 0;
	if (mouseY < 0) mouseY = 0;

	if (mouseX > screenWidth) mouseX = screenWidth;
	if (mouseY > screenHeight) mouseY = screenHeight;
}

bool Input::GetKeyDown(BYTE code)
{
	if (key[code] & 0x80)
		return true;

	return false;
}

bool Input::IsEscapePressed()
{
	if (key[DIK_ESCAPE] & 0x80)
	{
		return true;
	}
	return false;
}

MousePos Input::GetMouseLocation()
{
	return MousePos{ mouseX, mouseY };
}