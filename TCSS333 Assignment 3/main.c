/*
 *  TCSS 333 - Autumn 2015
 *
 *  Assignment 3 - Most Popular Names
 *  Alex Terikov (teraliv@uw.edu)
 *  10/22/2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NAME_LENGTH 30
#define MAXIMUM_NAMES 400


void openFileAndReadContent(char (*names)[NAME_LENGTH], int (*ranks)[10], int *totalNames);
void writeNamesAndRanks(char *line, int *length, int rank, int comma, int *totalNames, int fileNum, char (*names)[NAME_LENGTH], int (*ranks)[10]);
void writeOutputFile(char (*allNames)[NAME_LENGTH], int (*ranks)[10], int numberOfNames);
void printResults(char (*allNames)[NAME_LENGTH], int (*ranks)[10], int numberOfNames);
int findNamePosition(char (*allNames)[NAME_LENGTH], char *theName, int numberOfNames);
int findName(char (*allNames)[NAME_LENGTH], char *theName, int numberOfNames);
int compareNames(char *first, char *second);
int findIndexOfComma(char *);
int getRank(char *line, size_t length);
void quickStrSort(char *c, char *left, char *right,
                  int length, char *limit, int (*rank)[10], char (*allNames)[NAME_LENGTH]);



int main(void) {
    
    char names[MAXIMUM_NAMES][NAME_LENGTH] = {'\0'}; // 2d array of names
    int ranks[MAXIMUM_NAMES][10] = {'\0'}; // 2d array of ranks
    
    int totalNames = 0;
    int *pTotalNames = &totalNames;
    

    openFileAndReadContent(names, ranks, pTotalNames);
    quickStrSort((char *)names, (char *)names, (char *)names + NAME_LENGTH * (totalNames - 1),
                 NAME_LENGTH, (char *)names + NAME_LENGTH * totalNames, ranks, names);
    
    printResults(names, ranks, totalNames);
    writeOutputFile(names, ranks, totalNames);
    
    return 0;
}



/*
 * This function open 10 txt files
 * and read the content of every file line by line.
 *
 * @param *names - pointer to 2d array of names
 * @param *ranks - pointer to 2d array of name ranks
 * @param *totalNames - pointer to keep track of total number of names
 */
void openFileAndReadContent(char (*names)[NAME_LENGTH], int (*ranks)[10], int *totalNames) {
    
    // open all 10 files with names
    FILE *inputFile[] = {
        inputFile[0] = fopen("yob1920.txt", "r"),
        inputFile[1] = fopen("yob1930.txt", "r"),
        inputFile[2] = fopen("yob1940.txt", "r"),
        inputFile[3] = fopen("yob1950.txt", "r"),
        inputFile[4] = fopen("yob1960.txt", "r"),
        inputFile[5] = fopen("yob1970.txt", "r"),
        inputFile[6] = fopen("yob1980.txt", "r"),
        inputFile[7] = fopen("yob1990.txt", "r"),
        inputFile[8] = fopen("yob2000.txt", "r"),
        inputFile[9] = fopen("yob2010.txt", "r"),
    };
    
    int i, currentFile = 0;
    
    char *line = NULL; // line to read from file
    size_t length = 0; // length of the line
    

    // read all files
    while (currentFile < 10) {
        
        //read only firs 100 lines
        for (i = 0; i < 100; i++) {
            
            // read current line
            getline(&line, &length, inputFile[currentFile]);
            // get the index of first comma
            int indexOfComma = findIndexOfComma(line);

            writeNamesAndRanks((char *)line, (int *)length, i, indexOfComma, totalNames, currentFile, names, ranks);
        }
        currentFile++;
    }
    
    if (line) {
        free(line); // empty memory
    }
    
    // close all files
    for (i = 0; i < 10; i++) {
        fclose(inputFile[i]);
    }
}


/*
 * This function gets the line with the name and rank
 * and writes it to the 2d arrays of names and ranks
 *
 * @param *line - pointer to the current line from a file
 * @param *length - pointer to the length of the line
 * @param *totalNames - pointer to keep track of total number of names
 * @param fileNum - current file number
 * @param *names - pointer to 2d array of names
 * @param *ranks - pointer to 2d array of name ranks
 */
