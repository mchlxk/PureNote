#pragma once

#include <string>
#include <QString>

struct hash_qstring
{
    std::size_t operator () (const QString& s) const { return std::hash<std::string>{}(s.toStdString()); }
};

