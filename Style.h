#pragma once

#include <tuple>
#include <set>

#include <QString>

namespace Style
{
    using T = std::tuple<QString, uint32_t, float>;

    static inline QString& color_scheme(T& op) { return std::get<0>(op); }
    static inline const QString& color_scheme(const T& op) { return std::get<0>(op); }
    static inline uint32_t& font_size(T& op) { return std::get<1>(op); }
    static inline const uint32_t& font_size(const T& op) { return std::get<1>(op); }
    static inline float& opacity(T& op) { return std::get<2>(op); }
    static inline const float& opacity(const T& op) { return std::get<2>(op); }

    extern const T defaults;    
    extern const std::set<uint32_t> font_sizes;
    static constexpr float min_opacity{ .15f };
}
using style_t = Style::T;

