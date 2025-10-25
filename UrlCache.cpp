#include "UrlCache.h"


bool UrlCache::has_url(const T& cache, uint32_t blockId, int pos)
{
	if(!cache.count(blockId))
		return false;
	const auto found = std::find_if(cache.at(blockId).cbegin(), cache.at(blockId).cend(), [pos] (const url_item_t& urlItem) {
		return pos >= std::get<0>(urlItem) && pos <= std::get<1>(urlItem);
	});
	return found != cache.at(blockId).cend();
}

QString UrlCache::get_url(const T& cache, uint32_t blockId, int pos)
{
	if(!cache.count(blockId))
		return "";
	const auto found = std::find_if(cache.at(blockId).cbegin(), cache.at(blockId).cend(), [pos] (const url_item_t& urlItem) {
		return pos >= std::get<0>(urlItem) && pos <= std::get<1>(urlItem);
	});
	if (found == cache.at(blockId).cend())
		return "";
	return std::get<2>(*found);
}




