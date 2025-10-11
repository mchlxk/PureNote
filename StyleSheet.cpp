#include "StyleSheet.h"

QString StyleSheet::format_global(const color_scheme_t& scheme, uint32_t fontSize)
{
    return format_main_window(scheme, fontSize)+format_scrollbar(scheme);
}


QString StyleSheet::format_main_window(const color_scheme_t& scheme, uint32_t fontSize)
{
    return QString("QMainWindow{font-size: %1px; color: %2; background-color: %3}").arg(
        QString::number(fontSize),
        ColorScheme::fg(scheme),
        ColorScheme::bg(scheme) );
}

QString StyleSheet::format_text_edit(const color_scheme_t& scheme, uint32_t fontSize)
{
    return QString("QPlainTextEdit{border: 0; font-size: %1px; color: %2; background-color: %3; selection-color: %2; selection-background-color: %4}").arg(
        QString::number(fontSize),
        ColorScheme::fg(scheme),
        ColorScheme::bg(scheme),
        ColorScheme::highlighted(scheme));
}


QString StyleSheet::format_status_bar(const color_scheme_t& scheme, uint32_t fontSize)
{
    return QString("QStatusBar{font-size: %1px; color: %2; background: %3}").arg(
        QString::number(fontSize),
        ColorScheme::fg(scheme),
        ColorScheme::highlighted(scheme) );
}


QString StyleSheet::format_scrollbar(const color_scheme_t& scheme)
{
    return QString(
        "QScrollBar:vertical{"
        "border: 0;"
        "background-color: %1;"
        //"background: solid;"
        "width: 10px;"
        "}"
        "QScrollBar::handle:vertical{"
        "background-color: %2;"
        "min-height: 10px;"
        "}").arg(ColorScheme::bg(scheme), ColorScheme::highlighted(scheme));
}


QString StyleSheet::format_menu()
{
    return QString("QMenu{border: 0; color: #dddddd; background-color: #6a6366} QMenu::separator{height: 15px; background: #6a6366; margin-left: 0px; margin-right: 0px; } QMenu::item{ padding: 2px 25px 2px 20px; border: 0; } QMenu::item:selected{ background: #5a5255; }");
}


