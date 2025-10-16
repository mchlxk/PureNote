#pragma once

#include <QByteArray>
#include <QString>

#include "Pun.h"
#include "PunExpected.h"

namespace PunParser
{
    Pun::expected<pun_t, QString> parse(const QByteArray& input);
}

