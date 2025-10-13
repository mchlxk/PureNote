#pragma once

#include "Style.h"

namespace StyleSheet 
{
	QString format_menu();
	QString format_push_button();
	QString format_message_box();
	QString format_tooltip();

	QString format_global(const style_t& style);
	QString format_main_window(const style_t& style);
	QString format_text_edit(const style_t& style);
	QString format_status_bar(const style_t& style);
	QString format_status_label(const style_t& style);
	QString format_scrollbar(const style_t& style);
}

