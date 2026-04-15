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

int queue[2* MAX_ROW* MAX_COL];
int front; // points to the first element
int tail; // pointes to next enqueue placee

// top points to the next element into the stack
void enqueue(int v) {
    queue[tail++] = v;
}

// Do not violated top pointer invarints
int dequeue() {
    return queue[front++];
}

int empty() {return front == tail;}

/*
 * difference between bfs and dfs depends on when the node is operated, in thic  case, marked as vistied.
 * 1. bfs is top to down, operate then expand
 * 2. dfs is down to top, expand then operate when back
 * */

/*
 * Start from the left most node, mark as vistied.
 *  foreach c in node.children(): (exclude wall)
 *      if unvisited, **mark visited first**:
 *          bfs(c)
 * */
int main(void)
{
    enqueue(0);
    visited[0][0] = 1;
    int v = 0; // start from left most node
    while (!empty() && v != 24) {
        v = dequeue();
        int i = v / 5, j = v % 5;
        // do not violates the wall invarints, visited invarints
        if (i + 1 < 5 && visited[i+1][j]==0 && maze[i+1][j] == 0) {
            enqueue((i+1)*5 + j);
            parent[i+1][j] = v;
            visited[i+1][j] = 1;
        }
        if (j + 1 < 5 && visited[i][j+1] == 0 && maze[i][j+1] == 0) {
            enqueue(i*5+j+1);
            parent[i][j+1] = v;
            visited[i][j+1] = 1;
        }
        if (j - 1 >= 0 && visited[i][j-1] == 0 && maze[i][j-1] == 0) {
            enqueue(i*5+j-1);
            parent[i][j-1] = v;
            visited[i][j-1] = 1;
        }
        if (i - 1 >= 0 && visited[i-1][j] == 0 && maze[i-1][j] == 0) {
            enqueue((i-1)*5+j);
            parent[i-1][j] = v;
            visited[i-1][j] = 1;
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