void writeNamesAndRanks(char *line, int *length, int rank, int comma, int *totalNames, int fileNum, char (*names)[NAME_LENGTH], int (*ranks)[10]) {
    
    int i, row;
    char currentName[comma + 1];
    
    // write first hundred names withoud checking
    // if the are exist int the names array
    if (*totalNames < 100) {
        for (i = 0; i < comma; i++) {
            names[*totalNames][i] = line[i];
        }
        
        // write rank
//        ranks[*totalNames][fileNum] = getRank(line, (int)length);
        ranks[*totalNames][fileNum] = rank + 1;
        (*totalNames)++;
    }
    
    // make current name to compare with all names int the array
    for (i = 0; i < comma; i++) {
        currentName[i] = line[i];
    }
    currentName[comma] = '\0'; // set null character
    
    // do not go here untill write firs hundred
    // names are writen from 1st file
    if (*totalNames >= 100) {
        
        // check if the name is in the array and
        // update its rank for the given year
        for (row = 0; row < *totalNames; row++) {
            if (strcmp(names[row], currentName) == 0) {
//                ranks[row][fileNum] = getRank(line, (int)length);
                ranks[row][fileNum] = rank + 1;
            }
        }
    }
    
    // check if a name is in the list of hundred for the
    // current year and do not exist in the names array
    // then add it to the array and write it rank
    if (findName(names, currentName, *totalNames) == 1) {

        for (i = 0; i < comma; i++) {
            names[*totalNames][i] = currentName[i];
        }
        
//        ranks[*totalNames][fileNum] = getRank(line, (int)length);
        ranks[*totalNames][fileNum] = rank + 1;
        (*totalNames)++;
    }
}


/*
 * This function open writes all names and ranks
 * to the CSV file
 *
 * @param *names - pointer to 2d array of names
 * @param *ranks - pointer to 2d array of name ranks
 * @param numberOfNames - pointer to keep track of total number of names
 */
void writeOutputFile(char (*allNames)[NAME_LENGTH], int (*ranks)[10], int numberOfNames) {
    
    FILE *outputFile = fopen("summary.csv", "w");
    
    int i, j;
    char *name;
    
    
    
    for (i = 0, name = (char *)allNames; i < numberOfNames; i++, name += NAME_LENGTH) {
        fprintf(outputFile, "%s,", name);
        
        for (j = 0; j < 10; j++) {
            if (ranks[i][j] != '\0') {
                fprintf(outputFile, "%d,", ranks[i][j]);
            } else {
                fprintf(outputFile, ",");

            }
        }
        fprintf(outputFile, "\n");
    }
    
    fclose(outputFile);
}


/*
 * This function open outputs results.
 *
 * @param *names - pointer to 2d array of names
 * @param *ranks - pointer to 2d array of name ranks
 * @param numberOfNames - pointer to keep track of total number of names
 */
void printResults(char (*allNames)[NAME_LENGTH], int (*ranks)[10], int numberOfNames) {
    
    char *name;
    
    int i, j;
    
    // traverse and print the names and ranks
    for (i = 0, name = (char *)allNames; i < numberOfNames; i++, name += NAME_LENGTH) {
        printf("%2d %-13s", i, name);
        
        for (j = 0; j < 10; j++) {
            printf("%8d", ranks[i][j]);
        }
        printf("\n");
    }
}

/*
 * This function check if the given name is
 * in array of names.
 *
 * @param *names - pointer to 2d array of names
 * @param *ranks - pointer to 2d array of name ranks
 * @param numberOfNames - pointer to keep track of total number of names
 * @return - returns 0 if the name was found, otherwise returns 1
 */
int findName(char (*allNames)[NAME_LENGTH], char *theName, int numberOfNames) {

    bool found = true;
    int result = 0, i;
    
    for (i = 0; i < numberOfNames; i++) {
        if (compareNames(*(allNames + i), theName) == 0) {
            found = false;
            result = 0;
        }
    }
    
    if (found) {
        found = false;
        result = 1;
    }
    
    return result;
}


/*
 * This function check if the two given names
 * are identical.
 *
 * @param *first - pointer to the first name
 * @param *second - pointer to the second name
 * @return - returns 0 if names are identical, otherwise returns 1
 */
