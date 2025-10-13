#include "Style.h"

const Style::T Style::defaults{ "Flamingo", 18 , "Helvetica" };

const std::set<uint32_t> Style::font_sizes{ 10, 12, 14, 16, 18, 20, 24, 30, 36 };

const std::unordered_map<QString, QString, hash_qstring> Style::font_families = {
	{"Helvetica", "Helvetica" }
	,{ "Monospaced", "Lucida Console" }
	,{ "Comic Sans", "Comic Sans MS" }
	,{ "Handwritig", "Lucida Handwriting" }
};

