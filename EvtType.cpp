#include "EvtType.h"

#include <QMouseEvent>

EvtType::E EvtType::get(QEvent* evt)
{
    if (evt->type() == QEvent::MouseMove)
    {
        const QMouseEvent* mouseEvent = static_cast<const QMouseEvent*>(evt);
        if (mouseEvent->modifiers() == Qt::ControlModifier)
            return EvtType::E::MouseMoveCtrl;
		return EvtType::E::MouseMove;
    }

    if (evt->type() == QEvent::MouseButtonPress)
    {
        const QMouseEvent* mouseEvent = static_cast<const QMouseEvent*>(evt);

        if (mouseEvent->button() == Qt::MouseButton::LeftButton)
        {
			if (mouseEvent->modifiers() == Qt::ControlModifier)
				return EvtType::E::LmbPressCtrl;
			if (mouseEvent->modifiers() == Qt::AltModifier)
				return EvtType::E::LmbPressAlt;
			return EvtType::E::LmbPress;
        }

        if (mouseEvent->button() == Qt::MouseButton::RightButton)
        {
			if (mouseEvent->modifiers() == Qt::ControlModifier)
				return EvtType::E::RmbPressCtrl;
			if (mouseEvent->modifiers() == Qt::AltModifier)
				return EvtType::E::RmbPressAlt;
			return EvtType::E::LmbPress;
        }

        if (mouseEvent->button() == Qt::MouseButton::MiddleButton)
			return EvtType::E::MmbPress;
    }

    if (evt->type() == QEvent::MouseButtonRelease)
    {
        const QMouseEvent* mouseEvent = static_cast<const QMouseEvent*>(evt);
        if (mouseEvent->button() == Qt::MouseButton::LeftButton)
			return EvtType::E::LmbRelease;
        if (mouseEvent->button() == Qt::MouseButton::RightButton)
			return EvtType::E::RmbRelease;
        if (mouseEvent->button() == Qt::MouseButton::MiddleButton)
			return EvtType::E::MmbRelease;
    }

    if (evt->type() == QEvent::Wheel)
    {
		const QWheelEvent* wheelEvt = static_cast<const QWheelEvent*>(evt);
        if (wheelEvt->angleDelta().y() < 0 && wheelEvt->modifiers() == Qt::ControlModifier)
            return EvtType::E::WheelDownCtrl;
        if (wheelEvt->angleDelta().y() > 0 && wheelEvt->modifiers() == Qt::ControlModifier)
            return EvtType::E::WheelUpCtrl;
		if ((wheelEvt->angleDelta().y() == 0) && (wheelEvt->angleDelta().x() < 0))
			return EvtType::E::WheelDownAlt;
		if ((wheelEvt->angleDelta().y() == 0) && (wheelEvt->angleDelta().x() > 0))
			return EvtType::E::WheelUpAlt;
    }

    if (evt->type() == QEvent::WindowActivate)
        return EvtType::E::Activate;
    if (evt->type() == QEvent::WindowDeactivate)
        return EvtType::E::Deactivate;
    if (evt->type() == QEvent::Enter)
        return EvtType::E::Enter;
    if (evt->type() == QEvent::Leave)
        return EvtType::E::Leave;

    if (evt->type() == QEvent::KeyPress)
    {
        const QKeyEvent* keyEvent = static_cast<QKeyEvent*>(evt);
        if (keyEvent->key() == Qt::Key_Control)
            return EvtType::E::CtrlPress;
    }

    if (evt->type() == QEvent::KeyRelease)
    {
        const QKeyEvent* keyEvent = static_cast<QKeyEvent*>(evt);
        if (keyEvent->key() == Qt::Key_Control)
            return EvtType::E::CtrlRelease;
    }

    return EvtType::E::None;
}


