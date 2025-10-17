#pragma once

#include <tuple>

#include "ElementExtract.h"

#include <QString>
#include <QSize>

#include "Style.h"
#include "Window.h"
#include "Content.h"

namespace Pun
{
	using T = std::tuple<style_t, window_t, content_t>;

	ELEMENT_EXTRACT(T, 0, style)
	ELEMENT_EXTRACT(T, 1, window)
	ELEMENT_EXTRACT(T, 2, content)
}
using pun_t = Pun::T;


