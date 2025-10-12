#include <QMouseEvent>

#include "MouseEvent.h"


bool MouseEvent::is_alt_lmb_press(const QEvent* evt)
{
    if (evt->type() != QEvent::MouseButtonPress)
        return false;
    const QMouseEvent* mouseEvt = static_cast<const QMouseEvent*>(evt);
    if (mouseEvt->modifiers() != Qt::AltModifier)
        return false;
    if (mouseEvt->button() != Qt::MouseButton::LeftButton)
        return false;
    return true;
}

bool MouseEvent::is_lmb_release(const QEvent* evt)
{
    return evt->type() == QEvent::MouseButtonRelease
        && static_cast<const QMouseEvent*>(evt)->button() == Qt::MouseButton::LeftButton;
}

bool MouseEvent::is_alt_rmb_press(const QEvent* evt)
{
    if (evt->type() != QEvent::MouseButtonPress)
        return false;
    const QMouseEvent* mouseEvt = static_cast<const QMouseEvent*>(evt);
    if (mouseEvt->modifiers() != Qt::AltModifier)
        return false;
    if (mouseEvt->button() != Qt::MouseButton::RightButton)
        return false;
    return true;
}

bool MouseEvent::is_rmb_release(const QEvent* evt)
{
    return evt->type() == QEvent::MouseButtonRelease
        && static_cast<const QMouseEvent*>(evt)->button() == Qt::MouseButton::RightButton;
}

bool MouseEvent::is_mmb_press(const QEvent* evt)
{
    return evt->type() == QEvent::MouseButtonPress
        && static_cast<const QMouseEvent*>(evt)->button() == Qt::MouseButton::MiddleButton;
}

bool MouseEvent::is_mmb_release(const QEvent* evt)
{
    return evt->type() == QEvent::MouseButtonRelease
        && static_cast<const QMouseEvent*>(evt)->button() == Qt::MouseButton::MiddleButton;
}

bool MouseEvent::is_rmb_press(const QEvent* evt)
{
    if (evt->type() != QEvent::MouseButtonPress)
        return false;
    const QMouseEvent* mouseEvt = static_cast<const QMouseEvent*>(evt);
    if (mouseEvt->modifiers() != Qt::NoModifier)
        return false;
    if (mouseEvt->button() != Qt::MouseButton::RightButton)
        return false;
    return true;
}

bool MouseEvent::is_ctrl_wheel_down(const QEvent* evt)
{
    if (evt->type() != QEvent::Wheel)
        return false;
    const QWheelEvent* wheelEvt = static_cast<const QWheelEvent*>(evt);
    if (wheelEvt->modifiers() != Qt::ControlModifier)
        return false;
    if (wheelEvt->angleDelta().y() > 0)
        return false;
    return true;
}

bool MouseEvent::is_ctrl_wheel_up(const QEvent* evt)
{
    if (evt->type() != QEvent::Wheel)
        return false;
    const QWheelEvent* wheelEvt = static_cast<const QWheelEvent*>(evt);
    if (wheelEvt->modifiers() != Qt::ControlModifier)
        return false;
    if (wheelEvt->angleDelta().y() < 0)
        return false;
    return true;
}

bool MouseEvent::is_alt_wheel_down(const QEvent* evt)
{
    if (evt->type() != QEvent::Wheel)
        return false;
    const QWheelEvent* wheelEvt = static_cast<const QWheelEvent*>(evt);
    if (wheelEvt->angleDelta().y() != 0)
        return false;
    if (wheelEvt->angleDelta().x() > 0)
        return false;
    return true;
}

bool MouseEvent::is_alt_wheel_up(const QEvent* evt)
{
    if (evt->type() != QEvent::Wheel)
        return false;
    const QWheelEvent* wheelEvt = static_cast<const QWheelEvent*>(evt);
    if (wheelEvt->angleDelta().y() != 0)
        return false;
    if (wheelEvt->angleDelta().x() < 0)
        return false;
    return true;
}

