#include "s21_cat.h"

int main(int argc, char **argv) {
    int i2 = 2, i3 = 1, i4 = 0;
    for (int i = argc; i != i4; i--) {
        i4 = checkFlags(argv, i2, i3, i4);
        i2++;
        i3++;
    }
    return 0;
}

void printFile(FILE *fptr) {
    char b;
    while ((b = fgetc(fptr)) != EOF) {
        putchar(b);
    }
}


int checkFlags(char **argv, int i2, int i3, int i4) {
    FILE *fptr;
    if (argv[1][0] == '-') {
        i4 = 2;
        if (argv[1][1] == 'n' || argv[1][1] == 'b' || !strcmp(argv[1], "--number-nonblank")
         || !strcmp(argv[1], "--number")) {
            fptr = fopen(argv[i2], "r+");
            optionNandB(fptr, argv);
        } else if (argv[1][1] == 'e' || argv[1][1] == 't' || argv[1][1] == 'E' || argv[1][1] == 'T') {
            fptr = fopen(argv[i2], "r+");
            optionEandT(fptr, argv);
        } else if (argv[1][1] == 's' || !strcmp(argv[1], "--squeeze-blank")) {
            fptr = fopen(argv[i2], "r+");
            optionS(fptr);
        } else if (argv[1][1] == 'v') {
            fptr = fopen(argv[i2], "r+");
            optionV(fptr);
        } else {
            fprintf(stderr, "cat: illegal option -- %c\n", argv[1][1]);
            fprintf(stderr, "usage: cat [-benstuv] [file ...]\n");
        }
    } else {
        i4 = 1;
        fptr = fopen(argv[i3], "r+");
        printFile(fptr);
    }
    fclose(fptr);
    return i4;
}


void optionNandB(FILE *fptr, char **argv) {
    char ch;
    if (argv[1][1] == 'n' || !strcmp(argv[1], "--number")) {
        char line[1024] = {0};
        int count = 1, index = 0;
        while ((line[index] = fgetc(fptr)) != EOF) {
            if (line[index] == '\n') {
                line[index] = '\0';
                printf("%6d\t", count++);
                printf("%s\n", line);
                index = 0;
                continue;
            }
            index++;
        }
        line[index] = '\0';
        if (index) {
            printf("%6d\t", count++);
        } else {
            NULL;
        }
        printf("%s", line);
    } else {
        char line[1024] = {0};
        int count = 1, index = 0;
        while ((line[index] = fgetc(fptr)) != EOF) {
            if (line[index] == '\n') {
                line[index] = '\0';
                if (index) {
                    printf("%6d\t", count++);
                } else {
                    NULL;
                }
                printf("%s\n", line);
                index = 0;
                continue;
            }
            index++;
        }
        line[index] = '\0';
        if (index) {
            printf("%6d\t", count++);
        } else {
            NULL;
        }
        printf("%s", line);
}
}


void optionEandT(FILE *fptr, char **argv) {
    int ch;
    if (argv[1][1] == 'e' || argv[1][1] == 'E') {
        while (true) {
            ch = fgetc(fptr);
            if (ch == EOF) break;
            if (ch == '\n') {
                printf("$");
                putchar(ch);
            } else if (ch >= 0x00 && ch < 128) {
                if ((ch >= 0x00 && ch < 0x09) || (ch > 0x0A && ch <= 0x1F)) {
                    printf("^");
                    ch += 64;
                }
                if (ch == 0x7F) {
                    printf("^");
                    ch = '?';
                }
                if (ch >= 0x80 && ch <= 0x9F) {
                    printf("M-^");
                    ch -= 64;
                }
                if (ch >= 0xA0 && ch <= 0xFF) {
                    printf("M-");
                    ch -= 128;
                }
                    printf("%c", ch);
            } else {
                putchar(ch);
            }
            }
        } else {
        while ((ch = fgetc(fptr)) != EOF) {
            if (ch == '\t') {
                printf("^I");
            } else if (ch >= 0x00 && ch < 128) {
                if ((ch >= 0x00 && ch < 0x09) || (ch > 0x0A && ch <= 0x1F)) {
                    printf("^");
                    ch += 64;
                }
                if (ch == 127) {
                    printf("^");
                    ch = '?';
                }
                if (ch >= 0x80 && ch <= 0x9F) {
                    printf("M-^");
                    ch -= 64;
                }
                if (ch >= 0xA0 && ch <= 0xFF) {
                    printf("M-");
                    ch -= 128;
                }
                    printf("%c", ch);
            } else {
                putchar(ch);
            }
        }
    }
}

void optionS(FILE *fptr) {
    int emp_str_cnt = 0;
    int next_symbol = '\n';
    while (true) {
        int current_symbol = fgetc(fptr);

        if (current_symbol == EOF) break;

        if (current_symbol == '\n' && next_symbol == '\n') {
            emp_str_cnt++;
            if (emp_str_cnt > 1) continue;
        } else {
            emp_str_cnt = 0;
        }
        printf("%c", current_symbol);
        next_symbol = current_symbol;
    }
}

void optionV(FILE *fptr) {
    while (true) {
        int current_symbol = fgetc(fptr);
        if (current_symbol == EOF) break;
        if ((current_symbol >= 0x00 && current_symbol < 0x09) ||
         (current_symbol > 0x0A && current_symbol <= 0x1F)) {
            printf("^");
            current_symbol += 64;
        }
        if (current_symbol == 0x7F) {
            printf("^");
            current_symbol = '?';
        }
        if (current_symbol >= 0x80 && current_symbol <= 0x9F) {
            printf("M-^");
            current_symbol -= 64;
        }
        if (current_symbol >= 0xA0 && current_symbol <= 0xFF) {
            printf("M-");
            current_symbol -= 128;
        }
        printf("%c", current_symbol);
    }
}
