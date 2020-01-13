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
#include "Camera.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"

namespace vivid {
	class Renderer {
	private:
		static DirectX11Wrapper* graphics;

	public:
		Object* mainCamera = nullptr;
		static DirectX11Wrapper* GetGraphicsInterface();
		static void SetGraphicsInterface(DirectX11Wrapper* value);
		static inline int GetScreenWidth() { return graphics->GetScreenWidth(); }
		static inline int GetScreenHeight() { return graphics->GetScreenHeight(); }

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