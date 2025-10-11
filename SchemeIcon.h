#pragma once

#include <QIcon>

#include "ColorScheme.h"

namespace SchemeIcon
{
	QIcon get(const color_scheme_t& scheme, const uint32_t size);
}

