#pragma once

#include <tuple>

#include "ElementExtract.h"

#include <QString>
#include <QSize>
#include <QRect>

#include "Style.h"

namespace Pun
{
	using T = std::tuple<QByteArray, style_t, float, bool, bool, bool, bool, QString>;

	ELEMENT_EXTRACT(T, 0, geometry)
	ELEMENT_EXTRACT(T, 1, style)
	ELEMENT_EXTRACT(T, 2, opacity)
	ELEMENT_EXTRACT(T, 3, opaque_when_active)
	ELEMENT_EXTRACT(T, 4, locked)
	ELEMENT_EXTRACT(T, 5, on_top)
	ELEMENT_EXTRACT(T, 6, fullscreen)
	ELEMENT_EXTRACT(T, 7, content)
}
using pun_t = Pun::T;


