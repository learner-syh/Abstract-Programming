// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readTextFile(string filename,Vector<string> &text) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    readEntireFile(in, text);
 }

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
string cleanToken(string s)
{
    char ls;
    string result = "";
    bool index = 0;
    //将s中的前后符号去除,这里不太明白erase用front和back为啥不行
    while(ispunct(s.front())){
        s.erase(s.begin());
    }
    while(ispunct(s.back())){
        s.erase(s.end()-1);
    }
    for(int i = 0; i < s.length(); i++){
        ls = s[i];

        if(isalpha(ls)){
            result += tolower(ls);
            index = 1;
        }
        else{
            result += ls;
        }

    }
    if(!index){
        return "";
    }
    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> v = stringSplit(text, " ");
    for(string elem : v){
        if(cleanToken(elem) != ""){
            tokens.add(cleanToken(elem));
        }
    }
    return tokens;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    //读取网页文件
    int tag = 1;//指示器
    int times = 0;
    string URL;
    Vector<string> text;
    readTextFile(dbfile, text);
    for(string elem : text){
        if(tag == 1){
            URL = elem;
            tag = 0;
            times ++;
        }
        else{
            for(string item :gatherTokens(elem)){
                index[item].add(URL);
                tag = 1;
            }
        }
    }
    return times;
}

// TODO: Add a function header comment here to explain the
// behavior  of the function and how you implemented this behavior
//这个函数我把问题想复杂了，我处理的情况是' '和'+''-'在同一级别，按照题目应该是用所有都用' '分隔，这样的话用stringsplit就很好实现
//我先将就这个用了
//Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
//{
//    Set<string> result;
//    // TODO: your code here
//    //首先处理搜索文本
//    string container = "";
//    int temp = 0;
//    int temp1 = 0;
//    for (int i = 0; i < query.length(); i++) {
//        //第一个搜索
//        if((query[i] == ' ' || query[i] == '+' || query[i] == '-' || i == query.length()-1) && result.size() == 0){
//            //末尾和第一次碰到分隔符处理方式不一样
//            if(i == query.length()-1){
//                container = query.substr(temp,i+1);
//            }
//            else {
//                container = query.substr(temp,i);
//            }
//            result = index[cleanToken(container)];
//            temp = i + 1;
//        }
//        if(query[i] == ' '){
//            temp = i + 1;
//            temp1 = temp;
//            while(!(temp1 == ' ' || temp1 == '+' || temp1 == '-' || temp1 == query.length())){
//                temp1++;
//            }
//            container = query.substr(temp,temp1);
//            result = result.unionWith(index[cleanToken(container)]);
//        }
//        if(query[i] == '+'){
//            temp = i + 1;
//            temp1 = temp;
//            while(!(temp1 == ' ' || temp1 == '+' || temp1 == '-' || temp1 == query.length())){
//                temp1++;
//            }
//            container = query.substr(temp,temp1);
//            result = result.intersect(index[cleanToken(container)]);
//        }
//        if(query[i] == '-'){
//            temp = i + 1;
//            temp1 = temp;
//            while(!(temp1 == ' ' || temp1 == '+' || temp1 == '-' || temp1 == query.length())){
//                temp1++;
//            }
//            container = query.substr(temp,temp1);
//            result = result.difference(index[cleanToken(container)]);
//        }
//    }

//    return result;
//}

Set<string> findQueryMatches(Map<string, Set<string>>& index, string query){
    Set<string> result;
    Vector<string> purQuery = stringSplit(query,' ');
    for(string elem : purQuery){
        if(elem[0] == '+' || elem[0] == '-'){
            if(elem[0] == '+'){
                result = result.intersect(index[cleanToken(elem)]);
            }
            if(elem[0] == '-'){
                result = result.difference(index[cleanToken(elem)]);
            }
        }
        else {
            result = result.unionWith(index[cleanToken(elem)]);
        }
    }

    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    // TODO: your code here
    cout<<"Stand by while building index..."<<endl;
    Map<string, Set<string>> index;
    int nPages = buildIndex(dbfile, index);
    string query;
    Set<string> URLs;
    cout<<"Indexed"<< nPages<<" pages containing "<<index.size()<< "unique terms"<<endl;
    while (true) {
        query = getLine("Enter query sentence (RETURN/ENTER to quit):");
        if (query == "")
            break;
        URLs = findQueryMatches(index,query);
        cout<<"Found "<<URLs.size()<<" matching pages"<<endl;
        cout<<URLs<<endl;
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here

STUDENT_TEST("cleanToken on strings with one or zero char") {
    EXPECT_EQUAL(cleanToken(""), "");
    EXPECT_EQUAL(cleanToken("W"), "w");
    EXPECT_EQUAL(cleanToken(".w"), "w");
}

STUDENT_TEST("cleanToken on strings with starting mutiple puncts") {
    EXPECT_EQUAL(cleanToken("...."), "");
    EXPECT_EQUAL(cleanToken("."), "");
    EXPECT_EQUAL(cleanToken("++abc123//"), "abc123");
    EXPECT_EQUAL(cleanToken("<<section>>"), "section");
}

STUDENT_TEST("gatherTokens from seuss, 1 unique words") {
    gatherTokens(";';';';' sss s a");
    Set<string> tokens = gatherTokens(";';';';' sss s a");
    EXPECT_EQUAL(tokens.size(), 3);
    EXPECT(tokens.contains("sss"));
    EXPECT(tokens.contains("a"));
}













