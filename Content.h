#pragma once

#include <tuple>
#include <QString>

#include "ElementExtract.h"

namespace Content
{
	using T = std::tuple<bool, QString>;

	ELEMENT_EXTRACT(T, 0, locked)
	ELEMENT_EXTRACT(T, 1, text)

	extern const T defaults;
}
using content_t = Content::T;


