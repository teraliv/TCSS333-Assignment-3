//
//  main.c
//  TCSS333 Assignment 3
//
//  Created by aterikov on 10/22/15.
//  Copyright © 2015 Alex Terikov. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NAME_SIZE 20;
#define TOTAL_NAMES 100;

#define NAME_LENGTH 30


int findNamePosition(char (*allNames)[30], char *theName, int numberOfNames);
int findName(char (*allNames)[30], char *theName, int numberOfNames);
//void print2dArray(char *allNames, char *theName);
int compareNames(char *first, char *second);
//void compareNames(char name1[][20], char name2[], int num);
int nameLength(char *);
int findIndexOfComma(char *);
char parseLine(char *, int);
int getRank(char *line, size_t length);
void quickStrSort(char *c, char *left, char *right,
                  int length, char *limit, int (*rank)[3], char (*allNames)[30]);



int main(void) {
    
    FILE *txtFile1 = fopen("names/yob1920.txt", "r");
    FILE *inputFile[] = {
        inputFile[0] = fopen("names/yob1920.txt", "r"),
        inputFile[1] = fopen("names/yob1930.txt", "r"),
        inputFile[2] = fopen("names/yob1940.txt", "r"),
        inputFile[3] = fopen("names/yob1950.txt", "r"),
        inputFile[4] = fopen("names/yob1960.txt", "r"),
        inputFile[5] = fopen("names/yob1970.txt", "r"),
        inputFile[6] = fopen("names/yob1980.txt", "r"),
        inputFile[7] = fopen("names/yob1990.txt", "r"),
        inputFile[8] = fopen("names/yob2000.txt", "r"),
        inputFile[9] = fopen("names/yob2010.txt", "r"),
    };
    
    char *line = NULL;
    size_t length = 0;
//    size_t read;
    
    int i, j;
    int currentFile = 0;
    int totalNames = 0;
//    int numOfFiles = 10;
//    int counter = 0;
    
//    bool nameNotFound = false;
    
//    char names[150][20]; // 2d array of names
    char names[400][NAME_LENGTH] = {'\0'}; // 2d array of names
    int ranks[400][10] = {'\0'}; // 2d array of ranks
    
//    char (*pointer)[30] = names;
    char *pCurrentName;
    
    
    int nameCount = 0;
    char *nam;
    
    char x[] = "Mary";
    
//    char patr[] = "Patricia";
//    char *p1 = patr;
    
//    char (*pointer)[200][30];
    
//    for (i = 0; i < 400; i++) {
//        for (j = 0; j < 10; j++) {
//            ranks[i][j] = 0;
//        }
//    }
//    
//    for (i = 0; i < 400; i++) {
//        for (j = 0; j < 30; j++) {
//            names[i][j] = '\0';
//        }
//    }
    
    bool firstFile = true;
    
//    for (i = 1; i <= 10; i++) {
//        inputFile[i] = fopen("names/yob1920.txt", "r");
//    }
    
    if (txtFile1 == NULL) {
        perror("Failed to open file");
    }
    
    for (i = 0; i < 10; i++) {
        if (inputFile[i] == NULL) {
            perror("Failed to open file");
        }
    }
    
    while (currentFile < 10) {
        
        for (i = 0; i < 300; i++) {
            getline(&line, &length, inputFile[currentFile]);
            int indexComma = findIndexOfComma(line);
            
            // 100 names from first file
            // Check if the current name row is empty
            // If it is empty write the current name from the file
            if (i < 100 && firstFile == true) {
                if (names[i][0] == 0) {
//                    int indexComma = findIndexOfComma(line);
                    
                    for (j = 0; j < findIndexOfComma(line); j++) {
                        names[i][j] = line[j];
                    }
                    names[i][indexComma] = '\0';
                }
                totalNames++;
            }
            
            
            // 100 ranks from first file
            if (i < 100 && firstFile == true) {
                for (j = 0; j < 1; j++) {
                    ranks[i][j] = getRank(line, length);
                }

            }
            
            
            if (!firstFile && i < 100) {
                
                char currentName[indexComma + 1];
                
                // make currentName to compare with names array
                for (int k = 0; k < indexComma; k++) {
                    currentName[k] = line[k];
                }
                
                currentName[indexComma] = '\0'; // set null character
                pCurrentName = currentName;
                
                for (int row = 0; row < totalNames; row++) {
                    if (strcmp(names[row], currentName) == 0) {
//                        printf("%d - %s - %d - %d\n", row, currentName, currentFile, getRank(line, length));
                        ranks[row][currentFile] = getRank(line, length);
                    
                    }
                    
                }
                
//                findName(names, pCurrentName);
                
                if (findName(names, pCurrentName, totalNames) == 1) {
                    
//                    printf("%s\n", currentName);
//                    printf("%d\n", totalNames);
//                    printf("%s\n", line);
                    
                    for (int f = 0; f < indexComma; f++) {
                        names[totalNames][f] = currentName[f];
                    }
                    
                    ranks[totalNames][currentFile] = getRank(line, length);
                    
                    totalNames++;

//                    while (*pCurrentName) {
                    
//                        names[totalNames][index] = currentName[index];
//                        index++;
//                        *names[totalNames] = *pCurrentName;
//                        pCurrentName++;
//                        names[totalNames]++;
//                    }
                    
                }
                
            }
        }
        
        
//        print2dArray(pointer);
        
        currentFile++;
        firstFile = false;
        
    }
    
    printf("Total num of names is %d\n", totalNames);
//    printf("Desired name index: %d\n", findNamePosition(names, x, totalNames));
    
    
    // quick sort results
    nameCount = totalNames;
    
//    for (i = 0, nam = (char *)names; i <= nameCount; i++, nam += NAME_LENGTH) {
//        printf("%3d %-13s%lu\n", i, nam, strlen(nam));
//    }
    
    quickStrSort((char *)names, (char *)names, (char *)names + NAME_LENGTH * (nameCount - 1),
                 NAME_LENGTH, (char *)names + NAME_LENGTH * nameCount, (int *)ranks, names);
    
    // traverse and print the sorted names
    for (i = 0, nam = (char *)names; i < 366; i++, nam += NAME_LENGTH) {
        printf("%2d %-13s", i, nam);
        
        for (j = 0; j < 10; j++) {
            printf("%8d", ranks[i][j]);
        }
        printf("\n");
    }
    
    
    
//    print2dArray(pointer, p1);
//    print2dArray(names, p1);
//    printf("%d\n", compareNames(names[0], p1));
    //print ranks
//    for (int i = 0; i < 100; i++) {
//        for (int j = 0; j < 3; j++) {
//            printf("%d - %d  ", i, ranks[i][j]);
//        }
//        printf("\n");
//    }
    
    
    // print names
//    for (i = 0; i <= 365; i++) {
//        printf("%d - %s\n", i, names[i]);
//    }
    
    // print all names and ranks
//    for (i = 0; i <= 365; i++) {
//        printf("%d - ", i);
//        for (j = 0; j < 15; j++) {
//            printf("%c", names[i][j]);
//        }
//        
//        for (j = 0; j < 10; j++) {
//            printf(" | %d | ", ranks[i][j]);
//        }
//        printf("\n");
//    }
    
//    printf("%s\n", names[365]);
//    printf("%d\n", ranks[365][9]);
    
    
    // print ranks
//    for (i = 0; i < 365; i++) {
//        
//        for (j = 0; j < 10; j++) {
//            printf(" %d ", ranks[i][j]);
//        }
//        
//        printf("\n");
//    }
    
//    printf("%d\n", ranks[0][0]);
    
    
    
    fclose(txtFile1);
    
    for (i = 0; i < 10; i++) {
        fclose(inputFile[i]);
    }
    
    if (line) {
        free(line);
    }
    
    return 0;
}

