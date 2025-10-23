#pragma once

#include <set>

#include <QObject>

namespace State
{
    namespace Tag
    {
        constexpr uint8_t Fullscreen{1};
        constexpr uint8_t OnTop{2};
        constexpr uint8_t OpaqueWhenActive{3};
        constexpr uint8_t HasDialogContext{4};
        constexpr uint8_t HasMouseContext{5};
        constexpr uint8_t OpacityInteraction{6};
        constexpr uint8_t MsgBox{8};
        constexpr uint8_t Locked{9};
    }
    using tags_t = std::set<uint8_t>;    
    
    class T
	: public QObject
    {
        Q_OBJECT
    
    public:
        void Set(uint8_t tag);
        void Set(uint8_t tag, bool set);
        void Clear(uint8_t tag);
        void Toggle(uint8_t tag);
        bool Get(uint8_t tag) const;
    signals:
        void state_changed(uint8_t);

	private:
        tags_t m_tags;
    };
}
using state_t = State::T;

