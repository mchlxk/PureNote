#include "StyleSheet.h"



QString StyleSheet::format_main_window(const color_scheme_t& scheme, uint32_t fontSize)
{
    return QString("QMainWindow{font-size: %1px; color: %2; background-color: %3}").arg(
        QString::number(fontSize),
        ColorScheme::fg(scheme),
        ColorScheme::bg(scheme) );
}

QString StyleSheet::format_text_edit(const color_scheme_t& scheme, uint32_t fontSize)
{
    return QString("QPlainTextEdit{border: 0; font-size: %1px; color: %2; background-color: %3}").arg(
        QString::number(fontSize),
        ColorScheme::fg(scheme),
        ColorScheme::bg(scheme) );
}


QString StyleSheet::format_status_bar(const color_scheme_t& scheme, uint32_t fontSize)
{
    return QString("QStatusBar{font-size: %1px; color: %2; background: %3}").arg(
        QString::number(fontSize),
        ColorScheme::fg(scheme),
        ColorScheme::highlighted(scheme) );
}
