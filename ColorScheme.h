#pragma once

#include <tuple>
#include <unordered_map>

#include <QString>
#include <QHash>

namespace std {
  template<> struct hash<QString> {
    std::size_t operator()(const QString& s) const noexcept {
      return (size_t) qHash(s);
    }
  };
}

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
}

using color_scheme_t = ColorScheme::T;


/*

"#f0adb0"
"#f5c2ab"
"#fae0ad"
"#c6d7b2"
"#c4def0"

dark:
#dddddd
#5a5255

*/