int compareNames(char *first, char *second) {
    
    int result;
    
    while (*first == *second && *first != '\0' && * second != '\0') {
        first++;
        second++;
    }
    
    if ((*first - *second) == 0) {
        result = 0;
    } else {
        result = 1;
    }
    
    return result;
}


/*
 * This function finds the index position
 * of the first comma in the give line of characters.
 *
 * @param *line - pointer to the line of characters
 * @return - returns index of the comma
 */
int findIndexOfComma(char *line) {
    int index = 0;
    
    while (line[index] != ',') {
        index++;
    }
    
    return index;
}


/*
 * This function finds and gets the rank
 * of the name from the given line.
 *
 * @param *line - pointer to the line of characters
 * @param length - length of the line
 * @return - returns rank of the name
 */
int getRank(char *line, size_t length) {
    
    int comma = findIndexOfComma(line) + 3;
    int i;
    int size = (int)length - comma - 2;
    char num[size];
    
    for (i = 0; i < size; i++) {
        num[i] = line[i + comma];
    }
    
    int rank = atoi(num);
    
    return rank;
}


/*
 * This function finds the row index of the
 * given name in the 2d names array.
 *
 * @param *allNames - pointer to the 2d array of names
 * @param *theName - pointer to the name to be found
 * @param numberOfNames - total number of the names
 * @return - returns the row index of the name
 */
int findNamePosition(char (*allNames)[NAME_LENGTH], char *theName, int numberOfNames) {
    
    int result = 0;
    
    for (int i = 0; i < numberOfNames; i++) {
        if (strcmp(*(allNames + i), theName) == 0) {
            result = i;
        }
    }
    
    return result;
}


/*
 * This function sorts all names alphabeticaly and swaps
 * the ranks of the names.
 *
 * @param *c - pointer to the 2d array of names
 * @param *left - pointer to the 2d array of names
 * @param *right - pointer to the 2d array of names
 * @param length - the length of the name
 * @param *limit - pointer to the end of the names array
 * @param *allNames - pointer to the 2d array of names
 * @param *rank - pointer to the ranks array
 */
void quickStrSort(char *c, char *left, char *right,
                  int length, char *limit, int (*rank)[10], char (*allNames)[NAME_LENGTH]){

    int n1, n2;
    int row1[10];
    int row2[10];
    
    int (*p)[10] = rank;
    
    char *i = left;
    char *j = right;
    char *swap[NAME_LENGTH] = {'\0'};
    char test[length];
    char *s1 = i, *s2 = j;
    while (s1 < s2) {   // find midpoint for the test value
        s1 = s1 + length;
        s2 = s2 - length;
    }
    strncpy(test, s2, length - 1);

    do {
        while (strcmp(i, test) < 0 && i < limit) i = i + length;
        while (strcmp(j, test) > 0 && j >= c) j = j - length;
        
        // Swap the two values:
        if (i <= j){
            for (s1 = (char *)swap, s2 = i; *s2 != '\0'; s1++, s2++)
                *s1 = *s2;
            *s1 = *s2;  // Also copy the '\0'
            for (s1 = i, s2 = j; *s2 != '\0'; s1++, s2++)
                *s1 = *s2;
            *s1 = *s2;
            for (s1 = j, s2 = (char *)swap; *s2 != '\0'; s1++, s2++)
                *s1 = *s2;
            *s1 = *s2;
            
            n1 = findNamePosition(allNames, i, 366);
            n2 = findNamePosition(allNames, j, 366);
            
            memcpy(row1, (*(p) + n1 * 10), sizeof(int) * 10);
            memcpy(row2, (*(p) + n2 * 10), sizeof(int) * 10);
            memcpy((*(p) + n1 * 10), row2, sizeof(int) * 10);
            memcpy((*(p) + n2 * 10), row1, sizeof(int) * 10);

            
            i = i + length;
            j = j - length;
        }
    } while (i <= j);
    if (left < j) {
        quickStrSort(c, left, j, length, limit, rank, allNames);
    }
    if (i < right) {
        quickStrSort(c, i, right, length, limit, rank, allNames);
    }
}
