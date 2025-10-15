#pragma once

#include <tuple>
#include <unordered_map>

#include <QString>
#include <QHash>

#include "HashQString.h"
#include "AccessTupleElement.h"

namespace ColorScheme
{
	using T = std::tuple<QString, QString, QString, QString>;

	ACCESS_TUPLE_ELEMENT(T, 0, color);
	ACCESS_TUPLE_ELEMENT(T, 1, background);
	ACCESS_TUPLE_ELEMENT(T, 2, accent);
	ACCESS_TUPLE_ELEMENT(T, 3, disabled);
	
	extern const std::unordered_map<QString, T, hash_qstring> schemas;

	namespace Ui
	{
		extern const QString color;
		extern const QString dimmed;
		extern const QString background;
		extern const QString accent;
	}
}

using color_scheme_t = ColorScheme::T;

