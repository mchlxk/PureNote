#pragma once

#include <QIcon>

#include "ColorScheme.h"

namespace SchemeIcon
{
	QIcon get(const color_scheme_t& scheme, const uint32_t size);
	QPixmap get_warning(const color_scheme_t& scheme, const uint32_t size);
}

