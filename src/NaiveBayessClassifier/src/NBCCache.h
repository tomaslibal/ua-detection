#ifndef NBCCACHE_H
#define NBCCACHE_H

#include <unordered_map>
#include <string>

class NBCCache {
private:
    int expiry = 3600;
public:
    NBCCache();
    virtual ~NBCCache();
    
    /*
     * For each key this map holds the timestamp of the cache insertion. This
     * assumes that all keys accros different cache types are unique.
     */
    std::unordered_map<std::string, int> timestamps;
    std::unordered_map<std::string, int> int_cache;
    std::unordered_map<std::string, double> dbl_cache;
    
    bool in_int_cache(std::string const& key);
    bool in_dbl_cache(std::string const& key);
    
    void insert_int_cache(std::string const& key, int val);
    void insert_dbl_cache(std::string const& key, double val);
    
    int get_int_cache(std::string const& key);
    double get_dbl_cache(std::string const& key);
};

#endif /* NBCCACHE_H */