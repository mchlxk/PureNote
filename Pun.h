#pragma once

#include <tuple>

#include "AccessTupleElement.h"

#include <QString>
#include <QSize>
#include <QRect>

#include "Style.h"

namespace Pun
{
	using T = std::tuple<QByteArray, style_t, float, bool, bool, bool, bool, QString>;

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


