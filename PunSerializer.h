#pragma once

#include <unordered_map>

#include <QString>
#include <QByteArray>

#include "Pun.h"

namespace PunSerializer
{
    void serialize(const pun_t& pun, QByteArray* output);
    void write_style(const style_t& style, QXmlStreamWriter& writer);

    static const std::unordered_map<bool, QString> serialize_bool = { { true, "true"} ,{ false, "false"} };
}

