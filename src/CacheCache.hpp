/*************************************************
 * Copyright(C) 2016-2019. All right reserved.
 * 
 * Filename: CacheCache.h
 * Author: ahaoozhang
 * Date: 2019-07-27 20:30:01 (星期六)
 * Describe: 
 *************************************************/
#ifndef __CACHECACHE_H__
#define __CACHECACHE_H_
#include <unordered_map>
#include <rw_mutex.h>


namespace AHAOAHA {

    template<class Key, class Value>
    class CacheCache {
        public:
            CacheCache() {}

            ~CacheCache() {}

            bool Get(const Key& k, Value& v) {
                _rw_mtx.r_lock();

                // find key
                auto got = _cache_map.find(k);
                if (got == _cache_map.end()) {
                    // current cache have no key
                    _rw_mtx.r_unlock();
                    return false;
                }

                // add key value
                v = _cache_map[k];
                _rw_mtx.r_unlock();
                return true;    
            }

            bool Put(Key k, Value v) {
                //write lock
                _rw_mtx.w_lock();
                _cache_map[k] = v;
                _rw_mtx.w_unlock();
                return true;
            }

            //bool SetMaxMem(uint64_t max_mem) {
            //    _max_mem = max_mem;
            //}
            //bool SetExprTime(uint64_t expr_time/*second*/) {
            //    _expr_time = expr_time;
            //}

        private:

            std::unordered_map<Key, Value> _cache_map; //cache
            AHAOAHA::rw_mutex _rw_mtx;   //rw lock
    };
}


#endif //cachecache.h
