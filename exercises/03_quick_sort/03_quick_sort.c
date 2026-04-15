#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN     50

typedef struct {
    char name[NAME_LEN];
    int score;
} Student;

Student students[MAX_STUDENTS];

void quick_sort(int left, int right) {

    // edge cases, when the number in the array is one or below
    if (left >= right) {return ;}

    // choose a pivot
    int v = students[right].score;

    // reorder the array, make the element higher than the pivot on the left side of the pivot.
    int pivot = left,i = 0; // inital ptr

    for ( i = left; i <= right - 1; i++) {
        if (students[i].score > v) {
            Student t = students[pivot];
            students[pivot] = students[i];
            students[i] = t;
            pivot++;
        }
    }

    // now pivot must points to the value that is less than the pivot
    Student t = students[right];
    students[right] = students[pivot];
    students[pivot] = t;


    // recursively sort on the left hand side
    quick_sort(left, pivot - 1);
    // recursively sort on the right hand side
    quick_sort(pivot + 1, right);
}

int main(void) {
    FILE *file = fopen("03_students.txt", "r");
    if (!file) {
        printf("错误：无法打开文件 03_students.txt\n");
        return 1;
    }

    int n;
    fscanf(file, "%d", &n);

    if (n <= 0 || n > MAX_STUDENTS) {
        printf("学生人数无效：%d（应为 1-%d）\n", n, MAX_STUDENTS);
        fclose(file);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fscanf(file, "%s %d", students[i].name, &students[i].score);
    }
    fclose(file);

    quick_sort(0, n - 1);

    // 输出结果
    printf("\n快速排序后按成绩从高到低排序的学生名单：\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", students[i].name, students[i].score);
    }

    return 0;
}
