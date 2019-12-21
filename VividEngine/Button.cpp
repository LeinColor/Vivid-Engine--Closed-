#include "Button.h"

#include <functional>

void Button::OnClick(std::function<void(vivid::EventArgs args)> func)
{
	onClick = std::move(func);
}
void Button::OnDragStart(std::function<void(vivid::EventArgs args)> func)
{
	onDragStart = std::move(func);
}
void Button::OnDrag(std::function<void(vivid::EventArgs args)> func)
{
	onDrag = std::move(func);
}
void Button::OnDragEnd(std::function<void(vivid::EventArgs args)> func)
{
	onDragEnd = std::move(func);
}