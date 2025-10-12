#include "StyleSheet.h"

QString StyleSheet::format_global(const color_scheme_t& scheme, uint32_t fontSize)
{
    return format_main_window(scheme, fontSize) 
        + format_scrollbar(scheme) 
        + format_push_button() 
        + format_menu()
        + format_message_box();
}


QString StyleSheet::format_main_window(const color_scheme_t& scheme, uint32_t fontSize)
{
    return QString("QMainWindow{font-size: %1px; color: %2; background-color: %3}").arg(
        QString::number(fontSize),
        ColorScheme::color(scheme),
        ColorScheme::highlighted(scheme) );
}

QString StyleSheet::format_text_edit(const color_scheme_t& scheme, uint32_t fontSize)
{
    return QString("QPlainTextEdit{border: 0; font-size: %1px; color: %2; background-color: %3; selection-color: %2; selection-background-color: %4}").arg(
        QString::number(fontSize),
        ColorScheme::color(scheme),
        ColorScheme::background(scheme),
        ColorScheme::highlighted(scheme));
}


QString StyleSheet::format_status_bar(const color_scheme_t& scheme, uint32_t fontSize)
{
    return QString("QStatusBar{border: 0;font-size: %1px; color: %2; background: %3}").arg(
        QString::number(fontSize),
        ColorScheme::color(scheme),
        ColorScheme::highlighted(scheme) );
}


QString StyleSheet::format_status_label(const color_scheme_t& scheme, uint32_t fontSize)
{
    return QString("QLabel{border: 0; font-size: %1px; color: %2; background: %3}").arg(
        QString::number(fontSize),
        ColorScheme::color(scheme),
        ColorScheme::highlighted(scheme) );
}


QString StyleSheet::format_scrollbar(const color_scheme_t& scheme)
{
    return QString(
		"QScrollBar::sub-page:vertical {background: %1;}"
		"QScrollBar::add-page:vertical {background: %1;}"
        "QScrollBar:vertical{border: 0;background-color: %1;width: 10px;}"
        "QScrollBar::handle:vertical{background-color: %2;min-height: 10px;}").arg(
            ColorScheme::background(scheme), 
            ColorScheme::highlighted(scheme));
}


QString StyleSheet::format_menu()
{
    return QString(
        "QMenu{border: 0; color: %1; background-color: %2} "
        "QMenu::separator{height: 15px; background: %2; margin-left: 0px; margin-right: 0px; } "
        "QMenu::item{ padding: 2 25px 2 20px; border: 0; } "
        "QMenu::item:selected{ background: %3; }"
        "QMenu::item:disabled{ color: %4 }" ).arg(
            ColorScheme::Ui::color,
            ColorScheme::Ui::background,
            ColorScheme::Ui::highlighted,
            ColorScheme::Ui::dimmed);
}


QString StyleSheet::format_push_button()
{
    return QString(
        "QPushButton{border: 0; color: %1; background-color: %3; padding: 5 25px 5 25px;}"
        "QPushButton:hover{color: %4;}"
        "QPushButton:pressed{color: %4; background-color: %2;}" ).arg(
            ColorScheme::Ui::color,
            ColorScheme::Ui::background,
			ColorScheme::Ui::highlighted,
            ColorScheme::Ui::dimmed);
}

QString StyleSheet::format_message_box()
{
    return QString(
        "QMessageBox{ color: %1; background: %2;}" 
        "QMessageBox QLabel{ color: %1;}" ).arg(
            ColorScheme::Ui::color,
			ColorScheme::Ui::background );
}



