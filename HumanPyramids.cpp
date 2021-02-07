#include "HumanPyramids.h"
#include "GUI/SimpleTest.h"
#include "gridlocation.h"
#include "map.h"
using namespace std;

/*
 * This program calculates the weight a person carries in a human pyramid.
 */

double noRedundancy(int row, int col, int pyramidHeight, Map<GridLocation, double>& map){
    double weight = 0;
    GridLocation loc (row, col);
    if (row < 0 || col < 0 || col > row || row >= pyramidHeight) {
        error("This position is out of bound.");
    }
    else if (col == 0 && row == 0){
        return 0.0;
    }
    else if (map.containsKey(loc)){
        return map[loc];
    }
    else {
        if (col == 0) {
                weight = 1.0/2 * (noRedundancy (row - 1, col, pyramidHeight, map) + 160.0);
            }
        else if (row == col) {
                weight = 1.0/2 * (noRedundancy (row - 1, col - 1, pyramidHeight, map) + 160.0);
        } else {
                weight = 1.0/2 * (noRedundancy (row - 1, col, pyramidHeight, map) + 160.0) +
                         1.0/2 * (noRedundancy (row - 1, col - 1, pyramidHeight, map) + 160.0);
        }
        map[loc] = weight;
        return weight;
    }
}


double weightOnBackOf(int row, int col, int pyramidHeight) {
    Map<GridLocation, double> existingval;
    return noRedundancy (row, col, pyramidHeight, existingval);
}






/* * * * * * Test Cases * * * * * */


STUDENT_TEST("Function reports errors when column number is bigger than the row."){
    EXPECT_ERROR(weightOnBackOf(8, 9, 10));
}

STUDENT_TEST("Small row numbers.") {
    EXPECT_EQUAL(weightOnBackOf(0, 0, 2), 0);
    EXPECT_EQUAL(weightOnBackOf(1, 0, 2), 80);
    EXPECT_EQUAL(weightOnBackOf(1, 1, 2), 80);
}

STUDENT_TEST("Carries one person on back.") {
    EXPECT_EQUAL(weightOnBackOf(5, 0, 6), 155);
    EXPECT_EQUAL(weightOnBackOf(5, 5, 6), 155);
    EXPECT_EQUAL(weightOnBackOf(4, 4, 6), 150);
}

STUDENT_TEST("Weight number is decimal .") {
    EXPECT_EQUAL(weightOnBackOf(6, 0, 7), 157.5);
    EXPECT_EQUAL(weightOnBackOf(6, 2, 7), 682.5);
    EXPECT_EQUAL(weightOnBackOf(7, 2, 8), 728.75);
}




/* * * * * * Test cases from the starter files below this point. * * * * * */

PROVIDED_TEST("Check Person E from the handout.") {
    /* Person E is located at row 2, column 1. */
    EXPECT_EQUAL(weightOnBackOf(2, 1, 5), 240);
}

PROVIDED_TEST("Function reports errors in invalid cases.") {
    EXPECT_ERROR(weightOnBackOf(-1, 0, 10));
    EXPECT_ERROR(weightOnBackOf(10, 10, 5));
    EXPECT_ERROR(weightOnBackOf(-1, 10, 20));
}

PROVIDED_TEST("Stress test: Memoization is implemented.") {
    /* Do not delete anything below this point. :-) */

    /* This will take a LONG time to complete if memoization isn't implemented.
     * We're talking "heat death of the universe" amounts of time. :-)
     */
    EXPECT(weightOnBackOf(100, 50, 200) >= 10000);
}

STUDENT_TEST("Large row numbers with column being 0.") {
    EXPECT(weightOnBackOf(120, 0, 121 ) > 158 );
    EXPECT(weightOnBackOf(300, 0, 400) > 158);
}

STUDENT_TEST("Stress test-memorization is implemented") {
    EXPECT(weightOnBackOf(400, 250, 510 ) > 15000 );
}


