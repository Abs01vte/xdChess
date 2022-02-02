/*
 * Main program.
 * Copyright
 *     (C) 2022 Kyle McDermott and Ryan Jeffrey
 *
 * This file is part of XDchess.
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>

// Move mode.
enum moveMode {
    // Alternate player moves from input (w->b->w->b...).
    ALTERNATING,
    // Split input down the middle, white gets first half, black second.
    MIDDLE
};

// The move mode. Default to alternating.
enum moveMode mode = ALTERNATING;
// Maximum amount of moves to derive from a file before stopping.
size_t maxMoves = SIZE_MAX;
// Maximum amount of bytes to read from a file before stopping.
size_t maxBytes = SIZE_MAX;
// First file input.
FILE* file1 = NULL;
// Second file input.
FILE* file2 = NULL;

// TODO return false on error.
/*
 *  Parse command line arguments and set global settings.
 *  argc: Size of the argument vector.
 *  argv: Argument vector.
 */
void parseCMDArgs(int argc, const char * const *argv) {
    //TODO: files with dashes
    for(int i = 1; i < argc; i++) {
        const char* curArg = argv[i];
        if(curArg[0] == '-') {
            // Check if just a -, meaning stdin.
            if(curArg[1] == '\0') {
                if(file1 == NULL) {
                    file1 = stdin;
                }
                else if(file2 == NULL) {
                    file2 = stdin;
                }
                else { // Error, both files already set.
                    fprintf(stderr, "Error: given too many files. Exiting.\n");
                }
            }
        }
        else {
            if(file1 == NULL) {
                file1 = fopen(curArg, "r");
                if(file1 == NULL) {
                    fprintf(stderr, "Error when opening %s:", curArg);
                    perror(NULL);
                }
            }
            else if(file2 == NULL) {
                file2 = fopen(curArg, "r");
                if(file2 == NULL) {
                    fprintf(stderr, "Error when opening %s:", curArg);
                    perror(NULL);
                }
            }
            else { // Error, both files already set.
                fprintf(stderr, "Error: given too many files. Exiting.\n");
            }
        }
    }
    if(file1 == NULL) {
        file1=stdin;
    }
}

/*
 * Main function. Init's xdchess.
 * argc: Argument vector size.
 * argv: Argument vector.
 * return: 0 on success, 1 on initialization failure.
 */
int main(int argc, const char * const *argv) {
    parseCMDArgs(argc, argv);
    char buff[512];
    for(size_t bytesRead = 0; bytesRead < sizeof(buff);
        bytesRead += strlen(buff + bytesRead)) {
        char* retVal = fgets(buff + bytesRead, sizeof(buff) - bytesRead,
                             file1);
        if(retVal == NULL) {
            break;
        }
    }
    printf(buff);
    return 0;
}
