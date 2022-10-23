/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int points(string str) {
    /* TODO: Implement this function. */
    if(str.size() < 4){
        return 0;
    }else{
        return str.size() - 3;
    }
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */

Set<GridLocation> generateValidMoves(Grid<char>& board,GridLocation cur){
    Set<GridLocation> neighbors;
    //潜在的前进方向
    Set<GridLocation> potentialNeighbors;
    for(int i = -1;i < 2;i++){
        for(int j = -1; j < 2; j++){
            potentialNeighbors.add(GridLocation(cur.row+i, cur.col+j));
        }
    }
    potentialNeighbors.remove(cur);
    for (GridLocation elem : potentialNeighbors) {
        if(board.inBounds(elem))
            neighbors.add(elem);
    }
    return neighbors;
}

//从某个位置出发，看看有没有合适的单词
int scoreBoardRec(Grid<char>& board, Lexicon& lex, GridLocation cur, string sofar,Set<GridLocation> usedLocation,Set<string> &usedWord) {
    /* TODO: Implement this function. */
    int result = 0;
    Set<GridLocation> neighbors = generateValidMoves(board,cur);
    sofar += board.get(cur);
    if(neighbors.size() == 0){
        if(!usedWord.contains(sofar)){
            //cout<<sofar<<endl;
            usedWord.add(sofar);
            return points(sofar);
        }else {
            return 0;
        }
    }else{
        if(lex.contains(sofar)){
            if(!usedWord.contains(sofar)){
                //cout<<sofar<<endl;
                usedWord.add(sofar);
                result += points(sofar);
            }
        }
        usedLocation.add(cur);
        if(lex.containsPrefix(sofar)){
            for(GridLocation elem : neighbors){
                if(!usedLocation.contains(elem)){
                     result += scoreBoardRec(board,lex,elem,sofar,usedLocation,usedWord);
                }
            }
        }
    }
    return result;
}

int scoreBoard(Grid<char>& board, Lexicon& lex) {
    /* TODO: Implement this function. */
    int result = 0;
    Set<string> usedWord;
    for(int i = 0; i < board.numCols(); i++){
        for(int j = 0;j < board.numRows();j++){
            result += scoreBoardRec(board,lex,{i,j},"",{},usedWord);
        }
    }
    return result;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}


/****************************Mytest***************************/

STUDENT_TEST("Test generateValidMoves") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    Set<GridLocation> neighbors;
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 1}};
    neighbors = generateValidMoves(board,{0,0});
    EXPECT_EQUAL(expected, neighbors);

    expected = {{0, 0}, {0, 1}, {0, 2},{1,0},{1,2},{2,0},{2,1},{2,2}};
    neighbors = generateValidMoves(board,{1,1});
    EXPECT_EQUAL(expected, neighbors);

    expected = {{0, 2}, {0, 3}, {1, 2},{2,2},{2,3}};
    neighbors = generateValidMoves(board,{1,3});
    EXPECT_EQUAL(expected, neighbors);
}

























