/**
	VividEngine.h

	Purpose: Class responsible for rendering all objects.

	@author Lein
	@version 1.0 12/20/19
*/
#pragma once
#include "AppHandle.h"
#include "Renderer.h"
#include "Manager.h"
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"
#include "SpriteRenderer.h"
#include "Renderer3D.h"

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