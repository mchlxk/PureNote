#pragma once

#define ACCESS_TUPLE_ELEMENT(container, position, identifier) \
static inline auto& identifier(container& op) { return std::get<position>(op); }  \
static inline const auto& identifier(const container& op) { return std::get<position>(op); }  \

