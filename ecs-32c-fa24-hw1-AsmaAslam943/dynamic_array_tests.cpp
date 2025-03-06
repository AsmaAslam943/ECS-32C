#include <gtest/gtest.h>

// We are using a C++ test framework to test
// C code.  We do this because this test 
// framework is so much better than the vanilla
// C equivalent.  Thus we need to 
// explicitly say we are importing a 
// C not a C++ file.
extern "C" {
    #include "dynamic_array.h"
}

// This tests that the dynamic array initial allocation and
TEST(DynamicIntArrayTests, TestAllocation) {
    DynamicIntArray *test_data = allocate_int_array();
    ASSERT_FALSE(test_data == NULL);
    EXPECT_EQ(test_data->capacity, 1) << "Initialized with capacity != 1";
    EXPECT_EQ(test_data->elements, 0) << "Initialized with elements != 0";
    append(0, test_data);
    EXPECT_EQ(test_data->capacity, 1);
    append(0, test_data);
    EXPECT_EQ(test_data->capacity, 2);
    append(0, test_data);
    EXPECT_EQ(test_data->capacity, 4) << "Checking that capacity properly doubles";
    append(0, test_data);
    EXPECT_EQ(test_data->capacity, 4) << "And it shouldn't expand again";
    deallocate_int_array(test_data);
}

TEST(DynamicIntArrayTests, TestOperation) {
    int i = 0;
    DynamicIntArray *test_data = allocate_int_array();
    ASSERT_FALSE(test_data == NULL);
    for(i = 0; i < 10000; ++i){
        EXPECT_FALSE(present(i, test_data));
        EXPECT_EQ(get_int(i, test_data), -1);
        EXPECT_TRUE(set_int(i, test_data) == NULL);
        append(i, test_data);
        EXPECT_TRUE(present(i, test_data));
        EXPECT_EQ(get_int(i, test_data), i);
    }
    for(i = 0; i < 10000; ++i){
        EXPECT_EQ(get_int(i, test_data), i);
        *set_int(i, test_data) = i * 2;
    }
    for(i = 0; i < 10000; ++i){
        EXPECT_EQ(get_int(i, test_data), i * 2);
    }

    deallocate_int_array(test_data);

}