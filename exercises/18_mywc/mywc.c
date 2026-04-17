#include "mywc.h"

// 创建哈希表
WordCount **create_hash_table() {
  WordCount **hash_table = calloc(HASH_SIZE, sizeof(WordCount *));
  return hash_table;
}

// 简单的哈希函数
unsigned int hash(const char *word) {
  unsigned long hash = 5381;
  int c;
  while ((c = *word++))
    hash = ((hash << 5) + hash) + c; // hash * 33 + c
  return hash % HASH_SIZE;
}

// 检查字符是否构成单词的一部分
bool is_valid_word_char(char c) { return isalpha(c) || c == '\''; }

// 转换为小写
char to_lower(char c) { return tolower(c); }

// 添加单词到哈希表
void add_word(WordCount **hash_table, const char *word) {
    int hash_value = hash(word);
    WordCount* list = hash_table[hash_value];
    while (list != NULL) {
        if (strcmp(list->word, word) == 0) {
            list->count++;
            break;
        }
        list = list->next;
    }
    if (!list) { // not found
        WordCount* new_word = calloc(1, sizeof(WordCount));
        list = hash_table[hash_value];
        hash_table[hash_value] = new_word;
        new_word->next = list;
        new_word->count = 1;
        strcpy(new_word->word, word);
    }
}

// 打印单词统计结果
void print_word_counts(WordCount **hash_table) {
  printf("Word Count Statistics:\n");
  printf("======================\n");

    for (int i = 0; i < HASH_SIZE; i++)  {
        WordCount* list = hash_table[i];
        while (list != NULL) {
            WordCount* next = list->next;
            printf("%-21s%d\n", list->word, list->count);
            list = next;
        }
    }
}

// 释放哈希表内存
void free_hash_table(WordCount **hash_table) {
    for (int i = 0; i < HASH_SIZE; i++)  {
        WordCount* list = hash_table[i];
        while (list != NULL) {
            WordCount* next = list->next;
            free(list);
            list = next;
        }
    }
    // forgot to free
    free(hash_table);
}


// 处理文件并统计单词
void process_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  WordCount **hash_table = create_hash_table();
  char word[MAX_WORD_LEN];
  int word_pos = 0;
  int c;

  while ((c = fgetc(file)) != EOF) {
    if (is_valid_word_char(c)) {
      if (word_pos < MAX_WORD_LEN - 1) {
        word[word_pos++] = to_lower(c);
      }
    } else {
      if (word_pos > 0) {
        word[word_pos] = '\0';
        add_word(hash_table, word);
        word_pos = 0;
      }
    }
  }

  // 处理文件末尾的最后一个单词
  if (word_pos > 0) {
    word[word_pos] = '\0';
    add_word(hash_table, word);
  }

  fclose(file);
  print_word_counts(hash_table);
  free_hash_table(hash_table);
}
