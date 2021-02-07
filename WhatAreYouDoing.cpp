#include "WhatAreYouDoing.h"
#include "GUI/SimpleTest.h"
#include "strlib.h"
#include <cctype>
using namespace std;

/*
 * This program outputs all the ways of capitalizing the words in a given input sentence
 * without changing the word orders.
 */

void allEmphasesofRec(Vector<string>& vec, int index, Set<string>& result) {
    if (index == vec.size()){
        string str = stringJoin(vec, "");
        result.add(str);
    }
    /* If it's not a letter, skip it. */
    else if (!isalpha(vec[index][0])){
        allEmphasesofRec(vec, index+1, result);
    }
    else {
    vec[index] = toLowerCase(vec[index]);
    allEmphasesofRec(vec, index+1, result);
    vec[index] = toUpperCase(vec[index]);
    allEmphasesofRec(vec, index+1, result);
    }
}


Set<string> allEmphasesOf(const string& sentence) {
    Vector<string> vec = tokenize(sentence);
    Set<string> result;
    allEmphasesofRec(vec, 0, result);
    return result;
}



/* * * * * * Test Cases * * * * * */

STUDENT_TEST("Two sentences whose only difference is initial capitalization.") {
    EXPECT_EQUAL(allEmphasesOf("Have a nice day!!"), allEmphasesOf("hAVE A nIcE DAy!!"));
}

STUDENT_TEST("The sentence is an empty string") {
    Set<string> expected = {
        ""
    };

    EXPECT_EQUAL(allEmphasesOf(""), expected);
}

STUDENT_TEST("The sentence doesn't have any word.") {
    Set<string> expected = {
        "8apples != 5pear) "
    };

    EXPECT_EQUAL(allEmphasesOf("8apples != 5pear) "), expected);
}



/* * * * * * Test cases from the starter files below this point. * * * * * */

PROVIDED_TEST("Provided Test: Enumerates all options in a simple case.") {
    Set<string> expected = {
        "hello",
        "HELLO",
    };

    EXPECT_EQUAL(allEmphasesOf("Hello"), expected);
}

PROVIDED_TEST("Provided Test: Each option has the right length.") {
    string sentence = "Hello, world!";
    for (string option: allEmphasesOf(sentence)) {
        EXPECT_EQUAL(option.length(), sentence.length());
    }
}

PROVIDED_TEST("Provided Test: Enumerates all options in a more typical case.") {
    Set<string> expected = {
        "you are?",
        "you ARE?",
        "YOU are?",
        "YOU ARE?"
    };

    EXPECT_EQUAL(allEmphasesOf("You Are?"), expected);
}

PROVIDED_TEST("Provided Test: Stress Test: Recursion only branches on words.") {
    /* We're producing a string consisting of fifty copies of the * character. This tokenizes
     * into fifty separate stars. A star is the same whether or not you capitalize it - there
     * is no such thing as an "upper-case" or "lower-case" star. Therefore, your code should
     * not try to form two versions of the sentence, one with the star capitalized and one
     * without, because the two versions will end up being the same and the work to compute
     * both options will dramatically increase the runtime.
     *
     * For reference, if you do try branching the recursion and checking what happens both if
     * you capitalize the star and if you don't, you'll try exploring 2^50 different possible
     * capitalizations. That's 1,125,899,906,842,624 options, and even doing a billion of
     * these a second is going to take over two years to generate them all! And of course,
     * that's all wasted work, since there's only one way to capitalize this sentence, and
     * that's just to leave it as-is.
     *
     * If your code is hanging when this test is running, it likely means that your code is
     * trying to enumerate all of these options. See if you can edit your code so that, if
     * you're given a non-word token, you just leave it as-is and don't change anything.
     */
    string punctuation(50, '*'); // 50 copies of *

    /* The only emphasis is itself. */
    Set<string> expected = {
        punctuation
    };

    EXPECT_EQUAL(allEmphasesOf(punctuation), expected);
}
