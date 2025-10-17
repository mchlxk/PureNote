
#include "Window.h"

const Window::T Window::defaults = { {}, 1.f, false, false, false };

bool Window::equal_opacity(float a, float b)
{
	return std::fabs(a-b) < .02f;
}

bool Window::equal(const T& a, const T& b)
{
	if (geometry(a) != geometry(b))
		return false;
	if (!equal_opacity(opacity(a), opacity(b)))
		return false;
	if (opaque_when_active(a) != opaque_when_active(b))
		return false;
	if(on_top(a) != on_top(b))
		return false;
	if(fullscreen(a) != fullscreen(b))
		return false;
	return true;
}

