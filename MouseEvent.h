#pragma once


#include <QEvent>

namespace MouseEvent
{
    enum class ActionE
    {
        None,
        MoveMmb,
        MoveAltLmb,
        ResizeAltRmb
    };

    bool is_alt_lmb_press(const QEvent* evt);
    bool is_lmb_release(const QEvent* evt);
    bool is_alt_rmb_press(const QEvent* evt);
    bool is_rmb_release(const QEvent* evt);
    bool is_mmb_press(const QEvent* evt);
    bool is_mmb_release(const QEvent* evt);
    bool is_rmb_press(const QEvent* evt);
    bool is_ctrl_wheel_down(const QEvent* evt);
    bool is_ctrl_wheel_up(const QEvent* evt);
    bool is_alt_wheel_down(const QEvent* evt);
    bool is_alt_wheel_up(const QEvent* evt);
}

