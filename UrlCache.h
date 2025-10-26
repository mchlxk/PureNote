#pragma once

#include <tuple>
#include <unordered_map>
#include <vector>

#include <QString>

namespace UrlCache
{
	using url_item_t = std::tuple<int, int, QString>;
	using url_array_t = std::vector<url_item_t>;
	using T = std::unordered_map<uint32_t, url_array_t>;

	bool has_url(const T&, uint32_t blockId, int pos);
	QString get_url(const T&, uint32_t blockId, int pos);
}
using url_cache_t = UrlCache::T;

