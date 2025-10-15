#pragma once

#include <tuple>
#include <unordered_map>

#include <QString>
#include <QHash>

#include "HashQString.h"
#include "ElementExtract.h"

namespace ColorScheme
{
	using T = std::tuple<QString, QString, QString, QString>;

	ELEMENT_EXTRACT(T, 0, color);
	ELEMENT_EXTRACT(T, 1, background);
	ELEMENT_EXTRACT(T, 2, accent);
	ELEMENT_EXTRACT(T, 3, disabled);
	
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

