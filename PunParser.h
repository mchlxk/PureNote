#pragma once

#include <QByteArray>
#include <QString>

#include "Pun.h"
#include "PunExpected.h"
#include "Window.h"

namespace PunParser
{
    Pun::expected<pun_t, QString> parse(const QByteArray& input);

    Pun::expected<style_t, QString> parse_style(QXmlStreamReader& reader);
    Pun::expected<window_t, QString> parse_window(QXmlStreamReader& reader);
    Pun::expected<content_t, QString> parse_content(QXmlStreamReader& reader);
    Pun::expected<std::pair<QString, int>, QString> parse_font(QXmlStreamReader& reader);

    namespace Utility
    {
        bool at_element_end(QXmlStreamReader& reader, const QString& name);
        bool parse_bool_attribute(const QXmlStreamReader& reader, const QString& attributeName);
        bool parse_bool(const QString& value);
    }
}

