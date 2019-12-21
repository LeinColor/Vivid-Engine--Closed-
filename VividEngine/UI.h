#pragma once
#include "stdafx.h"

#include <string>

using namespace vivid;

namespace vivid {
	class UI {
	protected:
		std::string name;
		std::string text;
		XMFLOAT2 pos;
		XMFLOAT2 size;
		float width;
		float height;
		bool visible;
		bool enabled;

	public:
		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual ~UI() = default;

	public:
		enum class STATE {
			NORMAL,
			FOCUS,
			MOUSEON,
			MOUSEOUT,
		};

		void SetName(const std::string& value);
		void SetText(const std::string& value);
		void SetPos(float x, float y);
		void SetSize(float w, float h);
		void SetVisible(bool value);
		void SetEnabled(bool value);

		const std::string& GetName() const;
		const std::string& GetText() const;
		const XMFLOAT2& GetPos() const;
		const XMFLOAT2& GetSize() const;
		bool isVIsible() const;
		bool isEnabled() const;
	};
}