#pragma once

#define ELEMENT_EXTRACT(container, position, identifier) \
static inline auto& identifier(container& op) { return std::get<position>(op); }  \
static inline const auto& identifier(const container& op) { return std::get<position>(op); }  \

