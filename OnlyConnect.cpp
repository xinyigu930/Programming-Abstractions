/* File: OnlyConnect.cpp
 *
 * This program takes asks the user to enter a string, and then returns a transformed string
 * where only consonant letters remain, and the consonant letters are capitalized.
 *
 * Puzzle!--THGDFTHR
 */
#include "OnlyConnect.h"
#include "GUI/SimpleTest.h"
using namespace std;

string onlyConnectize(string phrase) {
   if (phrase == ""){
       return "";
   }
   char c = toUpperCase(phrase[0]);
   if (isalpha(c) && !(stringContains("AEIOUY", charToString(c)))){
            return toUpperCase(phrase[0]) + onlyConnectize(phrase.substr(1));
   }
   else {
       return onlyConnectize(phrase.substr(1));
   }
}




/* * * * * * Provided Test Cases * * * * * */

PROVIDED_TEST("Converts lower-case to upper-case.") {
    EXPECT_EQUAL(onlyConnectize("lowercase"), "LWRCS");
    EXPECT_EQUAL(onlyConnectize("uppercase"), "PPRCS");
}

PROVIDED_TEST("Handles non-letter characters properly.") {
    EXPECT_EQUAL(onlyConnectize("2.718281828459045"), "");
    EXPECT_EQUAL(onlyConnectize("'Hi, Mom!'"), "HMM");
}

PROVIDED_TEST("Handles single-character inputs.") {
    EXPECT_EQUAL(onlyConnectize("A"), "");
    EXPECT_EQUAL(onlyConnectize("+"), "");
    EXPECT_EQUAL(onlyConnectize("Q"), "Q");
}

STUDENT_TEST("Handles empty string") {
    EXPECT_EQUAL(onlyConnectize(""), "");
}

STUDENT_TEST("Handles a combination of lowercase, uppercase letters and non-letter characters") {
    EXPECT_EQUAL(onlyConnectize("'Cs106B rocks!!'"), "CSBRCKS");
    EXPECT_EQUAL(onlyConnectize("'Year 2021, Mon. Jan.'"), "RMNJN");
}




