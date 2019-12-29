/**
	Renderer.h

	Purpose: Class responsible for rendering all objects.

	@author Lein
	@version 1.0 12/20/19
*/
#pragma once
#include "DirectX11Wrapper.h"
#include "GUI.h"
#include "Manager.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Camera.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

namespace vivid {
	class Renderer {
	private:
		DirectX11Wrapper* dxWrapper = nullptr;

	public:
		GameObject* mainCamera = nullptr;
		DirectX11Wrapper* GetDevice();
		void SetDevice(DirectX11Wrapper* value);

		void Initialize(Manager<GameObject>& manager);
		void Render(Manager<GameObject>& manager);

		void LoadMesh(const char* fileName);
		void LoadShader();
		void LoadMaterial();
	};
}