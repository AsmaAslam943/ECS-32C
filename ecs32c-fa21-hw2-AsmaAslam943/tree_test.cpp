#include <gtest/gtest.h>
#include <vector>
#include <random>

extern "C"
{
#include "tree.h"
#include <string.h>
// We can add in any true C functions we want here.  By being in the
// extern C block we guarantee that the 

void cat(void *key, void *data, void *context){
    char *str = (char *) context;
    strcat(str, (char *) key);
    strcat(str, "/");
    strcat(str, (char *) data);
    strcat(str, ",");
}

}

TEST(C_LIST, BasicTests)
{
    tree *t = new_tree( (int (*)(const void*, const void*)) strcmp);

    // This shows an example of creating an array (vector) of strings in 
    // C++
    std::vector<const char *> teststrings = {"foo", "bar", "baz", "garplay"};
    // And this randomizes the order of the vector
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(teststrings), std::end(teststrings), rng);

    char *tmp = (char *) malloc(5000);
    char *tmp2 = (char *) malloc(5000);
    int i;
    sprintf(tmp, "foo");

    for(i = 0; i < 4; ++i){
        insert(t, (void *) teststrings[i], (void *) teststrings[i]);
    }
    for(i = 0; i < 4; ++i){
        EXPECT_EQ(find(t, (void *) teststrings[i]), (void *) teststrings[i]);
    }
    insert(t, (void *) "foo", tmp);
    EXPECT_STREQ((char *) find(t, (void *) "foo"), "foo");

    tmp2[0] = '\0';
    traverse(t, cat, (void *)tmp2);
    EXPECT_STREQ(tmp2, "bar/bar,baz/baz,foo/foo,garplay/garplay,");

    free_tree(t);
    free(tmp);
};



