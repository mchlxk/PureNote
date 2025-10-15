#include <vector>

#include "Style.h"

const Style::T Style::defaults{ "Flamingo", 18 , "Sans Serif" };

const std::set<uint32_t> Style::font_sizes{ 10, 12, 14, 16, 18, 20, 24, 30, 36, 45 };

const std::unordered_map<QString, QStringList, hash_qstring> Style::font_families = {
	{"Sans Serif", { "Helvetica", "Tahoma", "Verdana", "Microsoft Sans Serif" }}
	,{"Monospace", { "Lucida Console", "Cascadia Code", "Cascadia Mono", "Consolas", "Fira Code", "Courier New" }}
	,{"Serif", { "Constantia", "Georgia", "Palatino Linotype", "Adobe Garamond Pro", "Times New Roman" }}
	,{"Comic", { "Bad Comic", "Comic Sans MS" }}
	,{"Handwritten", { "Segoe Print", "Ink Free", "MV Boli" }}
	,{"Script", { "Lucida Handwriting", "Segoe Script", "Brush Script Std", "Script" }}
};


