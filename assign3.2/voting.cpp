// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "testing/SimpleTest.h"
using namespace std;

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int computePowerIndexesRec(Vector<int>& blocks,const int tag,int index,const int sum,int container)
{
    int result = 0;
    // TODO your code here
    if((container + blocks[tag]) > sum && container <= sum){
        result += 1;
    }

    for(int i = index;i < blocks.size();i++){
        if(i == tag){
            result += computePowerIndexesRec(blocks,tag,i+1,sum,container); ;
        }else {
            result += computePowerIndexesRec(blocks,tag,i+1,sum,container + blocks[i]);
        }
    }

    return result;
}



Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result;
    int sum = 0;
    int resum = 0;
    // TODO your code here
    for(int elem : blocks){
        sum += elem;
    }
    for(int i = 0;i < blocks.size();i++){
        result.add(computePowerIndexesRec(blocks,i,0,sum/2,0));
    }
    for(int item : result){
        resum += item;
    }
    for(int i = 0;i < result.size();i++){
        result[i] =  double(result[i])/resum * 100;
    }
    cout<<result<<endl;
    return result;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}


