#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    enum {
        ST_OLD,
        ST_NEW
    } state = ST_OLD;
    char old_buf[MAX_LINE_LENGTH] = {0};
    char new_buf[MAX_LINE_LENGTH] = {0};
    int old_len = 0;
    int new_len = 0;
    if (cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

    for (int i = 2; ; i++) {
        char ch = cmd[i];

        if (state == ST_OLD) {
            if (ch == '/') {
                old_buf[old_len] = '\0';
                state = ST_NEW;
            } else if (ch == '\0') {
                return -1;
            } else if (old_len < MAX_LINE_LENGTH - 1) {
                old_buf[old_len++] = ch;
            }
        } else {
            if (ch == '/') {
                new_buf[new_len] = '\0';
                *old_str = malloc((size_t)old_len + 1);
                *new_str = malloc((size_t)new_len + 1);
                if (*old_str == NULL || *new_str == NULL) {
                    free(*old_str);
                    free(*new_str);
                    *old_str = NULL;
                    *new_str = NULL;
                    return -1;
                }
                strcpy(*old_str, old_buf);
                strcpy(*new_str, new_buf);
                return 0;
            } else if (ch == '\0') {
                return -1;
            } else if (new_len < MAX_LINE_LENGTH - 1) {
                new_buf[new_len++] = ch;
            }
        }
    }

    return 0;
}

// could use strstr search string, much concise
void replace_first_occurrence(char* str, const char* old, const char* new) {
    enum {
        ST_NORMAL,
        ST_MATCH
    } state = ST_NORMAL;
    char *start = NULL;
    char *end = NULL;
    size_t match_index = 0;
    char *p = str;

    if (old[0] == '\0') {
        return;
    }

    while (*p != '\0' && end == NULL) {
        if (state == ST_NORMAL) {
            if (*p == old[0]) {
                start = p;
                match_index = 1;
                if (old[match_index] == '\0') {
                    end = p + 1;
                    break;
                }
                state = ST_MATCH;
            }
        } else {
            if (*p == old[match_index]) {
                match_index++;
                if (old[match_index] == '\0') {
                    end = p + 1;
                    break;
                }
            } else {
                state = ST_NORMAL;
                match_index = 0;
                p = start;
            }
        }
        p++;
    }

    if (start == NULL || end == NULL) {
        return;
    }

    {
        char tmp[MAX_LINE_LENGTH];
        size_t prefix_len = (size_t)(start - str);

        snprintf(tmp, sizeof(tmp), "%.*s%s%s",
                 (int)prefix_len, str, new, end);
        strncpy(str, tmp, MAX_LINE_LENGTH - 1);
        str[MAX_LINE_LENGTH - 1] = '\0';
    }
}

int main(int argc, char* argv[]) {
    const char* replcae_rules = "s/unix/linux/";

    char line[MAX_LINE_LENGTH] = {"unix is opensource. unix is free os."};

    char* old_str = NULL;
    char* new_str = NULL;

    if (parse_replace_command(replcae_rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }


    replace_first_occurrence(line, old_str, new_str);
    fputs(line, stdout);

    free(old_str);
    free(new_str);
    return 0;
}