// finds the name that not in the names array
int findName(char (*allNames)[30], char *theName, int numberOfNames) {
//void print2dArray(char *allNames, char *theName) {

    bool found = true;
    int result = 0;
    
    for (int i = 0; i < numberOfNames; i++) {
        if (compareNames(*(allNames + i), theName) == 0) {
//            printf("%d\n", compareNames(*(allNames + i), theName));
//            printf("orig - %s\n", allNames[i]);
//            printf("new  - %s\n", theName);
//            printf("%d\n", compareNames(*(allNames + i), theName));
            found = false;
            result = 0;
        }
    
    }
    
//    printf("orig - %s\n", allNames);
//    printf("new  - %s\n", theName);
//    printf("%d\n", compareNames(allNames, theName));

//    if (compareNames(allNames, theName) != 0) {
//        notFound = true;
//    }
    
    if (found) {
//        printf("%s\n", theName);
        found = false;
        result = 1;
    }
    
    return result;
}



int compareNames(char *first, char *second) {
    
    int result;
    
    while (*first == *second && *first != '\0' && * second != '\0') {
        first++;
        second++;
    }
    
    if ((*first - *second) == 0) {
        result = 0;
//        printf("names are equal\n");
    } else {
        result = 1;
    }
    
    return result;
}

int nameLength(char *name) {
    int length = 0;
    
    while (name[length] != '\0') {
        length++;
    }
    
    return length;
}

int findIndexOfComma(char *line) {
    int commaIndex = 0;
    
    while (line[commaIndex] != ',') {
        commaIndex++;
    }
    
    return commaIndex;
}



char parseLine(char *line, int indexOfComma) {
    
    int i;
    char name[indexOfComma];
    
    for (i = 0; i < indexOfComma; i++) {
        name[i] = line[i];
    }
    
    return *name;
}


int getRank(char *line, size_t length) {
    
    int comma = findIndexOfComma(line) + 3;
    int i;
    int size = (int)length - comma - 2;
    char num[size];
    
//    printf("size - %d\n", comma);
//    printf("start - %d\n", findIndexOfComma(line)+3);
//    printf("value - %c\n", line[11]);
//    printf("length - %zu\n", length);
    
    for (i = 0; i < size; i++) {
        num[i] = line[i + comma];
//        printf("num[i] - %c\n", num[i]);
    }
    
    int newNum = atoi(num);
    
//    printf("%s\n", num);
//    printf("new num is %d\n", newNum);
    
    return newNum;
}


// finds the row position of a name
int findNamePosition(char (*allNames)[30], char *theName, int numberOfNames) {
    
    int result = 0;
    
    for (int i = 0; i < numberOfNames; i++) {
        if (strcmp(*(allNames + i), theName) == 0) {
            result = i;
        }
    }
    
    return result;
}


// QUICK SORT
void quickStrSort(char *c, char *left, char *right,
                  int length, char *limit, int (*rank)[3], char (*allNames)[30]){
//    int k;
    int n1, n2;
    int row1[10];
    int row2[10];
    
    int (*p)[3] = rank;
    
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
//    printf("%s\n", s2);
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
//            printf("index: %d - %s\n", n1, i);
            
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
