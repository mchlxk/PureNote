#pragma once

#include <tuple>

#include <QString>
#include <QSize>
#include <QRect>

#include "Style.h"

namespace Pun
{
	namespace Geometry
	{
		using T = std::tuple<uint32_t, QSize, QRect>;

		// display_id, display_size, window_size
	}
	using geometry_t = Geometry::T;

	using T = std::tuple<geometry_t, style_t, float, bool, bool, bool, bool, QString>;

	// geometry
	// style
	// opacity
	// opaque_on_context
	// locked
	// on_top
	// fullscreen
	// content
}
using pun_t = Pun::T;


