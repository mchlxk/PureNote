#pragma once

#include "ColorScheme.h"

namespace StyleSheet 
{
	QString format_main_window(const color_scheme_t& scheme, uint32_t fontSize);
	QString format_text_edit(const color_scheme_t& scheme, uint32_t fontSize);
	QString format_status_bar(const color_scheme_t& scheme, uint32_t fontSize);
}

