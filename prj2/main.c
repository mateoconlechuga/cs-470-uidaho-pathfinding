// matt waltz
// connect 4 ai algorithm
// cs470 spring 2018

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#define WIDTH 7
#define HEIGHT 6
#define MAX_DEPTH 9
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))

static uint8_t board[HEIGHT][WIDTH];
static int move_next;

enum {
    NONE=-1, EMPTY, AI, HUMAN, CHECK, COMBO=4
};

static char *win_str[] = {
    "Draw Game\n", "AI Won\n", "You Won\n",
};

bool legal(int col) {
    return col < WIDTH && col >= 0 && !board[0][col];
}

void move_col(int col, int player) {
    for (int i=HEIGHT-1; i>=0; i--) {
        if (board[i][col] == EMPTY) {
            board[i][col] = player;
            break;
        }
    }
}

void move_undo(int col) {
    for (int i=0; i<HEIGHT; i++) {
        if (board[i][col]) {
            board[i][col] = EMPTY;
            break;
        }
    }
}

void show_board(void) {
    printf("\n");
    for (int i=0; i<HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++) {
            printf("%c ", board[i][j] == AI ? 'A' : board[i][j] == HUMAN ? 'H' : 'O');
        }
        printf("\n");
    }
    printf("\n");
}

int calc_score(int m, int distance) {
    int score = COMBO - distance;
    switch (m) {
        case 0: return 0;
        case 1: return 1 * score;
        case 2: return 10 * score;
        case 3: return 100 * score;
        default: return 1000;
    }
}

int check_board(void) {
    int k, ai = 0, human = 0;
    for (int i=HEIGHT-1; i>=0; i--) {
        for (int j=0; j<WIDTH; j++) {
            if (j <= CHECK) {
                for (k=0; k<COMBO; k++) {
                        if (board[i][j+k] == AI) ai++;
                        else if (board[i][j+k] == HUMAN) human++;
                        else break;
                }
                if (ai == COMBO || human == COMBO) {
                    return ai == COMBO ? AI : HUMAN;
                }
                ai = human = 0;
            }
            if (i >= CHECK) {
                for (k=0; k<COMBO; k++) {
                        if (board[i-k][j] == AI) ai++;
                        else if (board[i-k][j] == HUMAN) human++;
                        else break;
                }
                if (ai == COMBO || human == COMBO) {
                    return ai == COMBO ? AI : HUMAN;
                }
                ai = human = 0;
            }
            if (j <= CHECK && i >= CHECK) {
                for (k=0; k<COMBO; k++) {
                    if (board[i-k][j+k] == AI) ai++;
                    else if (board[i-k][j+k] == HUMAN) human++;
                    else break;
                }
                if (ai == COMBO || human == COMBO) {
                    return ai == COMBO ? AI : HUMAN;
                }
                ai = human = 0;
            }
            if (j >= CHECK && i >= CHECK) {
                for (k=0; k<COMBO; k++) {
                    if (board[i-k][j-k] == AI) ai++;
                    else if (board[i-k][j-k] == HUMAN) human++;
                    else break;
                }
                if (ai == COMBO || human == COMBO) {
                    return ai == COMBO ? AI : HUMAN;
                }
                ai = human = 0;
            }
        }
    }

    for (int j=0; j<WIDTH; j++) {
        if (!board[0][j]) return NONE;
    }

    return EMPTY;
}

