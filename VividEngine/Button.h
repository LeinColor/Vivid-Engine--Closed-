#pragma once
#include "stdafx.h"
#include "UI.h"

#include <string>
#include <functional>

using namespace vivid;

namespace vivid {
	class Button : public UI {
	protected:
		std::function<void(EventArgs args)> onClick;
		std::function<void(EventArgs args)> onDragStart;
		std::function<void(EventArgs args)> onDrag;
		std::function<void(EventArgs args)> onDragEnd;
		XMFLOAT2 dragStart;
		XMFLOAT2 prevPos;

	public:
		virtual void Update() override;
		virtual void Render() override;

	public:
		void OnClick(std::function<void(EventArgs args)> func);
		void OnDragStart(std::function<void(EventArgs args)> func);
		void OnDrag(std::function<void(EventArgs args)> func);
		void OnDragEnd(std::function<void(EventArgs args)> func);
	};
}