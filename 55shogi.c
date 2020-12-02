#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NONE -1
#define FU 0
#define GIN 1
#define KAKU 2
#define HISHA 3
#define KIN 4
#define KING 5
#define TOKIN 6
#define NARIGIN 7
#define UMA 8
#define RYU 9

#define PLAYER 0   // プレイヤー
#define AI 1       // AI
#define NEUTRAL -1 // 中立

#define FOUL_PLAY -1 // 反則
#define VALID_PLAY 0 // 正当なプレイ
#define CHECK_MATE 1 // 相手を詰ませた

#define WILL_WIN 0    // n手先に必勝
#define WILL_LOSE 1   // n手先に必敗
#define EQUAL_FIGHT 2 // 互角の戦いである

#define PLAYER_WIN 1
#define AI_WIN 0
#define CONITNUE_FIGHT -1

int max(int a, int b);
void youWin();
void youLose();
void draw();
void swapTurn();
char *int2KomaName(int n);
int komaName2Int(char *s);
int getBaseKoma(int koma);
int getSuperKoma(int koma);
void show();
int onTheBoard(int y, int x);
int isInTekijin(int y);
int sameBoardLog(int x, int y);
void writeOuteLog();
int writeLogAndCheckSennnichite();
int judge(int n, int verbose);
int move(int y1, int x1, int y2, int x2, int nari, int n, int dryRun, int verbose);
int movable(int y1, int x1, int y2, int x2, int nari, int verbose);
int place(int y, int x, int koma, int n, int dryRun, int verbose);
int placable(int y, int x, int koma, int verbose);
int executeOrder(char *order);
int player();
int ai();
void initVYVX();
void initBoard();
void init();
void game();

int gBoard[5][5];     // 盤面
int gWhich[5][5];     // playerのコマならPL, aiのコマならAI, どちらのコマでもないならNEUTRAL
int gKomaStock[2][5]; // コマのストック。playerが2個の歩を所持するときkomaStock[PLAYER][FU] == 2である。

int gBoardLog[151][5][5];     // 盤面のログ。千日手判定用。
int gWhichLog[151][5][5];     // 盤面のログ。千日手判定用。
int gKomaStockLog[151][2][5]; // コマのストックのログ。千日手判定用。
int gOuteLog[151];            // 王手であるかどうかのログ。n手目で王手であったときgOuteLog[n] == 1である。

int gCnt = 0;   // 手数
int gTurn = 0;  // ターン
int gSente = 0; // 先手
char gOrder[50];

// FU
int gFU_VY[] = {1};
int gFU_VX[] = {0};

// GI
int gGI_VY[] = {1, 1, 1, -1, -1};
int gGI_VX[] = {-1, 0, 1, -1, 1};

// KK
int gKK_VY[] = {1, 2, 3, 4, 1, 2, 3, 4, -1, -2, -3, -4, -1, -2, -3, -4};
int gKK_VX[] = {-1, -2, -3, -4, 1, 2, 3, 4, -1, -2, -3, -4, 1, 2, 3, 4};

// HI
int gHI_VY[] = {1, 2, 3, 4, 0, 0, 0, 0, -1, -2, -3, -4, 0, 0, 0, 0};
int gHI_VX[] = {0, 0, 0, 0, 1, 2, 3, 4, 0, 0, 0, 0, -1, -2, -3, -4};

// KI
int gKI_VY[] = {1, 1, 1, 0, -1, 0};
int gKI_VX[] = {-1, 0, 1, 1, 0, -1};

// KG
int gKG_VY[] = {1, 1, 1, 0, -1, -1, -1, 0};
int gKG_VX[] = {-1, 0, 1, 1, 1, 0, -1, -1};

// TK
int gTK_VY[] = {1, 1, 1, 0, -1, 0};
int gTK_VX[] = {-1, 0, 1, 1, 0, -1};

// NG
int gNG_VY[] = {1, 1, 1, 0, -1, 0};
int gNG_VX[] = {-1, 0, 1, 1, 0, -1};

// UM
int gUM_VY[] = {1, 2, 3, 4, 1, 2, 3, 4, -1, -2, -3, -4, -1, -2, -3, -4, 1, 0, -1, 0};
int gUM_VX[] = {-1, -2, -3, -4, 1, 2, 3, 4, -1, -2, -3, -4, 1, 2, 3, 4, 0, 1, 0, -1};

// RY
int gRY_VY[] = {1, 2, 3, 4, 0, 0, 0, 0, -1, -2, -3, -4, 0, 0, 0, 0, 1, 1, -1, -1};
int gRY_VX[] = {0, 0, 0, 0, 1, 2, 3, 4, 0, 0, 0, 0, -1, -2, -3, -4, -1, 1, 1, -1};

int *gVY[10];
int *gVX[10];
int gVL[10];

int main()
{
}
