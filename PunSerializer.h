#pragma once

#include <QString>
#include <QByteArray>

namespace PunSerializer
{
    void serialize(const QString& text, QByteArray* output);
}