int hueristic(void) {
    int ai = 1, score = 0, empty = 0;
    int i, c, r, j, k = 0, distance = 0;
    for (i=HEIGHT-1; i>=0; i--) {
        for (j=0; j<WIDTH; j++) {
            if (board[i][j] == EMPTY || board[i][j] == HUMAN) continue;
            if (j <= CHECK) {
                for (k=0; k<COMBO; k++) {
                    if (board[i][j+k] == AI) ai++;
                    else if (board[i][j+k] == HUMAN) {
                        ai = empty = 0; break;
                    } else empty++;
                }
                distance = 0;
                if (empty) {
                    for (c=0; c<COMBO; c++) {
                        for (r=i; r<HEIGHT; r++) {
                            if (board[r][j+c] == EMPTY) distance++;
                            else break;
                        }
                    }
                }
                if (distance) {
                    score += calc_score(ai, distance);
                }
                ai = 1;
                empty = 0;
            }
            if (i >= CHECK) {
                for (k=0; k<COMBO; k++) {
                    if (board[i-k][j] == AI) ai++;
                    else if (board[i-k][j] == HUMAN) {
                        ai = 0; break;
                    }
                }
                distance = 0;
                if (ai) {
                    for (r=i-k+1; r<=i-1; r++) {
                        if (board[r][j] == EMPTY) distance++;
                        else break;
                    }
                }
                if (distance) {
                    score += calc_score(ai, distance);
                }
                ai = 1;
                empty = 0;
            }
            if (j >= CHECK) {
                for (k=0; k<COMBO; k++) {
                    if (board[i][j-k] == AI)ai++;
                    else if (board[i][j-k] == HUMAN) {
                        ai = empty = 0; break;
                    } else empty++;
                }
                distance = 0;
                if (empty)
                    for (c=0; c<COMBO; c++) {
                        for (r=i; r<HEIGHT; r++) {
                            if (board[r][j-c] == EMPTY) distance++;
                            else break;
                        }
                    }

                if (distance) {
                    score += calc_score(ai, distance);
                }
                ai = 1;
                empty = 0;
            }
            if (j <= CHECK && i >= CHECK) {
                for (k=0; k<COMBO; k++) {
                    if (board[i-k][j+k] == AI) ai++;
                    else if (board[i-k][j+k] == HUMAN) {
                        ai = empty = 0; break;
                    } else empty++;
                }
                distance = 0;
                if (empty) {
                    for (c=0; c<COMBO; c++) {
                        for (r=i-c; r<HEIGHT; r++) {
                            if (board[r][j+c] == EMPTY) distance++;
                            if (board[r][j+c] == HUMAN) break;
                        }
                    }
                    if (distance) {
                        score += calc_score(ai, distance);
                    }
                    ai = 1;
                    empty = 0;
                }
            }

            if (i >= CHECK && j >= CHECK) {
                for (k=0; k<COMBO; k++) {
                    if (board[i-k][j-k] == AI) ai++;
                    else if (board[i-k][j-k] == HUMAN) {
                        ai = empty = 0; break;
                    } else empty++;
                }
                distance = 0;
                if (empty) {
                    for (c=0; c<COMBO; c++) {
                        for (r=i-c; r<HEIGHT; r++) {
                            if (board[r][j-c] == EMPTY) distance++;
                            if (board[r][j-c] == HUMAN) break;
                        }
                    }
                    if (distance) {
                        score += calc_score(ai, distance);
                    }
                    ai = 1;
                    empty = 0;
                }
            }
        }
    }
    return score;
}

int minmax(int depth, int player, int alpha, int beta) {
    int max = INT_MIN;
    int min = INT_MAX;

    if (beta <= alpha) {
        return player == AI ? INT_MAX : INT_MIN;
    }

    switch (check_board()) {
        case AI: return INT_MAX - 1;
        case HUMAN: return INT_MIN + 1;
        case EMPTY: return 0;
        default: break;
    }

    if (depth == MAX_DEPTH) {
        return hueristic();
    }

    for (int j=0; j<WIDTH; j++) {
        if (!legal(j)) continue;
        int score = 0;
        switch (player) {
            default:
            case AI:
                move_col(j, player);
                score = minmax(depth + 1, HUMAN, alpha, beta);
                if (!depth) {
                    if (score > max) move_next = j;
                    if (score == INT_MAX - 1) {
                        move_undo(j);
                        return score;
                    }
                }
                max = max(score, max);
                alpha = max(score, alpha);
                break;
            case HUMAN:
                move_col(j, player);
                score = minmax(depth + 1, AI, alpha, beta);
                min = min(score, min);
                beta = min(score, beta);
                break;
        }
        move_undo(j);
        if (score == INT_MAX || score == INT_MIN) break;
    }
    return player == AI ? max: min;
}

int move_ai(void) {
    minmax(0, AI, INT_MIN, INT_MAX);
    return move_next;
}

void move_human(void) {
    int move, res;
    printf("move [1-7]: ");
    res = scanf("%d", &move);
    while (res != 1 || !legal(move - 1)) {
        printf("invalid.\n\nmove [1-7]: ");
        res = scanf("%d", &move);
    }
    move_col(move - 1, HUMAN);
}

int main(void) {
    int res;
    char input[20];
    printf("play first? [y/n]: ");
    fgets(input, 20, stdin);

    if (input[0] == 'y') {
        move_human();
    }

    show_board();
    int ai_move = move_ai();
    move_col(3, AI);
    show_board();

    for (;;) {
        move_human();
        show_board();
        if ((res = check_board()) != NONE) {
            printf("%s", win_str[res]);
            break;
        }

        int ai_move = move_ai();
        printf("ai move: %d\n", ai_move + 1);

        move_col(ai_move, AI);
        show_board();
        if ((res = check_board()) != NONE) {
            printf("%s", win_str[res]);
            break;
        }
    }

    return 0;
}
