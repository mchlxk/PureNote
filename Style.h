#pragma once

#include <tuple>
#include <set>
#include <unordered_map>

#include <QString>
#include <QStringList>

#include "HashQString.h"
#include "AccessTupleElement.h"

namespace Style
{
    using T = std::tuple<QString, uint32_t, QString>;

    ACCESS_TUPLE_ELEMENT(T, 0, color_scheme) 
    ACCESS_TUPLE_ELEMENT(T, 1, font_size) 
    ACCESS_TUPLE_ELEMENT(T, 2, font_family) 

    extern const T defaults;    
    extern const std::set<uint32_t> font_sizes;
    extern const std::unordered_map<QString, QStringList, hash_qstring> font_families;
}
using style_t = Style::T;

