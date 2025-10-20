#pragma once

#include <QIcon>

#include "ColorScheme.h"

namespace SchemeIcon
{
	QPixmap get_menu_icon(const color_scheme_t& scheme, int size);
	QPixmap get_warning_icon(const color_scheme_t& scheme, int size);
	QPixmap get_window_icon(const color_scheme_t& scheme, int size);
}

