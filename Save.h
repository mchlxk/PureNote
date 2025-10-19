#pragma once

#include <tuple>

#include <QString>

#include "ElementExtract.h"
#include "PunOptional.h"
#include "Pun.h"

namespace Save
{
	using T = std::tuple<QString, pun_optional_t<pun_t>>;

	ELEMENT_EXTRACT(T, 0, file_path)
	ELEMENT_EXTRACT(T, 1, pun)

	extern const T no_save;
}

using save_t = Save::T;

