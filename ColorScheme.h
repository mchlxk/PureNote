#pragma once

#include <tuple>
#include <unordered_map>

#include <QString>
#include <QHash>

#include "HashQString.h"

namespace ColorScheme
{
	using T = std::tuple<QString, QString, QString>;
	
	static inline QString& fg(T& op) { return std::get<0>(op); }
	static inline const QString& fg(const  T& op) { return std::get<0>(op); }
	static inline QString& bg(T& op) { return std::get<1>(op); }
	static inline const QString& bg(const  T& op) { return std::get<1>(op); }
	static inline QString& highlighted(T& op) { return std::get<2>(op); }
	static inline const QString& highlighted(const  T& op) { return std::get<2>(op); }

	extern const std::unordered_map<QString, T> schemas;

	namespace Menu
	{
		extern const QString color;
		extern const QString background;
		extern const QString highlighted;
	}
}

using color_scheme_t = ColorScheme::T;

