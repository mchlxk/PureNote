#pragma once

#include <set>

namespace State
{
    namespace Tag
    {
        constexpr uint8_t Fullscreen{1};
        constexpr uint8_t OnTop{2};
        constexpr uint8_t OpaqueWhenActive{3};
        constexpr uint8_t HasDialogContext{4};
        constexpr uint8_t HasMouseContext{5};
        constexpr uint8_t OpacityAdjust{6};
        constexpr uint8_t MsgBox{7};
    }
    using tags_t = std::set<uint8_t>;    
    
    template<uint8_t TAG> static inline bool has_tag(const tags_t& op) { return op.count(TAG); }
    template<uint8_t TAG> static inline void clear_tag(tags_t& op) { op.erase(TAG); }
    template<uint8_t TAG> static inline void set_tag(tags_t& op) { op.insert(TAG); }
    template<uint8_t TAG> static inline void toggle_tag(tags_t& op) { if (has_tag<TAG>(op)) clear_tag<TAG>(op); else set_tag<TAG>(op); }
}

