#include "ColorScheme.h"

#include "HashQString.h"

const std::unordered_map<QString, ColorScheme::T, hash_qstring> ColorScheme::schemas = {
	{ "Flamingo", { "#5a5255", "#f0adb0", "#d89b9e"}}
	,{ "Mandys", { "#5a5255", "#f5c2ab", "#dcae99"} }
	,{ "Peach", { "#5a5255", "#fae0ad", "#e1c99b"} }
	,{ "Sprout", { "#5a5255", "#c6d7b2", "#b2c1a0"} }
	,{ "Tropical Blue", { "#5a5255", "#c4def0", "#b0c7d8"} }
	,{ "Dark", { "#dddddd", "#5a5255", "#6a6366"} }
};

const QString ColorScheme::Ui::color{"#dddddd"};
const QString ColorScheme::Ui::dimmed{"#bbbbbb"};
const QString ColorScheme::Ui::background{"#6a6366"};
const QString ColorScheme::Ui::highlighted{"#5a5255"};

