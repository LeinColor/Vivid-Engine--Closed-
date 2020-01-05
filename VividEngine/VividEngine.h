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
#include "Debug.h"

using namespace vivid;

	class VividEngine {
	private:
		void Initialize();

	public:
		void Run();
		void Start();
		void SetWindow(HWND hWnd, HINSTANCE hInstance);
		void Update();
		void FixedUpdate();

	private:
		Renderer renderer;
		bool initialized = false;

		float deltaTimeAccumulator = 0;

		//TextureShader ts;
	};
