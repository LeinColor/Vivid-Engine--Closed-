#pragma once

class Input
{
public:
	void Initialize();
	void ReadInput();
	bool ReadKeyboard();
	bool ReadMouse();
	bool GetMouseButtonDown(int button);
	bool GetMouseButton(int button);
	bool GetMouseButtonUp(int button);
	bool GetKeyDown(BYTE code);
	bool IsEscapePressed();
	MousePos GetMouseLocation();
private:
	void ProcessInput();

private:
	IDirectInput8* directInput = nullptr;
	IDirectInputDevice8* keyboard = nullptr;
	IDirectInputDevice8* mouse = nullptr;

	unsigned char key[256];
	DIMOUSESTATE mouseState;
	int screenWidth = 0;
	int screenHeight = 0;
	int mouseX = 0;
	int mouseY = 0;
};