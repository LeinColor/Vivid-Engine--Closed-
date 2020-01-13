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
#include "Renderer3D.h"
#include "Light.h"
#include "Input.h"
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
		Input input;
		Renderer renderer;
		bool initialized = false;
		
		float deltaTimeAccumulator = 0;

		unordered_map<int, Object*> editorObjects;

		enum EDITOR_OBJECT {
			EDITOR_CONE_X,
			EDITOR_CONE_Y,
			EDITOR_CONE_Z,
			EDITOR_ROTATION_X,
			EDITOR_ROTATION_Y,
			EDITOR_ROTATION_Z,
			EDITOR_CUBE_X,
			EDITOR_CUBE_Y,
			EDITOR_CUBE_Z,
		};


		//TextureShader ts;
	};
