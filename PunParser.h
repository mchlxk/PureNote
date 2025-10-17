#pragma once

#include <QByteArray>
#include <QString>

#include "Pun.h"
#include "PunExpected.h"

namespace PunParser
{
    Pun::expected<pun_t, QString> parse(const QByteArray& input);

    Pun::expected<style_t, QString> parse_style(QXmlStreamReader& reader);
    QString parse_window(QXmlStreamReader& reader, pun_t& output);
    Pun::expected<std::pair<QString, bool>, QString> parse_content(QXmlStreamReader& reader);
    Pun::expected<std::pair<QString, int>, QString> parse_font(QXmlStreamReader& reader);

    bool parse_bool_attribute(QXmlStreamReader& reader, const QString& attributeName);
    bool parse_bool(const QString& value);
}

