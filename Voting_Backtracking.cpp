/*
 * This program will take a set of voting blocks and return the
 * Banzhaf Power Index: that is the percentage of the total critical votes
 * of the set that each block holds
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "SimpleTest.h"
using namespace std;

/* This helper function adds up all the values in the blocks and returns
 * it halfed.
 */
int halfBlocks(Vector<int> &blocks) {
    int check = 0;
    for (int block : blocks) {
        check += block;
    }
    check = check / 2;
    return check;
}


/* The 'computeCritical' function takes a removed block and calculates the number of
 * critical votes the selected block has.
 */

int computeCritical(Vector<int> &blocks, int blockRemoved, int sum, int i, int majority) {
    if (sum > majority) {
        return 0;
    }
    if (i > blocks.size() - 1) {
        if (sum + blockRemoved > majority) {
            return 1;
        } else {
            return 0;
        }
    }
    else {
        int withBlock = computeCritical(blocks, blockRemoved, sum + blocks[i], i + 1, majority);
        int withoutBlock = computeCritical(blocks, blockRemoved, sum, i + 1, majority);
        int criticalVote = withBlock + withoutBlock;
        return criticalVote;
    }
}


/* The 'computerPowerIndexes' will create the Banzhaf Power Index for an inputed
 * vector of blocks.
 */
Vector<int> computePowerIndexes(Vector<int> &blocks) {
    int majority = halfBlocks(blocks);
    int total = 0;
    Vector<int> result;
    Vector<int> realResult;
    for (int i = 0; i < blocks.size(); i++) {
        int blockRemoved = blocks.remove(i);
        int criticalVote = computeCritical(blocks, blockRemoved, 0, 0, majority);
        result.add(criticalVote);
        blocks.insert(i, blockRemoved);
        }
    for (int criticalVote : result) {
        total += criticalVote;
    }
    for (int criticalVote : result) {
        realResult += (criticalVote * 100) / total;
    }
    return realResult;
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

STUDENT_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55};
    Vector<int> expected = {100};
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

STUDENT_TEST("Time power index operation over 5 different sizes") {
    Vector<int> blocks;
    for (int i = 1; i < 10; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    blocks.clear();
    for (int i = 1; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    blocks.clear();
    for (int i = 1; i < 20; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    blocks.clear();
    for (int i = 1; i < 25; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    blocks.clear();
    for (int i = 1; i < 30; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

