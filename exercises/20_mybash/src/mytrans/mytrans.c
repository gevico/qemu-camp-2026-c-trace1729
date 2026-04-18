// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void trim(char *str) {
  size_t start = 0;
  size_t end;

  while (str[start] && isspace((unsigned char)str[start])) {
    start++;
  }

  if (start > 0) {
    memmove(str, str + start, strlen(str + start) + 1);
  }

  end = strlen(str);
  while (end > 0 && isspace((unsigned char)str[end - 1])) {
    str[--end] = '\0';
  }
}

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("无法打开词典文件");
    return -1;
  }

  char line[1024];
  char current_word[100] = {0};
  char current_translation[1024] = {0};
  int in_entry = 0;
  while (fgets(line, sizeof(line), file) != NULL) {
    trim(line);
    if (line[0] == '\0') {
      continue;
    }

    if (line[0] == '#') {
      if (in_entry && current_word[0] != '\0' && current_translation[0] != '\0') {
        if (hash_table_insert(table, current_word, current_translation)) {
          (*dict_count)++;
        }
      }
      strncpy(current_word, line + 1, sizeof(current_word) - 1);
      current_word[sizeof(current_word) - 1] = '\0';
      current_translation[0] = '\0';
      in_entry = 1;
    } else if (strncmp(line, "Trans:", 6) == 0 && in_entry) {
      strncpy(current_translation, line + 6, sizeof(current_translation) - 1);
      current_translation[sizeof(current_translation) - 1] = '\0';
      trim(current_translation);
    }
  }

  if (in_entry && current_word[0] != '\0' && current_translation[0] != '\0') {
    if (hash_table_insert(table, current_word, current_translation)) {
      (*dict_count)++;
    }
  }

  fclose(file);
  return 0;
}

void to_lowercase(char *str) {
  for (; *str; ++str)
    *str = tolower((unsigned char)*str);
}

int __cmd_mytrans(const char* filename) {
  const char *dict_candidates[] = {
      "/workspace/exercises/20_mybash/src/mytrans/dict.txt",
      "../exercises/20_mybash/src/mytrans/dict.txt",
      "exercises/20_mybash/src/mytrans/dict.txt",
      "src/mytrans/dict.txt",
      NULL,
  };
  HashTable *table = create_hash_table();
  if (!table) {
    fprintf(stderr, "无法创建哈希表\n");
    return 1;
  }

  printf("=== 哈希表版英语翻译器（支持百万级数据）===\n");
  uint64_t dict_count = 0;
  const char *dict_path = NULL;
  for (int i = 0; dict_candidates[i] != NULL; i++) {
    if (access(dict_candidates[i], R_OK) == 0) {
      dict_path = dict_candidates[i];
      break;
    }
  }

  if (dict_path == NULL ||
      load_dictionary(dict_path, table, &dict_count) != 0) {
    fprintf(stderr, "加载词典失败，请确保 dict.txt 存在。\n");
    free_hash_table(table);
    return 1;
  }
  printf("词典加载完成，共计%ld词条。\n", dict_count);

  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "无法打开文件 dict.txt。\n");
    free_hash_table(table);
    return 1;
  }

  char line[256];
  while (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\n")] = '\0';

    if (strlen(line) == 0) {
      continue;
    }

    char word[128];
    int word_pos = 0;
    for (size_t i = 0; ; i++) {
      char ch = line[i];
      if (isalpha((unsigned char)ch) || ch == '\'') {
        if (word_pos < (int)sizeof(word) - 1) {
          word[word_pos++] = (char)tolower((unsigned char)ch);
        }
      } else {
        if (word_pos > 0) {
          word[word_pos] = '\0';
          {
            const char *translation = hash_table_lookup(table, word);
            if (translation) {
              printf("原文: %s\t翻译: %s\n", word, translation);
            } else {
              printf("原文: %s\t未找到该单词的翻译。\n", word);
            }
          }
          word_pos = 0;
        }

        if (ch == '\0') {
          break;
        }
      }
    }
  }

  fclose(file);
  free_hash_table(table);
  return 0;
}
