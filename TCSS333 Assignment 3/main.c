//
//  main.c
//  TCSS333 Assignment 3
//
//  Created by aterikov on 10/22/15.
//  Copyright © 2015 Alex Terikov. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#define NAME_SIZE 20;
#define TOTAL_NAMES 100;

int findIndexOfComma(char *);
char parseLine(char *, int);
int getRank(char *line, size_t length);

int main(void) {
    
    FILE *txtFile1 = fopen("names/yob1920.txt", "r");
//    FILE *txtFile1 = fopen("sample.txt", "r");
    
    char *line = NULL;
    size_t length = 0;
    size_t read;
    
    int i, j;
    int currentFile = 0;
    int numOfFiles;
    
    char names[100][20]; // 2d array of names
    int ranks[100][10]; // 2d array of ranks
    
    if (txtFile1 == NULL) {
        perror("Failed to open file");
    }
    
    
        
    for (i = 0; i < 100; i++) {
        read = getline(&line, &length, txtFile1);
//        printf("Retrieved line of length %zu :\n", read);
//        printf("%s", line);
//        printf("%c\n", parseLine(line, findIndexOfComma(line)));
        
        for (j = 0; j < findIndexOfComma(line); j++) {
            names[i][j] = line[j];
        }
        
        ranks[i][currentFile] = getRank(line, length);
        
//        printf("new num is - %d\n", getRank(line, length));
        
    }
    
    printf("%d\n", ranks[99][0]);
//    printf("%c\n", names[99][0]);
    
    fclose(txtFile1);
    
    if (line) {
        free(line);
    }
    
    return 0;
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
