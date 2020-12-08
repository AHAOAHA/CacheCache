#include <gtest/gtest.h>
#include <thread>

#include "CacheCache.hpp"

AHAOAHA::CacheCache<int, int> cache;


TEST(PUT, CACHE) {
    bool ok = true;
    cache.SetMaxCapacity(100);
    cache.SetExprTime(5);  // 5秒过期
    for(int i = 0; i < 100; ++i) {
        ASSERT_EQ(cache.Put(i, i), true);
    }
    ASSERT_EQ(cache.Put(101, 101), false);
    sleep(5);
    int v = 0;
    ASSERT_EQ(cache.Get(101, v), false);
    ASSERT_EQ(cache.Get(2, v), false);
    std::cout << "cache size: " << cache.Size() << std::endl;
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
    return 0;
}