#include <gtest/gtest.h>
#include "CacheCache.hpp"

TEST(PUT, CACHE) {
    bool ok = true;
    AHAOAHA::CacheCache<int, int> cache;
    cache.SetMaxCapacity(1);
    ASSERT_EQ(cache.Put(1, 2), true);
    ASSERT_EQ(cache.Put(2, 3), false);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc,argv);
    return RUN_ALL_TESTS();
    return 0;
}