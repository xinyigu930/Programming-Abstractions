#include "Sierpinski.h"
#include "error.h"
using namespace std;

/*
 * This program draws a sierpinski triangle given a positive number as the order number.
 */


/**
 * Draws a triangle with the specified corners in the specified window.
 *
 * @param window The window in which to draw the triangle.
 * @param x0 y0 The first corner of the triangle.
 * @param x1 y1 The second corner of the triangle.
 * @param x2 y2 The third corner of the triangle.
 */
void drawTriangle(GWindow& window,
                  double x0, double y0,
                  double x1, double y1,
                  double x2, double y2) {
    window.setColor("blue");
    window.fillPolygon({ x0, y0, x1, y1, x2, y2 });
}



void drawSierpinskiTriangle(GWindow& window,
                            double x0, double y0,
                            double x1, double y1,
                            double x2, double y2,
                            int order) {
    if (order < 0) {
        error("The order should be a non-negative number.");
    }
/* Base case: An order-0 Sierpinsk triangle is a single triangle. */
    if (order == 0) {
        drawTriangle(window, x0, y0, x1, y1, x2, y2);
    }
/* Recursive case: Draw 3 smaller triangles, each of which has
 * half side length of the main triangle.
 */
    else {
          drawSierpinskiTriangle(window, x0, y0, (x0+x1) / 2, (y0+y1) / 2,
                                 (x0+x2) / 2, (y0+y2) / 2, order - 1);
          drawSierpinskiTriangle(window, x1, y1, (x1+x0) / 2, (y1+y0) / 2,
                                 (x1+x2) / 2, (y1+y2) / 2, order - 1);
          drawSierpinskiTriangle(window, x2, y2, (x2+x1) / 2, (y2+y1) / 2,
                                 (x0+x2) / 2, (y0+y2) / 2, order - 1);
         }
}
