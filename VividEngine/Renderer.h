/**
	Renderer.h

	Purpose: Class responsible for rendering all objects.

	@author Lein
	@version 1.0 12/20/19
*/
#pragma once
#include "DirectX11Wrapper.h"
#include "GUI.h"
#include "Scene.h"
#include "Object.h"
#include "SpriteRenderer.h"
#include "Camera.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"

namespace vivid {
	class Renderer {
	private:
		DirectX11Wrapper* dxWrapper = nullptr;

	public:
		Object* mainCamera = nullptr;
		DirectX11Wrapper* GetDevice();
		void SetDevice(DirectX11Wrapper* value);

		void Initialize();
		void Render();

		void LoadMesh(const char* fileName);
		void LoadShader();
		void LoadMaterial();
		void DrawDebug();
		void LoadLine();
		void AddGizmoLine(const GizmoLine& line);
		void AddGizmoCone();
		void DrawLine(XMFLOAT3 startPoint, XMFLOAT3 endPoint, XMFLOAT4 color);
	};
}