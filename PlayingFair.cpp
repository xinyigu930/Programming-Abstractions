/* File: PlayingFair.cpp
 *
 * This program simulates the senarios where two teams take order to play games
 * and generates two sequences A-sequences and B-sequences. A-sequences give slight
 * advantage to team A, and the same with B-sequences to team B.
 */
#include "PlayingFair.h"
#include "GUI/SimpleTest.h"
#include "error.h"
using namespace std;

string aSequenceOfOrder(int n) {
    if (n < 0) {
        error("a string containing your error message");
    }
    else if (n == 0){
        return "A";
    }
    else return aSequenceOfOrder(n-1)+bSequenceOfOrder(n-1);
}

string bSequenceOfOrder(int n) {
    if (n < 0) {
        error("a string containing your error message");
    }
    else if (n == 0){
        return "B";
    }
    else return bSequenceOfOrder(n-1)+aSequenceOfOrder(n-1);
}




/* * * * * * Provided Test Cases * * * * * */

PROVIDED_TEST("Sequences of order 3 are correct.") {
    /* Some very basic checks. */
    EXPECT_EQUAL(aSequenceOfOrder(3), "ABBABAAB");
    EXPECT_EQUAL(bSequenceOfOrder(3), "BAABABBA");
}

PROVIDED_TEST("Only characters should be As and Bs.") {
    for (int i = 0; i < 10; i++) {
        for (char ch: aSequenceOfOrder(i)) {
            EXPECT(ch == 'A' || ch == 'B');
        }
    }
}

PROVIDED_TEST("A-sequence of positive order should have equal As and Bs.") {
    /* Exclude the sequence of order 0, which is just a single character. */
    for (int i = 1; i < 10; i++) {
        int as = 0;
        int bs = 0;
        for (char ch: aSequenceOfOrder(i)) {
            if (ch == 'A') as++;
            else bs++;
        }

        EXPECT_EQUAL(as, bs);
    }
}

PROVIDED_TEST("Triggers error on negative inputs.") {
    /* The EXPECT_ERROR macro expects the given expression to call error(). Remember that
     * you need to guard against invalid inputs.
     */
    EXPECT_ERROR(aSequenceOfOrder(-137));
    EXPECT_ERROR(bSequenceOfOrder(-137));
}

STUDENT_TEST("Check the base case when n is 0") {
    EXPECT_EQUAL(aSequenceOfOrder(0), "A");
    EXPECT_EQUAL(bSequenceOfOrder(0), "B");
}


STUDENT_TEST("Check the length of the sequences") {
    EXPECT_EQUAL(aSequenceOfOrder(15).size(), exp2(15));
    EXPECT_EQUAL(bSequenceOfOrder(10).size(), exp2(10));
}








