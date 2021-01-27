/*
 * This program plots various interesting 2D graphs!
 */

#include "Plotter.h"
#include "strlib.h"
using namespace std;



double xstart = 0;
double ystart = 0;
double xend = 0;
double yend = 0;
int pen=0;
PenStyle style = { 1, "black" };

void runPlotterScript(istream& input) {
    for (string line; getline(input, line); ) {
        Vector<string> splitResult = stringSplit(line," ");
        string command = toLowerCase(splitResult[0]);
        if (command=="pendown") {
            pen=1;
        }
        else if (command=="penup") {
            pen=0;
        }
        else if (command=="pencolor"){
            style.color=splitResult[1];
        }
        else if (command=="penwidth"){
            style.width=stringToReal(splitResult[1]);
        }
        else {
            xend=stringToReal(splitResult[1]);
            yend=stringToReal(splitResult[2]);
            if (command=="moveabs") {
                if (pen==1) {
                    drawLine(xstart, ystart, xend, yend, style);
                }
                xstart=xend;
                ystart=yend;
                }
            if (command=="moverel"){
                if (pen==1){
                    drawLine(xstart, ystart, xstart+xend, ystart+yend, style);
                }
                xstart=xstart+xend;
                ystart=ystart+yend;
        }
        }
    }
    }


