#include "s21_grep.h"

int main(int argc, char *argv[]) {
    struct flags flag = {0};
    char *line = 0;
    char f_buff[BUFSIZ] = {0}, e_buff[BUFSIZ] = {0},
        reg_buff[BUFSIZ] = {0}, f_var[] = "0";
    int c = 0, f_count = 0, e_count = 0, c_count = 0, l_count = 0,
        n_count = 0, ind_opt = 0, regflags = REG_EXTENDED, line_check = 0,
        match_check = REG_NOMATCH;
    size_t n_getline = 0;
    regex_t regular;
    regmatch_t regmat[1];
    while ((c = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, &ind_opt)) != -1) {
        switch (c) {
            case 'e':
                if (e_count > 0) {
                    strcat(e_buff, "|");
                }
                e_count++;
                strcat(e_buff, optarg);
                flag.e = 1;
                break;
            case 'i':
                flag.i = 1;
                regflags |= REG_ICASE;
                break;
            case 'v':
                flag.v = 1;
                break;
            case 'c':
                flag.c = 1;
                break;
            case 'l':
                flag.l = 1;
                break;
            case 'n':
                flag.n = 1;
                break;
            case 'h':
                flag.h = 1;
                break;
            case 's':
                flag.s = 1;
                break;
            case 'f':
                if (f_count > 0) {
                    strcat(f_buff, "|");
                }
                FILE *f_file = fopen(optarg, "r");
                while ((f_var[0] = getc(f_file)) != EOF) {
                    if (f_var[0] == '\n') {
                        strcat(f_buff, "|");
                    }
                    strcat(f_buff, f_var);
                }
                f_count++;
                fclose(f_file);
                flag.f = 1;
                break;
            case 'o':
                flag.o = 1;
                break;
            default:
                break;;
        }
    }
    if (flag.s) {
        int file_check = ++optind;
        FILE *s_file = fopen(argv[file_check], "r");
        while (argc >= file_check) {
            if (s_file == NULL) {
                exit(0);
            } else {
                fclose(s_file);
            }
        }
    }
    strcat(reg_buff, argv[optind]);
    if (flag.f || flag.e) {
        memset(reg_buff, 0, BUFSIZ);
    }
    if (!flag.f && !flag.e) {
        optind++;
    }
    if (flag.f) {
        strcat(reg_buff, f_buff);
    }
    if (flag.e) {
        if (flag.f) {
            strcat(reg_buff, "|");
            strcat(reg_buff, e_buff);
        } else {
            strcat(reg_buff, e_buff);
        }
    }

    regcomp(&regular, reg_buff, regflags);
    int name_print = argc - optind;
    while (argc > optind) {
        FILE *main_file = fopen(argv[optind], "r");
        while ((line_check = (getline(&line, &n_getline, main_file)) != -1)) {
            n_count++;
            match_check = regexec(&regular, line, 1, regmat, 0);
            if (match_check != REG_NOMATCH) l_count = 1;
            if (match_check != REG_NOMATCH && !flag.v && !flag.l) {
                if (flag.c) {
                    c_count++;
                    continue;
                }
                if (name_print > 1 && !flag.h) printf("%s:", argv[optind]);
                if (flag.n) printf("%d:", n_count);
                if (strchr(line, '\n') == NULL) strcat(line, "\n");
                if (flag.o) {
                    size_t len_buff = regmat->rm_eo - regmat->rm_so;
                    while (len_buff) {
                        printf("%c", line[regmat->rm_so++]);
                        len_buff--;
                    }
                    printf("\n");
                    continue;;
                }
                printf("%s", line);
            }
            if (match_check == REG_NOMATCH && flag.v && !flag.l) {
                if (flag.c) {
                    c_count++;
                    continue;
                }
                if (name_print > 1 && !flag.h) printf("%s:", argv[optind]);
                if (flag.n) printf("%d:", n_count);
                if (strchr(line, '\n') == NULL) strcat(line, "\n");
                printf("%s", line);
            }
        }
        if (flag.c) {
            if (name_print > 1 && !flag.h) printf("%s:", argv[optind]);
            if (flag.l) {
                c_count > 0 ? printf("%d\n", l_count) : printf("%d\n", l_count);
            } else {
                printf("%d\n", c_count);
            }
            c_count = 0;
        }
        if (flag.l) {
            if (l_count) printf("%s\n", argv[optind]);
            l_count = 0;
        }
        fclose(main_file);
        optind++;
        n_count = 0;
    }
    free(line);
    regfree(&regular);
    return 0;
}
