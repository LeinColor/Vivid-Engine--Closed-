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
	GraphicsAPI*			 m_Graphics;

	bool	 m_VSyncEnabled;
	uint32_t m_ScreenWidth;
	uint32_t m_ScreenHeight;
public:
	Renderer();
	void Initialize();
	inline uint32_t GetScreenWidth() { return m_ScreenWidth; }
	inline uint32_t GetScreenHeight() { return m_ScreenHeight; }
	
	void Render();
	void BeginScene(const float r, const float g, const float b, const float a);
	void EndScene();
};