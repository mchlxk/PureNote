#pragma once

#include <tuple>
#include <unordered_map>

#include <QString>
#include <QHash>

#include "HashQString.h"

namespace ColorScheme
{
	using T = std::tuple<QString, QString, QString, QString>;
	
	static inline QString& color(T& op) { return std::get<0>(op); }
	static inline const QString& color(const  T& op) { return std::get<0>(op); }
	static inline QString& background(T& op) { return std::get<1>(op); }
	static inline const QString& background(const  T& op) { return std::get<1>(op); }
	static inline QString& accent(T& op) { return std::get<2>(op); }
	static inline const QString& accent(const  T& op) { return std::get<2>(op); }
	static inline QString& disabled(T& op) { return std::get<3>(op); }
	static inline const QString& disabled(const  T& op) { return std::get<3>(op); }

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

