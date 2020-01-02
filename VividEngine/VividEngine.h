/**
	VividEngine.h

	Purpose: Class responsible for rendering all objects.

	@author Lein
	@version 1.0 12/20/19
*/
#pragma once
#include "AppHandle.h"
#include "Renderer.h"
#include "Scene.h"
#include "Component.h"
#include "Object.h"
#include "Transform.h"
#include "SpriteRenderer.h"
#include "Renderer3D.h"
#include "Light.h"

using namespace vivid;
namespace vivid {
	class VividEngine {
	private:
		void Initialize();

	public:
		void Run();	
		void SetWindow(HWND hWnd, HINSTANCE hInstance);
		void Update();

	private:
		Renderer renderer;
		bool initialized = false;

		//TextureShader ts;
	};
}