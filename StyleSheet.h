#pragma once

#include "ColorScheme.h"

namespace StyleSheet 
{
	QString format_global(const color_scheme_t& scheme, uint32_t fontSize);
	QString format_main_window(const color_scheme_t& scheme, uint32_t fontSize);
	QString format_text_edit(const color_scheme_t& scheme, uint32_t fontSize);
	QString format_status_bar(const color_scheme_t& scheme, uint32_t fontSize);
	QString format_status_label(const color_scheme_t& scheme, uint32_t fontSize);
	QString format_scrollbar(const color_scheme_t& scheme);
	QString format_menu();
}

