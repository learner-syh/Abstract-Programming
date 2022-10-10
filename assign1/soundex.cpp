/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

string lettersToNumbers(string s){
    char ls;
    string result = "";
    for(int i = 0; i < s.length(); i++){
        ls = tolower(s[i]);
        if(ls == 'a' || ls == 'e'|| ls == 'i' || ls == 'o' || ls == 'u'|| ls == 'h'|| ls == 'w'|| ls == 'y')
            result += "0";
        if(ls == 'b'|| ls == 'f'|| ls == 'p'|| ls == 'v')
            result += "1";
        if(ls == 'c'|| ls == 'g'|| ls == 'j'|| ls == 'k'|| ls == 'q'|| ls == 's'|| ls == 'x'|| ls == 'z')
            result += "2";
        if(ls == 'd'|| ls == 't')
            result += "3";
        if(ls == 'l')
            result += "4";
        if(ls == 'm'|| ls == 'n')
            result += "5";
        if(ls == 'r')
            result += "6";
    }
    return result;
}

string coalesceNumerbers(string s){
    char ls = ' ';
    string result = "";
    for(int i = 0; i < s.length(); i++){
        if(s[i] != ls){
            result += s[i];
            ls = s[i];
        }
    }
    return result;
}

string replaceFirstLetter(string ls,string ns){
    string firstChar = "";
    firstChar += toupper(ls.at(0));
    ns.replace(0,1,firstChar);
    return ns;
}

string delZero(string s){
    string result = "";
    for(int i = 0; i < s.length(); i++){
        if(s[i] != '0'){
            result += s[i];
        }
    }
    return result;
}

string standardizingSizeToFour(string s){
    string result = "";
    if(s.length() >= 4){
        for(int i = 0; i < 4; i++)
                result += s[i];
    }
    else{
        for(int i = 0; i < s.length(); i++)
            result += s[i];
        for(int i = s.length() - 1; i < 3; i++)
            result += "0";
    }
    return result;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
string soundex(string s) {
    string temp = removeNonLetters(s);
    temp = lettersToNumbers(temp);
    temp = coalesceNumerbers(temp);
    temp = replaceFirstLetter(s,temp);
    temp = delZero(temp);
    temp = standardizingSizeToFour(temp);
    return temp;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;
    string s = "";
    while((s = getLine("Enter a surname (RETURN to quit):")) != ""){
        string Soundex = soundex(s);
        Vector<string> vec;
        for (string elem: databaseNames) {
            if(soundex(elem) == Soundex)
                vec += elem;
        }
        cout<<"Soundex code is "<<Soundex<<endl;
        vec.sort();
        cout<<"Matches from database: "<<vec<<endl;
    }


    // The names in the database are now stored in the provided
    // vector named databaseNames

    /* TODO: Fill in the remainder of this function. */
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here


STUDENT_TEST("Test removing puntuation, digits, and spaces" ) {
    string s = "2yinghaosong";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "yinghaosong");
}

STUDENT_TEST("Test turn letters to numbers" ) {
    string s = "Curie";
    string result = lettersToNumbers(s);
    EXPECT_EQUAL(result, "20600");
    result = lettersToNumbers("OConner");
    EXPECT_EQUAL(result, "0205506");
}

STUDENT_TEST("Test coalesceNumerbers" ) {
    string s = "222025";
    string result = coalesceNumerbers(s);
    EXPECT_EQUAL(result, "2025");
    result = coalesceNumerbers("111");
    EXPECT_EQUAL(result, "1");
    result = coalesceNumerbers("123333");
    EXPECT_EQUAL(result, "123");
    result = coalesceNumerbers("1233344556");
    EXPECT_EQUAL(result, "123456");
}

STUDENT_TEST("replaceFirstLetter" ) {
    string ls = "curie";
    string ns = "12345";
    string result = replaceFirstLetter(ls,ns);
    EXPECT_EQUAL(result, "C2345");
}

STUDENT_TEST("delZero" ) {
    string result = delZero("0001200300");
    EXPECT_EQUAL(result, "123");
    result = delZero("232200023");
    EXPECT_EQUAL(result, "232223");
}

STUDENT_TEST("standardizingSizeToFour" ) {
    string result = standardizingSizeToFour("S12333");
    EXPECT_EQUAL(result, "S123");
    result = standardizingSizeToFour("C12");
    EXPECT_EQUAL(result, "C120");
}

STUDENT_TEST("soundex" ) {
    string result = soundex("Angelou");
    EXPECT_EQUAL(result, "A524");
    result = soundex("Curie");
    EXPECT_EQUAL(result, "C600");
}























