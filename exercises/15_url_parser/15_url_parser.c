#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * URL参数解析器
 * 输入：包含http/https超链接的字符串
 * 输出：解析出所有的key-value键值对，每行显示一个
 */

int parse_url(const char* url) {
    int err = 0;
    enum {
        ST_NORMAL,
        ST_KEY,
        ST_VALUE
    } state = ST_NORMAL;
    char key[128] = {0};
    char value[128] = {0};
    int key_len = 0;
    int value_len = 0;

    for (size_t i = 0; ; i++) {
        char ch = url[i];

        switch (state) {
        case ST_NORMAL:
            if (ch == '?') {
                state = ST_KEY;
                key_len = 0;
                value_len = 0;
            } else if (ch == '\0') {
                goto exit;
            }
            break;
        case ST_KEY:
            if (ch == '=') {
                key[key_len] = '\0';
                value_len = 0;
                state = ST_VALUE;
            } else if (ch == '\0') {
                goto exit;
            } else if (key_len < (int)sizeof(key) - 1) {
                key[key_len++] = ch;
            }
            break;
        case ST_VALUE:
            if (ch == '&' || ch == '\0') {
                value[value_len] = '\0';
                printf("key = %s, value = %s\n", key, value);
                key_len = 0;
                value_len = 0;
                if (ch == '&') {
                    state = ST_KEY;
                } else {
                    goto exit;
                }
            } else if (value_len < (int)sizeof(value) - 1) {
                value[value_len++] = ch;
            }
            break;
        }
    }

exit:
    return err;
}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}
