#include "UI.h"

using namespace vivid;

void UI::SetName(const std::string& value)
{
	name = value;
}

void UI::SetText(const std::string& value)
{
	text = value;
}

void UI::SetPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
}

void UI::SetSize(float w, float h)
{
	size.x = w;
	size.y = h;
}

void UI::SetVisible(bool value)
{
	visible = value;
}

void UI::SetEnabled(bool value)
{
	enabled = value;
}

const std::string& UI::GetName() const
{
	return name;
}

const std::string& UI::GetText() const
{
	return text;
}

const XMFLOAT2& UI::GetPos() const
{
	return pos;
}

const XMFLOAT2& UI::GetSize() const
{
	return size;
}

bool UI::isVIsible() const
{
	return visible;
}

bool UI::isEnabled() const
{
	return enabled;
}