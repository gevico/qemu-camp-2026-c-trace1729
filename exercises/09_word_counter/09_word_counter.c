#include <stdio.h>

#define IN_WORD 1
#define OUT_WORD 2

int main() {
    char str[]="Don't ask what your country can do for you, but ask what you can do for your country.";
    int wordCount = 0;

    // construct state machines.
    // Two, one in inword, the other is out word. when transition from outword to inword, counter ++;
    char* s = str;

    int state = OUT_WORD;
    while(*s) {
        switch(state) {
            case OUT_WORD:
                if (*s != ' ') {
                    wordCount++;
                    state = IN_WORD;
                }
            break;
            case IN_WORD:
                if (*s == ' ') {
                    state = OUT_WORD;
                }
            break;
        }
        s++;
    }

    printf("单词数量: %d\n", wordCount);

    return 0;
}
