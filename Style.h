#pragma once

#include <tuple>
#include <set>
#include <unordered_map>

#include <QString>
#include <QStringList>

#include "HashQString.h"

namespace Style
{
    using T = std::tuple<QString, uint32_t, QString>;

    static inline QString& color_scheme(T& op) { return std::get<0>(op); }
    static inline const QString& color_scheme(const T& op) { return std::get<0>(op); }
    static inline uint32_t& font_size(T& op) { return std::get<1>(op); }
    static inline const uint32_t& font_size(const T& op) { return std::get<1>(op); }
    static inline QString& font_family(T& op) { return std::get<2>(op); }
    static inline const QString& font_family(const T& op) { return std::get<2>(op); }

    extern const T defaults;    
    extern const std::set<uint32_t> font_sizes;
    extern const std::unordered_map<QString, QStringList, hash_qstring> font_families;
}
using style_t = Style::T;

