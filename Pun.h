#pragma once

#include <tuple>

#include "AccessTupleElement.h"

#include <QString>
#include <QSize>
#include <QRect>

#include "Style.h"

namespace Pun
{
	namespace Geometry
	{
		using T = std::tuple<uint32_t, QSize, QRect>;

		ACCESS_TUPLE_ELEMENT(T, 0, display_id)
		ACCESS_TUPLE_ELEMENT(T, 1, display_size)
		ACCESS_TUPLE_ELEMENT(T, 2, window_rect)
	}
	using geometry_t = Geometry::T;

	using T = std::tuple<geometry_t, style_t, float, bool, bool, bool, bool, QString>;

	ACCESS_TUPLE_ELEMENT(T, 0, geometry)
	ACCESS_TUPLE_ELEMENT(T, 1, style)
	ACCESS_TUPLE_ELEMENT(T, 2, opacity)
	ACCESS_TUPLE_ELEMENT(T, 3, opaque_on_context)
	ACCESS_TUPLE_ELEMENT(T, 4, locked)
	ACCESS_TUPLE_ELEMENT(T, 5, on_top)
	ACCESS_TUPLE_ELEMENT(T, 6, fullscreen)
	ACCESS_TUPLE_ELEMENT(T, 7, content)
}
using pun_t = Pun::T;


