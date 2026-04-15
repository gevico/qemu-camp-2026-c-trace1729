#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

int visited[MAX_ROW][MAX_COL];
int parent[MAX_ROW][MAX_COL];

int stack[2* MAX_ROW* MAX_COL];
int top;

// top points to the next element into the stack
void push(int v) {
    stack[top++] = v;
}

// Do not violated top pointer invarints
int pop() {
    return stack[--top];
}

/*
 * Start from the left most node, mark as vistied.
 *  foreach c in node.children(): (exclude wall)
 *      if unvisited:
 *          dfs(c)
 * */
int main(void)
{
    push(0);

    int v = 0; // start from left most node
    while (top > 0 && v != 24) {
        v = pop();
        int i = v / 5, j = v % 5;
        // do not violates the wall invarints, visited invarints
        if (visited[i][j] == 0){ // if unvisited
            visited[i][j] = 1;
            if (i + 1 < 5 && visited[i+1][j]==0 && maze[i+1][j] == 0) {
                push((i+1)*5 + j);
                parent[i+1][j] = v;
            }
            if (j + 1 < 5 && visited[i][j+1] == 0 && maze[i][j+1] == 0) {
                push(i*5+j+1);
                parent[i][j+1] = v;
            }
            if (j - 1 >= 0 && visited[i][j-1] == 0 && maze[i][j-1] == 0) {
                push(i*5+j-1);
                parent[i][j-1] = v;
            }
            if (i - 1 >= 0 && visited[i-1][j] == 0 && maze[i-1][j] == 0) {
                push((i-1)*5+j);
                parent[i-1][j] = v;
            }
        }
    }

    v = 4*5+4;
    while (v != 0) {
        int i = v / 5, j = v % 5;
        printf("(%d, %d)\n", i, j);
        v = parent[i][j];
    }
    printf("(0, 0)\n");

	return 0;
}

