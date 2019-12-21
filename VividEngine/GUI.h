#pragma once
#include "stdafx.h"
#include "UI.h"

#include <string>
#include <vector>
namespace vivid {
	class GUI {
	public:
		GUI() {}
		~GUI() {}
		void AddItem(UI* item);
		void RemoveItem(UI* item);

	private:
		std::vector<UI*> items;
	};
}