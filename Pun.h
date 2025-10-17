#pragma once

#include <tuple>

#include "ElementExtract.h"

#include <QString>
#include <QSize>

#include "Style.h"
#include "Window.h"

namespace Pun
{
	using T = std::tuple<style_t, window_t, bool, QString>;

	ELEMENT_EXTRACT(T, 0, style)
	ELEMENT_EXTRACT(T, 1, window)
	ELEMENT_EXTRACT(T, 2, locked)
	ELEMENT_EXTRACT(T, 3, content)
}
using pun_t = Pun::T;


