#pragma once

#include <set>

#include <QEvent>

namespace EvtType
{
	enum class E : uint8_t
	{
		None,
		Activate,
		Deactivate,
		Enter,
		Leave,
		MouseMove,
		MouseMoveCtrl,
		LmbPress,
		LmbPressCtrl,
		LmbPressAlt,
		LmbRelease,
		RmbPress,
		RmbPressCtrl,
		RmbPressAlt,
		RmbRelease,
		MmbPress,
		MmbRelease,
		WheelUpCtrl,
		WheelDownCtrl,
		WheelUpAlt,
		WheelDownAlt,
		CtrlPress,
		CtrlRelease
	};

	E get(QEvent* evt);
}



