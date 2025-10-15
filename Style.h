#pragma once

#include <tuple>
#include <set>
#include <unordered_map>

#include <QString>
#include <QStringList>

#include "HashQString.h"
#include "ElementExtract.h"

namespace Style
{
    using T = std::tuple<QString, uint32_t, QString>;

    ELEMENT_EXTRACT(T, 0, color_scheme) 
    ELEMENT_EXTRACT(T, 1, font_size) 
    ELEMENT_EXTRACT(T, 2, font_family) 

    extern const T defaults;    
    extern const std::set<uint32_t> font_sizes;
    extern const std::unordered_map<QString, QStringList, hash_qstring> font_families;
}
using style_t = Style::T;

