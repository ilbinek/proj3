#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

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

// Enum
typedef enum {
    LEFT = 0,
    RIGHT = 1,
    MIDDLE = 2
} Border;

void printHelp();   // Prints help text

Map loadStructure(FILE *file); // Prepares structure to be used
Map createMap(int a, int b); // Creates map, allocates required memory
void destroyMap(Map *map);   // Frees memory allocated by map
void fillMap(Map *map, FILE *file); // Fills cells with chars in file
unsigned char getCell(Map *map, int i); // Returns char that is on desired position
void printMap(Map *map); // Prints map

bool isBorder(Map *map, int r, int c, Border border);
bool testMap(Map *map); // Tests map if valid

int main(int argc, char *argv[]) {
    // Check if any arguments were provided
    if (argc > 1) {
        // Check provided arguments and decide what to do
        if (strcmp(argv[1], "--help") == 0) {
            // Prints help text
            printHelp();
        } else if (strcmp(argv[1], "--test") == 0) {
            // Check if third argument was provided
            if (argc < 2) {
                // Prints error and exits
                fputs("No third argument provided\n", stderr);
                return -1;
            }
            // Check file
            FILE *fptr = fopen(argv[2], "r");
            if (fptr == NULL) {
                fputs("File could't be opened\n", stderr);
                return -1;
            }
            // Load into structure
            Map map = loadStructure(fptr);
            // Close file
            fclose(fptr);
            // Check for errors
            if (errno == 1) {
                fputs("Invalid", stdout);
                destroyMap(&map);
                return 0;
            }
            //printMap(&map);
            // Check structure if valid
            bool valid = testMap(&map);
            if (valid) {
                printf("Valid\n");
            } else {
                printf("Invalid\n");
            }
            // Clean up
            destroyMap(&map);
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

/**
 * Tests map if valid
 * @param map Map to be tested
 * @return Returns if map is valid
 */
bool testMap(Map *map) {
    // Check right sides - leave last from column
    for (int i = 0; i < map->rows; ++i) {
        for (int j = 0; j < map->cols - 1; ++j) {
            if (isBorder(map, i, j, RIGHT) != isBorder(map, i, j + 1, LEFT)) {
                return false;
            }
        }
    }

    // Check middle - leave last row
    for (int i = 0; i < map->rows - 1; ++i) {
        for (int j = 0; j < map->cols; ++j) {
            // If on top check only the ones aiming up
            if (i == 0) {
                if ((i + j) % 2 == 0) { // Aiming down
                    // Skip
                } else {
                    // Check middle with the one lower
                    if (isBorder(map, i, j, MIDDLE) != isBorder(map, i + 1, j, MIDDLE)) {
                        return false;
                    }
                }
            } else {
                if ((i + j) % 2 == 0) { // Aiming down
                    // Check middle with the one upper
                    if (isBorder(map, i, j, MIDDLE) != isBorder(map, i - 1, j, MIDDLE)) {
                        return false;
                    }
                } else {
                    // Check middle with the one lower
                    if (isBorder(map, i, j, MIDDLE) != isBorder(map, i + 1, j, MIDDLE)) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

/**
 * Check if desired border is solid or transparent
 * @param map Map to be checked
 * @param r Row coordinate of desire cell
 * @param c Coll coordinate of desired cell
 * @param border Border to be checked
 * @return Returns if desired border is solid
 */
bool isBorder(Map *map, int r, int c, Border border) {
    if (border > 2 || border < 0) {
        return false;
    }

    char cell = (char) getCell(map, r * map->cols + c);
    switch (border) {
        case LEFT:
            if (cell == '1' || cell == '3' || cell == '5' || cell == '7') {
                return true;
            } else {
                return false;
            }

        case RIGHT:
            if (cell == '2' || cell == '3' || cell == '6' || cell == '7') {
                return true;
            } else {
                return false;
            }

        case MIDDLE:
            if (cell == '4' || cell == '5' || cell == '6' || cell == '7') {
                return true;
            } else {
                return false;
            }

        default:
            return false;
    }
}

Map loadStructure(FILE *file) {
    // Get dimensions
    char ac;
    char bc;

    fscanf(file, "%c", &ac);
    int a = ac - '0';
    fscanf(file, "%c", &bc);
    fscanf(file, "%c", &bc);
    int b = bc - '0';

    Map map;

    // Check dimensions
    if (a < 1 || b < 1) {
        errno = 1;
        map = createMap(0, 0);
        return map;
    }

    // Create map with given dimensions
    map = createMap(a, b);

    // Check creation
    if (errno == 1) {
        destroyMap(&map);
        return map;
    }

    // Fill map with numbers
    fillMap(&map, file);

    // Returns map
    return map;
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
 * Function that creates map, allocates required memory
 * @param a rows dimension
 * @param b cols dimension
 * @return fully allocated map filled with 'e'
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

/**
 * Fills cells with chars in file
 * @param map Map that will be filled
 * @param file File that will be read from
 */
void fillMap(Map *map, FILE *file) {
    int i = 0;
    int cin = 0;
    while ((cin = fgetc(file)) != EOF) {
        unsigned char c = (unsigned char) cin;
        if (c != ' ' && c != '\n') {
            map->cells[i] = c;
            i++;
        }
    }

}

/**
 * Returns char that is on desired position
 * @param map Map to be searched
 * @param i Position of char
 * @return Char that is on desired position or x if out of bounds
 */
unsigned char getCell(Map *map, int i) {
    if (i < map->cols * map->rows) {
        return map->cells[i];
    } else {
        return 'x';
    }
}

/**
 * Frees allocated memory of a map
 * @param map Map to be freed
 */
void destroyMap(Map *map) {
    map->cols = -1;
    map->rows = -1;
    free(map->cells);
}

/**
 * Prints cells of map
 * @param map Map to be printed
 */
void printMap(Map *map) {
    for (int i = 0; i < map->rows; ++i) {
        for (int j = 0; j < map->cols; ++j) {
            printf("%c", getCell(map, i * map->cols + j));
        }
    }
    printf("\n");
}