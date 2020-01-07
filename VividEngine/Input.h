#pragma once

class Input
{
public:
	void Initialize();
	void ReadInput();
	bool ReadKeyboard();
	bool ReadMouse();

	// button means which mouse button will be used
	// 0: Left
	// 1: Right
	// 2: Wheel
	// 3~6: Additional Button

	// calls once when mouse button is pressed
	bool GetMouseButtonDown(BYTE button);

	// calls every frame while holding the button 
	bool GetMouseButton(BYTE button);

	// calls once when mouse button is released
	bool GetMouseButtonUp(BYTE button);
	
	// calls once when key button is pressed
	bool GetKeyDown(BYTE code);

	// calls every frame while holding key
	bool GetKey(BYTE code);

	// calls once when key button is released
	bool GetKeyUp(BYTE code);

	LONG GetMouseDx();
	LONG GetMouseDy();
	LONG GetMouseDz();
	MousePos GetMouseLocation();
	static float lZ;
private:
	void ProcessInput();

private:
	IDirectInput8* directInput = nullptr;
	IDirectInputDevice8* keyboard = nullptr;
	IDirectInputDevice8* mouse = nullptr;

	BYTE keys[256];
	bool isKeyPressed[256];
	DIMOUSESTATE mouseState;
	bool isMousePressed[8];
	int screenWidth = 0;
	int screenHeight = 0;
	int mouseX = 0;
	int mouseY = 0;
	int prevX = 0;
	int prevY = 0;
	int currX = 0;
	int currY = 0;
};