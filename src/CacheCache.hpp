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
        private:
            struct CacheValue {
                //everykey with a expr_time
                CacheValue(Value val, int64_t expr_time) {
                    _value = val;
                    _val_expr_time = expr_time;
                    _in_time = time();
                }

                ~CacheValue() {}
                Value _value;
                int64_t _val_expr_time;
                time_t  _in_time;   //time_t -> long int
            }

            bool is_expr(const CacheValue& cacheval) {
                //never expr
                if (cacheval._val_expr_time < 0) {
                    return true;
                }

                time_t now = time();
                return (now - cacheval._in_time) <= cacheval._val_expr_time ? true : false;
            }

        public:
            CacheCache() :_expr_time(-1), _max_mem(-1)
            {}

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

                if (is_expr(_cache_map[k]) == false) {
                    v = _cache_map[k]._val;
                    return false;
                }

                // add key value
                v = _cache_map[k]._val;
                _rw_mtx.r_unlock();
                return true;    
            }

            bool Put(Key k, Value v) {
                //is max mem
                if (_max_mem >= 0 && _cache_map.size() >= _max_mem) {
                    return false;
                }

                CacheVal cacheval(v, _expr_time);
                //write lock
                _rw_mtx.w_lock();

                //every time fflush expr_time
                _cache_map[k] = cacheval;

                _rw_mtx.w_unlock();
                return true;
            }

            bool SetMaxMem(uint64_t max_mem) {
                _max_mem = max_mem;
            }

            bool SetExprTime(uint64_t expr_time/*second*/) {
                _expr_time = expr_time;
            }

        private:
            std::unordered_map<Key, CacheValue> _cache_map; //cache
            AHAOAHA::rw_mutex _rw_mtx;   //rw lock
            int64_t _max_mem;   //mem
            int64_t _expr_time; //time s
    };
}


#endif //cachecache.h
