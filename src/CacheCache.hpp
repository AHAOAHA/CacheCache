/*************************************************
 * Copyright(C) 2016-2019. All right reserved.
 * 
 * Filename: CacheCache.h
 * Author: ahaoozhang
 * Date: 2019-07-27 20:30:01 (星期六)
 * Describe: 
 *************************************************/
#pragma once
#include <unordered_map>
#include <chrono>
#include <rw_mutex.h>


namespace AHAOAHA {
    uint64_t get_curr_millon_second() {
        std::chrono::system_clock::duration d = std::chrono::system_clock::now().time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
    }

    template<class Key, class Value>
    class CacheCache {
        public:
            bool Get(const Key& k, Value& v) {
                _rw_mtx.r_lock();

                // find key
                auto got = _cache_map.find(k);
                if (got == _cache_map.end()) {
                    // current cache have no key
                    _rw_mtx.r_unlock();
                    return false;
                }

                if (get_curr_millon_second() - _cache_map.at(k)._create_time_stamp > _expr_time) {
                    return false;
                }

                // add key value
                v = _cache_map.at(k)._v;
                _rw_mtx.r_unlock();
                return true;
            }

            bool Put(Key k, Value v) {
                //write lock
                cacheValue cv;
                cv._v = v;
                _rw_mtx.w_lock();
                _cache_map[k] = cv;
                _rw_mtx.w_unlock();
                return true;
            }

            bool SetMaxCapacity(uint64_t max_capacity) {
                _rw_mtx.w_lock();
                if (_max_capacity < max_capacity) {
                    return false;
                }
                _max_capacity = max_capacity;
                _rw_mtx.w_unlock();
                return true;
            }
            bool SetExprTime(uint64_t expr_time/*second*/) {
                _rw_mtx.w_lock();
                _expr_time = expr_time;
                _rw_mtx.w_unlock();
                return true;
            }

        private:
            struct cacheValue {
                Value _v;
                uint64_t _create_time_stamp = get_curr_millon_second();
            };
            std::unordered_map<Key, cacheValue> _cache_map; //cache
            AHAOAHA::rw_mutex _rw_mtx;   //rw lock
	        uint64_t _max_capacity = UINT64_MAX;
            uint64_t _expr_time = UINT64_MAX;
    };
}

