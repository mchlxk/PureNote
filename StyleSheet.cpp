#include "StyleSheet.h"
#include "ColorScheme.h"


QString StyleSheet::format_menu(const style_t& style)
{
    const auto& schema = ColorScheme::schemas.at(Style::color_scheme(style));
    return QString(
        "QMenu{border: 0; color: %1; background-color: %2} "
        "QMenu::separator{height: 15px; background: %2; margin-left: 0px; margin-right: 0px; } "
        "QMenu::item{ padding: 2 25px 2 20px; border: 0; } "
        "QMenu::item:selected{ background: %3; }"
        "QMenu::item:disabled{ color: %4 }" ).arg(
            ColorScheme::color(schema),
            ColorScheme::accent(schema),
            ColorScheme::background(schema),
            ColorScheme::disabled(schema));
}


QString StyleSheet::format_push_button()
{
    return QString(
        "QPushButton{border: 0; color: %1; background-color: %3; padding: 5 25px 5 25px;}"
        "QPushButton:hover{color: %4;}"
        "QPushButton:pressed{color: %4; background-color: %2;}" ).arg(
            ColorScheme::Ui::color,
            ColorScheme::Ui::background,
			ColorScheme::Ui::accent,
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


QString StyleSheet::format_tooltip()
{
    return QString("QToolTip { border: 0; color: %1; background-color: %2; }").arg(
            ColorScheme::Ui::color,
			ColorScheme::Ui::background );
}



QString StyleSheet::format_global(const style_t& style)
{
    return format_main_window(style) 
        + format_scrollbar(style) 
        + format_push_button() 
        + format_menu(style)
        + format_message_box();
}


QString StyleSheet::format_main_window(const style_t& style)
{
    const auto& schema = ColorScheme::schemas.at(Style::color_scheme(style));
    return QString("QMainWindow{font-size: %1px; color: %2; background-color: %3}").arg(
        QString::number(Style::font_size(style)),
        ColorScheme::color(schema),
        ColorScheme::accent(schema));
}

QString StyleSheet::format_text_edit(const style_t& style)
{
    const auto& schema = ColorScheme::schemas.at(Style::color_scheme(style));
    return QString("QPlainTextEdit{border: 0; font-size: %1px; font-family: '%2'; color: %3; background-color: %4; selection-color: %3; selection-background-color: %5}").arg(
        QString::number(Style::font_size(style)),
        Style::font_families.at(Style::font_family(style)).join(","),
        ColorScheme::color(schema),
        ColorScheme::background(schema),
        ColorScheme::accent(schema));
}


QString StyleSheet::format_status_bar(const style_t& style)
{
    const auto& schema = ColorScheme::schemas.at(Style::color_scheme(style));
    return QString("QStatusBar{background: %1} QStatusBar::item{border: 0;}").arg(
        ColorScheme::accent(schema) );
}


QString StyleSheet::format_status_label(const style_t& style)
{
    const auto& schema = ColorScheme::schemas.at(Style::color_scheme(style));
    return format_tooltip() + QString("QLabel{border: 0; font-size: %1px; font-family: '%2'; color: %3; background: %4}").arg(
        QString::number(Style::font_size(style)),
        Style::font_families.at(Style::font_family(style)).join(","),
        ColorScheme::color(schema),
        ColorScheme::accent(schema));
}


QString StyleSheet::format_scrollbar(const style_t& style)
{
    const auto& schema = ColorScheme::schemas.at(Style::color_scheme(style));
    return QString(
		"QScrollBar::sub-page:vertical {background: %1;}"
		"QScrollBar::add-page:vertical {background: %1;}"
        "QScrollBar:vertical{border: 0;background-color: %1;width: 10px;}"
        "QScrollBar::handle:vertical{background-color: %2;min-height: 10px;}").arg(
            ColorScheme::background(schema), 
            ColorScheme::accent(schema));

}
