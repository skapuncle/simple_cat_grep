#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

void printFile(FILE *fptr);
int checkFlags(char **argv, int i2, int i3, int i4);
void optionNandB(FILE *fptr, char **argv);
void optionEandT(FILE *fptr, char **argv);
void optionS(FILE *fptr);
void optionV(FILE *fptr);

#endif  //  SRC_CAT_S21_CAT_H_
