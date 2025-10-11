#include "ColorScheme.h"


const std::unordered_map<QString, ColorScheme::T> ColorScheme::schemas = {
	{ "Flamingo", { "#5a5255", "#f0adb0"} }
	,{ "Mandys", { "#5a5255", "#f5c2ab"} }
	,{ "Peach", { "#5a5255", "#fae0ad"} }
	,{ "Sprout", { "#5a5255", "#c6d7b2"} }
	,{ "Tropical Blue", { "#5a5255", "#c4def0"} }
	,{ "Dark", { "#dddddd", "#5a5255"} }
};

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

