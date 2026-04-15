#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[20];
    int score;
} Student;

/* 选择排序
 * 假设存在一个插入函数，这个函数的作用就是将一个数保持顺序的插入有序数组
 * 那么，给定一个长度为k的有序数组，在经过插入函数之后，会变为一个长度为 k + 1 的有序数组
 *
 * 运用到数组中，给定一个随机长度为n 的数组，他的 array[0] 一定有序，（因为只有一个数），那么将其分为 sort_array [array[0]], unsort_array: [array[1..n]], 对于 unsort array 的每个数，都调用一次插入函数，即可构建一个有序数组
 * */

void insertion_sort(Student students[], int n) {

    for (int i = 1; i < n; i++) { // foreah num in unsort_arry[1..n]
        Student key = students[i];
        int j = i;
        while (j >= 1 && key.score > students[j-1].score) {
            students[j] = students[j-1];
            j--;
        }
        students[j] = key;
    }
}

int main(void) {
    FILE *file;
    Student students[50];
    int n = 0;

    // 打开文件（从命令行参数获取文件名）
    file = fopen("01_students.txt", "r");
    if (!file) {
        printf("错误：无法打开文件 01_students.txt\n");
        return 1;
    }

    // 从文件读取学生信息
    while (n < 50 && fscanf(file, "%s %d", students[n].name, &students[n].score) == 2) {
        n++;
    }
    fclose(file);

    if (n == 0) {
        printf("文件中没有学生信息\n");
        return 1;
    }

    insertion_sort(students, n);

    printf("\n按成绩从高到低排序后的学生信息:\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", students[i].name, students[i].score);
    }

    return 0;
}

