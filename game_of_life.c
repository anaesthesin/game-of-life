#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 25
#define M 80

void enter_data(int matrix_1[][M]);
void print_matrix(int matrix[][M]);
void change_status(int matrix_1[][M], int matrix_2[][M]);
int count_alive(int matrix_1[][M], int i, int j);
void rewriteMatrix(int matrix_1[][M], int matrix_2[][M]);
int count_alive_cells(int matrix[][M]);

int main() {
    int matrix_1[N][M], matrix_2[N][M], count;
    int speed = 100000;
    enter_data(matrix_1);

    if (freopen("/dev/tty", "r", stdin) == NULL) {
        exit(EXIT_FAILURE);
    }

    initscr();
    curs_set(0);
    nodelay(stdscr, TRUE);
    noecho();

    do {
        print_matrix(matrix_1);
        change_status(matrix_1, matrix_2);
        rewriteMatrix(matrix_1, matrix_2);
        count = count_alive_cells(matrix_1);
        printw("\n\nAmount of alive cells: -%d-\n\n", count);
        printw("Press 'a' to lower speed, press 'z' to up speed");
        char c = getch();
        switch (c) {
            case 'a':
            case 'A':
                if (speed < 1000000) speed += 50000;
                break;
            case 'z':
            case 'Z':
                if (speed > 50000) speed -= 50000;
                break;
        }
        usleep(speed);
    } while (count > 0);

    printf("ALL CELLS ARE DEAD!!!");

    endwin();
    return 0;
}

void enter_data(int matrix_1[][M]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            scanf("%d", &matrix_1[i][j]);
        }
    }
}

void print_matrix(int matrix[][M]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (matrix[i][j] == 1) {
                mvaddch(i, j, '*');
            } else if (matrix[i][j] == 0) {
                mvaddch(i, j, ' ');
            }
        }
    }
    refresh();
}

void change_status(int matrix_1[][M], int matrix_2[][M]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int count = count_alive(matrix_1, i, j);
            if (matrix_1[i][j] == 1) {
                if (count < 2 || count > 3) {
                    matrix_2[i][j] = 0;
                } else {
                    matrix_2[i][j] = 1;
                }
            } else if (matrix_1[i][j] == 0) {
                if (count == 3) {
                    matrix_2[i][j] = 1;
                } else {
                    matrix_2[i][j] = 0;
                }
            }
        }
    }
}

int count_alive(int matrix_1[][M], int i, int j) {
    int count = 0;
    for (int k = -1; k <= 1; k++) {
        for (int t = -1; t <= 1; t++) {
            int copy_i = i;
            int copy_j = j;
            copy_i += k;
            copy_j += t;
            if (copy_i == N) {
                copy_i -= N;
            }
            if (copy_i == -1) {
                copy_i += N;
            }
            if (copy_j == M) {
                copy_j -= M;
            }
            if (copy_j == -1) {
                copy_j += M;
            }
            if ((copy_i == i) && (copy_j == j)) {
                continue;
            } else if (matrix_1[copy_i][copy_j] == 1) {
                count++;
            }
        }
    }
    return count;
}

void rewriteMatrix(int matrix_1[][M], int matrix_2[][M]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            matrix_1[i][j] = matrix_2[i][j];
        }
    }
}

int count_alive_cells(int matrix[][M]) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            count += matrix[i][j];
        }
    }
    return count;
}
