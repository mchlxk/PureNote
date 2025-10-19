#pragma once

#include <QByteArray>
#include <QString>

#include "Pun.h"
#include "PunExpected.h"
#include "Window.h"

namespace PunParser
{
    pun_expected_t<pun_t, QString> parse(const QByteArray& input);

    pun_expected_t<style_t, QString> parse_style(QXmlStreamReader& reader);
    pun_expected_t<window_t, QString> parse_window(QXmlStreamReader& reader);
    pun_expected_t<content_t, QString> parse_content(QXmlStreamReader& reader);
    pun_expected_t<std::pair<QString, int>, QString> parse_font(QXmlStreamReader& reader);

    namespace Utility
    {
        bool at_element_end(QXmlStreamReader& reader, const QString& name);
        bool parse_bool_attribute(const QXmlStreamReader& reader, const QString& attributeName);
        bool parse_bool(const QString& value);
    }
}

