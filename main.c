#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @author Sotirios Pupakis xpupak01
 * @date 18.11.2019
 * @name Projekt 3
 */

typedef struct {
    int rows;
    int cols;
    unsigned char *cells;
} Map;

void printHelp();   // Prints help text

Map loadStructure(FILE *file); // Prepares structure to be used
void destroyMap(Map *map);   // Frees memory allocated by map


int main(int argc, char *argv[]) {
    // Check if any arguments were provided
    if (argc > 1) {
        // Check provided arguments and decide what to do
        if (strcmp(argv[1], "--help") == 0) {
            // Prints help text
            printHelp();
        } else if (strcmp(argv[1], "--test") == 0) {
            // Check if third argument was provided
            if (argc < 3) {
                // Prints error and exits
                fputs("No third argument provided\n", stderr);
                return -1;
            }
            // TODO Implement test code
            // Check file
            FILE *fptr = fopen(argv[3], "r");
            if (fptr == NULL) {
                fputs("File could't be opened", stderr);
            }
            // Load into structure
            Map map = loadStructure(fptr);
            // Check for errors
            if (errno == 1) {
                fputs("Error while creating structure", stderr);
                return 1;
            }
            // Close file
            fclose(fptr);
            // Check structure if valid
        } else if (strcmp(argv[1], "--rpath") == 0) {
            // Check if third argument was provided
            if (argc < 2) {
                // Prints error and exits
                fputs("No third argument provided\n", stderr);
                return -1;
            }
            // TODO Implement rpath code
        } else if (strcmp(argv[1], "--lpath") == 0) {
            // Check if third argument was provided
            if (argc < 4) {
                // Prints error and exits
                fputs("No third argument provided\n", stderr);
                return -1;
            }
            // TODO Implement lpath code
        } else if (strcmp(argv[1], "--shortest") == 0) {
            // Check if third argument was provided
            if (argc < 4) {
                // Prints error and exits
                fputs("No third argument provided\n", stderr);
                return -1;
            }
            // TODO Implement shortest code
        } else {
            char *str = "Invalid flags!\n"
                        "Reffer to help\n"
                        "./proj3 --help\n";
            fputs(str, stderr);
            return -2;
        }
    } else {
        printHelp();
    }
    return 0;
}

Map loadStructure(FILE *file) {
    // Get dimensions
    int a = 0;
    int b = 0;
    // Check dimensions
    if ()
    // Create map with given dimensions

    // Check creation
    if (errno == 1) {
        destroyMap(&map);
        return ;
    }
}

/**
 * This function print's help for this program and how to use it
 */
void printHelp() {
    char *str = "Help for proj3\n\n"
                "Program usage: ./proj3 [FLAG] R C [FILE]\n"
                "R - x coordinate of start\n"
                "C - Y coordinate of start\n\n"
                "Available flags:\n"
                "--help             Prints help text\n"
                "--test             Tests validity of provided file\n"
                "--rpath            Finds exit based on right hand rule\n"
                "--lpath            Find exit based in left hand rule\n"
                "--shortest         Finds exit that is closest to the start\n";
    printf("%s", str);
}



// Help methods for Map
/**
 * Function that creates map, allocated required memory and fills with e
 * @param a rows dimension
 * @param b cols dimension
 * @return fully alocated map filled with 'e'
 */
Map createMap(int a, int b) {
    Map map;
    map.rows = a;
    map.cols = b;

    // Calculate size and allocate it
    int size = (int) sizeof(unsigned char) * a * b;
    map.cells = malloc(size);

    // Check allocation and fill
    if (map.cells != NULL) {
        for (int i = 0; i < a * b; ++i) {
            map.cells[i] = 'e';
        }
    } else {
        errno = 1;
    }
    return map;
}

void destroyMap(Map *map) {
    map->cols = -1;
    map->rows = -1;
    free(map->cells);
}