#pragma once

#include <tuple>

#include "ElementExtract.h"

#include <QByteArray>

#include "Style.h"

namespace Window
{
	using T = std::tuple<QByteArray, float, bool, bool, bool>;

	ELEMENT_EXTRACT(T, 0, geometry)
	ELEMENT_EXTRACT(T, 1, opacity)
	ELEMENT_EXTRACT(T, 2, opaque_when_active)
	ELEMENT_EXTRACT(T, 3, on_top)
	ELEMENT_EXTRACT(T, 4, fullscreen)

	extern const T defaults;

	static constexpr float min_opacity{ .15f };

	static inline float clamp_opacity(float value) { return std::max(min_opacity, std::min(1.f, value)); }

	bool equal_opacity(float a, float b);
	bool equal(const T& a, const T& b);
}
using window_t = Window::T;


