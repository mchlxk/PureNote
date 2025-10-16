#pragma once

#include <QByteArray>
#include <QString>

#include "Pun.h"
#include "PunExpected.h"

namespace PunParser
{
    Pun::expected<pun_t, QString> parse(const QByteArray& input);

    QString parse_style(QXmlStreamReader& reader, pun_t& output);
    QString parse_window(QXmlStreamReader& reader, pun_t& output);
    QString parse_content(QXmlStreamReader& reader, pun_t& output);

    bool parse_bool_attribute(QXmlStreamReader& reader, const QString& attributeName);
    bool parse_bool(const QString& value);
}

