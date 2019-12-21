/**
	Renderer.h

	Purpose: Class responsible for rendering all objects.

	@author Lein
	@version 1.0 12/20/19
*/
#pragma once
#include "DirectX11Wrapper.h"
#include "GUI.h"

namespace vivid {
	class Renderer {
	private:
		DirectX11Wrapper* device = nullptr;

	public:
		DirectX11Wrapper* GetDevice();
		void SetDevice(DirectX11Wrapper* value);
	};
}