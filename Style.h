#pragma once

#include <tuple>
#include <vector>

#include <QString>

namespace Style
{
    using T = std::tuple<QString, uint32_t>;

    static inline QString& color_scheme(T& op) { return std::get<0>(op); }
    static inline const QString& color_scheme(const T& op) { return std::get<0>(op); }
    static inline uint32_t& font_size(T& op) { return std::get<1>(op); }
    static inline const uint32_t& font_size(const T& op) { return std::get<1>(op); }

    extern const T defaults;    
    extern const std::vector<uint32_t> font_sizes;
}
using style_t = Style::T;

