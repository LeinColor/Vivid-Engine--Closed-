/**
	Renderer.h

	Purpose: Class responsible for rendering all objects.

	@author Lein
	@version 1.0 12/20/19
*/
#pragma once
#include "GraphicsAPI.h"

class Renderer {
private:
	GraphicsAPI* graphics;

	bool	 VSyncEnabled;
	uint32_t screenWidth;
	uint32_t screenHeight;
public:
	Renderer() {}
	~Renderer() {}
	void Initialize();
	void Shutdown();
	void Render();
	inline GraphicsAPI* GetGraphicsAPI() { return graphics; }
	inline uint32_t GetScreenWidth() { return screenWidth; }
	inline uint32_t GetScreenHeight() { return screenHeight; }
	
	void BeginScene(const float r, const float g, const float b, const float a);
	void EndScene();
};