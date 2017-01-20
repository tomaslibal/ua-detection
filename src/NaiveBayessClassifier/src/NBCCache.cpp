#include "NBCCache.h"

#define INIT_MAP_SIZE 64000

NBCCache::NBCCache()
{
    timestamps.reserve(INIT_MAP_SIZE);
    int_cache.reserve(INIT_MAP_SIZE);
    dbl_cache.reserve(INIT_MAP_SIZE);
}

NBCCache::~NBCCache()
{

}

bool NBCCache::in_int_cache(const std::string& key)
{
    auto it = int_cache.find(key);
    if (it != int_cache.end()) {
        return true;
    } else {
        return false;
    }
}

bool NBCCache::in_dbl_cache(const std::string& key)
{
    auto it = dbl_cache.find(key);
    if (it != dbl_cache.end()) {
        return true;
    } else {
        return false;
    }
}

void NBCCache::insert_int_cache(const std::string& key, int val)
{
    int_cache.insert( std::pair<std::string, int>(key, val) );
}

void NBCCache::insert_dbl_cache(const std::string& key, double val)
{
    dbl_cache.insert( std::pair<std::string, double>(key, val) );
}

int NBCCache::get_int_cache(const std::string& key)
{
    return int_cache.find(key)->second;
}

double NBCCache::get_dbl_cache(const std::string& key)
{
    return dbl_cache.find(key)->second;
}

