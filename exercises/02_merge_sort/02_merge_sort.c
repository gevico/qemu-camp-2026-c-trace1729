#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LEN 50

typedef struct {
    char name[NAME_LEN];
    int score;
} Student;

Student students[MAX_STUDENTS];
Student temp[MAX_STUDENTS];

void merge_sort(int left, int right) {
    // edge cases
    if (left >= right) {return;}

    int mid = (left+right) / 2;

    // recursively handle left hand side
    merge_sort(left, mid);

    // recursively handle right hand side
    merge_sort(mid+1, right);

    int i, j, k;
    // merge the two sorted list
    for (i = left, j = mid+1, k = 0; i <= mid && j <= right;) {
        if (students[i].score > students[j].score) {
            temp[k] = students[i];
            i++; k++;
        } else {
            temp[k] = students[j];
            j++; k++;
        }
    }

    // either one index exhuasted
    while (i <= mid) {
        temp[k] = students[i];
        k++; i++;
    }

    while (j <= right) {
        temp[k] = students[j];
        k++; j++;
    }
    // copy the sorted list back to original
    k = 0;
    for (i = left; i <= right; i ++, k++) {
        students[i] = temp[k];
    }
}

int main(void) {
    FILE *file = fopen("02_students.txt", "r");
    if (!file) {
        printf("错误：无法打开文件 02_students.txt\n");
        return 1;
    }

    int n;
    fscanf(file, "%d", &n);

    if (n <= 0 || n > MAX_STUDENTS) {
        printf("学生人数无效：%d\n", n);
        fclose(file);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fscanf(file, "%s %d", students[i].name, &students[i].score);
    }
    fclose(file);

    merge_sort(0, n - 1);

    printf("\n归并排序后按成绩从高到低排序的学生名单：\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", students[i].name, students[i].score);
    }

    return 0;
}
