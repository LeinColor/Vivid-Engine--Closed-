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

#include "Resources.h"
#include "Mesh.h"
#include "Shader.h"

#include "../ThirdParty/entt/entt.hpp"

class Renderer {
private:
	static GraphicsAPI* graphics;

	bool	 VSyncEnabled;
	uint32_t screenWidth;
	uint32_t screenHeight;

	entt::entity enttMainCamera;

	Scene* scene;

	XMMATRIX world, view, projection;

public:
	Renderer() {}
	~Renderer() {}
	void Initialize();
	void Shutdown();
	void Render();

	inline void SetScene(Scene* value) { scene = value; }
	inline void SetMainCamera(entt::entity entity) { enttMainCamera = entity; }

	void UpdatePipeline(Transform& transform, Renderer3D& renderer3D);

	void InitializeConstantBuffers();
	void UpdateVertexBuffer(Mesh& mesh);
	void UpdateIndexBuffer(Mesh& mesh);
	void UpdateInputLayout(Shader& shader);
	void UpdateShader(Shader& shader);
	void Apply();

	void UpdateConstantBuffers(TransformConstants& transform);

	inline static GraphicsAPI* GetGraphicsAPI() { return graphics; }
	inline uint32_t GetScreenWidth() { return screenWidth; }
	inline uint32_t GetScreenHeight() { return screenHeight; }
};