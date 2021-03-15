#include "Labyrinth.h"
#include "GUI/SimpleTest.h"
#include "Demos/MazeGenerator.h"
#include "set.h"
#include <string>

using namespace std;

/* This function returns whether you can escape the maze, given a starting
 * location and a sequence of steps. Successful escape requires you to find
 * the Potion, the Spellbook, and the Wand along the way and only move within
 * the boundaries of the maze.
 */
bool isPathToFreedom(MazeCell* start, const string& moves) {
    Set<Item> Items;
    for (int i = 0; i <= (int)moves.size() ; i++) {
        /* Before taking each step, checks if the current location contains an item;
         * after taking the final step, checks the last position one more time.
         */
        if (start->whatsHere != Item::NOTHING) {
            Items += start->whatsHere;
        }
        if (i != (int)moves.size()) {
            /* If it's a legal move, take the step. */
            if (moves[i] == 'E' && start->east != nullptr) {
                start = start->east;
            }   else if (moves[i] == 'S' && start->south != nullptr) {
                start = start->south;
            }   else if (moves[i] == 'W' && start->west != nullptr) {
                start = start->west;
            }   else if (moves[i] == 'N' && start->north != nullptr) {
                start = start->north;
            }
            /* Otherwise, the given sequence doesn't work. */
            else return false;
        }
    }
    /* After completing the steps, checks if all three items are collected. */
    return (Items.size() == 3) ? true: false;
}





/* * * * * * Test Cases Below This Point * * * * * */

/* Optional: Add your own custom tests here! */














/* * * * * Provided Tests Below This Point * * * * */

/* Utility function to free all memory allocated for a maze. */
void deleteMaze(const Grid<MazeCell*>& maze) {
    for (auto* elem: maze) {
        delete elem;
    }
    /* Good question to ponder: why don't we delete maze, or empty maze, or something
     * along those lines?
     */
}

PROVIDED_TEST("Checks paths in the sample maze.") {
    auto maze = toMaze({"* *-W *",
                        "| |   |",
                        "*-* * *",
                        "  | | |",
                        "S *-*-*",
                        "|   | |",
                        "*-*-* P"});

    /* These paths are the ones in the handout. They all work. */
    EXPECT(isPathToFreedom(maze[2][2], "ESNWWNNEWSSESWWN"));
    EXPECT(isPathToFreedom(maze[2][2], "SWWNSEENWNNEWSSEES"));
    EXPECT(isPathToFreedom(maze[2][2], "WNNEWSSESWWNSEENES"));

    /* These paths don't work, since they don't pick up all items. */
    EXPECT(!isPathToFreedom(maze[2][2], "ESNW"));
    EXPECT(!isPathToFreedom(maze[2][2], "SWWN"));
    EXPECT(!isPathToFreedom(maze[2][2], "WNNE"));

    /* These paths don't work, since they aren't legal paths. */
    EXPECT(!isPathToFreedom(maze[2][2], "WW"));
    EXPECT(!isPathToFreedom(maze[2][2], "NN"));
    EXPECT(!isPathToFreedom(maze[2][2], "EE"));
    EXPECT(!isPathToFreedom(maze[2][2], "SS"));

    deleteMaze(maze);
}

PROVIDED_TEST("Can't walk through walls.") {
    auto maze = toMaze({"* S *",
                        "     ",
                        "W * P",
                        "     ",
                        "* * *"});

    EXPECT(!isPathToFreedom(maze[1][1], "WNEES"));
    EXPECT(!isPathToFreedom(maze[1][1], "NWSEE"));
    EXPECT(!isPathToFreedom(maze[1][1], "ENWWS"));
    EXPECT(!isPathToFreedom(maze[1][1], "SWNNEES"));

    deleteMaze(maze);
}

PROVIDED_TEST("Works when starting on an item.") {
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "EE"));
    EXPECT(isPathToFreedom(maze[0][1], "WEE"));
    EXPECT(isPathToFreedom(maze[0][2], "WW"));

    deleteMaze(maze);
}

