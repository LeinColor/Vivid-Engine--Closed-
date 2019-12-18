#pragma once
#include "DirectX11Wrapper.h"

namespace vivid {
	class Renderer {
	private:
		DirectX11Wrapper* device = nullptr;

	public:
		DirectX11Wrapper* GetDevice();
		void SetDevice(DirectX11Wrapper* value);
	};
}