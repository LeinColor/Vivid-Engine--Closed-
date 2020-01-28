/**
	Renderer.h

	Purpose: Class responsible for rendering all objects.

	@author Lein
	@version 1.0 12/20/19
*/
#pragma once
#include "GraphicsAPI.h"
#include "Scene.h"
#include "Components.h"

class Renderer {
private:
	static GraphicsAPI* graphics;

	bool	 VSyncEnabled;
	uint32_t screenWidth;
	uint32_t screenHeight;

	Scene* scene;

public:
	Renderer() {}
	~Renderer() {}
	void Initialize();
	void Shutdown();
	void Render();

	inline void SetScene(Scene* value) { scene = value; }

	void InitializeConstantBuffers();
	void UpdateVertexBuffer(MeshComponent& mesh);
	void UpdateIndexBuffer(MeshComponent& mesh);
	void UpdateInputLayout(ShaderComponent& shader);
	void UpdateShader(ShaderComponent& shader);
	void Apply();

	inline static GraphicsAPI* GetGraphicsAPI() { return graphics; }
	inline uint32_t GetScreenWidth() { return screenWidth; }
	inline uint32_t GetScreenHeight() { return screenHeight; }
	
	void BeginScene(const float r, const float g, const float b, const float a);
	void EndScene();
};