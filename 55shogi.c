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

int max(int a, int b)
{
    return (a > b ? a : b);
}

void youWin()
{
    puts("You Win");
}

void youLose()
{
    puts("You Lose");
}

void draw()
{
    puts("Draw");
}

void swapTurn()
{
    gTurn = (gTurn == PLAYER ? AI : PLAYER);
}

char *int2KomaName(int n)
{
    switch (n)
    {
    case FU:
        return "FU";
    case GIN:
        return "GI";
    case KAKU:
        return "KK";
    case HISHA:
        return "HI";
    case KIN:
        return "KI";
    case KING:
        return "KG";
    case TOKIN:
        return "TK";
    case NARIGIN:
        return "NG";
    case UMA:
        return "UM";
    case RYU:
        return "RY";
    case NONE:
        return "--";
    }
}

int komaName2Int(char *s)
{
    if (strcmp(s, "FU") == 0)
        return FU;
    if (strcmp(s, "GI") == 0)
        return GIN;
    if (strcmp(s, "KK") == 0)
        return KAKU;
    if (strcmp(s, "HI") == 0)
        return HISHA;
    if (strcmp(s, "KI") == 0)
        return KIN;
    if (strcmp(s, "KG") == 0)
        return KING;
    if (strcmp(s, "TK") == 0)
        return TOKIN;
    if (strcmp(s, "NG") == 0)
        return NARIGIN;
    if (strcmp(s, "UM") == 0)
        return UMA;
    if (strcmp(s, "RY") == 0)
        return RYU;
    return -1;
}

int getBaseKoma(int koma)
{
    switch (koma)
    {
    case TOKIN:
        return FU;
    case NARIGIN:
        return GIN;
    case UMA:
        return KAKU;
    case RYU:
        return HISHA;
    default:
        return koma;
    }
}

int getSuperKoma(int koma)
{
    switch (koma)
    {
    case FU:
        return TOKIN;
    case GIN:
        return NARIGIN;
    case KAKU:
        return UMA;
    case HISHA:
        return RYU;
    default:
        return koma;
    }
}

int getVY(int koma, int i)
{
    return gTurn == PLAYER ? gVY[koma][i] : -gVY[koma][i];
}

int getVX(int koma, int i)
{
    return gVX[koma][i];
}

void show()
{
    int i, j;
    puts("    A | B | C | D | E");
    for (i = 4; i >= 0; i--)
    {
        printf("%d ", i + 1);
        for (j = 0; j < 5; j++)
        {
            if (gBoard[i][j] == NONE)
            {
                printf("|...");
                continue;
            }
            printf(gWhich[i][j] == PLAYER ? "|p" : "|a");
            printf(int2KomaName(gBoard[i][j]));
        }
        puts("|");
    }
    puts("");
    puts("   FU GI KK HI KI");
    printf("p ");
    for (i = 0; i < 5; i++)
        printf("  %d", gKomaStock[PLAYER][i]);
    printf("\na ");
    for (i = 0; i < 5; i++)
        printf("  %d", gKomaStock[AI][i]);

    puts("\n");
}

int onTheBoard(int y, int x) // ボード上なら1, それ以外は0
{
    if (0 <= y && y < 5 && 0 <= x && x < 5)
        return 1;
    else
        return 0;
}

int isInTekijin(int y) // 敵陣の中なら1, そうでないなら0
{
    if (gTurn == PLAYER)
        return y == 4;
    else
        return y == 0;
}

int sameBoardLog(int x, int y) // x手目とy手目の盤面、持ち駒、手番が同じなら1を返す。
{
    int i, j;
    if (x % 2 != y % 2)
        return 0;

    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
            if (gBoardLog[x][i][j] != gBoardLog[y][i][j] || gWhichLog[x][i][j] != gWhichLog[y][i][j])
                return 0;

    for (i = 0; i < 2; i++)
        for (j = 0; j < 5; j++)
            if (gKomaStockLog[x][i][j] != gKomaStockLog[y][i][j])
                return 0;

    return 1;
}

void writeOuteLog() //　gCnt手目で王手であったかどうかのログをgOuteLogに記録する。
{
    int y1, x1, y2, x2, k, koma;

    gOuteLog[gCnt] = 0;
    for (y1 = 0; y1 < 5; y1++)
        for (x1 = 0; x1 < 5; x1++)
        {
            if (gWhich[y1][x1] != gTurn)
                continue;

            koma = gBoard[y1][x1];
            for (k = 0; k < gVL[koma]; k++)
            {
                y2 = y1 + getVY(koma, k);
                x2 = x1 + getVX(koma, k);

                if (movable(y1, x1, y2, x2, 0, 0) && gBoard[y2][x2] == KING)
                    gOuteLog[gCnt] = 1;
            }
        }
}

int writeLogAndCheckSennnichite() // ログを記録し、千日手であるならFOUL_PLAY, そうでなりならVALID_PLAYを返す。
{
    int i, j, k;
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
        {
            gBoardLog[gCnt][i][j] = gBoard[i][j];
            gWhichLog[gCnt][i][j] = gWhich[i][j];
        }

    for (i = 0; i < 2; i++)
        for (j = 0; j < 5; j++)
            gKomaStockLog[gCnt][i][j] = gKomaStock[i][j];

    int sameBoardCnt = 0;
    for (i = 0; i < gCnt; i++)
        sameBoardCnt += sameBoardLog(i, gCnt);

    return (sameBoardCnt >= 4 ? FOUL_PLAY : VALID_PLAY);
}

int judge(int n, int verbose)
{
    // 操作を行った後に呼ぶ。
    // 操作後n手先以内に必勝であればWILL_WIN
    // n手以内に必敗であればWILL_LOSE
    // そうでなければEQUAL_FIGHTを返す

    // 移動した結果が王手であったかどうかを記録する。
    writeOuteLog();

    if (writeLogAndCheckSennnichite() == FOUL_PLAY) // 千日手が成立する場合
    {
        if (gCnt >= 2 && gOuteLog[gCnt] && gOuteLog[gCnt - 2]) // 連続王手による千日手
        {
            if (verbose)
                puts("連続王手による千日手の場合、王手を掛けている側が手を変えなければなりません。");
            return WILL_LOSE;
        }
        else
        {
            if (verbose) // 通常の千日手は先手の負け
                puts("千日手が成立すると、先手側の負けとなります。");
            return (gSente == gTurn ? WILL_LOSE : WILL_WIN);
        }
    }

    if (n == 0)
        return EQUAL_FIGHT;

    swapTurn(); // 一時的に相手に手番を切り替える

    int y, x, y1, x1, y2, x2, k, koma;
    int res = WILL_WIN;
    int opponentChance, opponentNariChance;

    for (y1 = 0; y1 < 5; y1++)
        for (x1 = 0; x1 < 5; x1++)
        {
            if (gWhich[y1][x1] != gTurn)
                continue;

            koma = gBoard[y1][x1];
            for (k = 0; k < gVL[koma]; k++)
            {
                y2 = y1 + getVY(koma, k);
                x2 = x1 + getVX(koma, k);

                opponentChance = move(y1, x1, y2, x2, 0, n - 1, 1, 0);
                opponentNariChance = move(y1, x1, y2, x2, 1, n - 1, 1, 0);

                if (opponentChance == WILL_WIN || opponentNariChance == WILL_WIN)
                { // 相手がn-1手以内に必勝の場合負ける
                    swapTurn();
                    return WILL_LOSE;
                }

                if (opponentChance != WILL_LOSE || opponentNariChance != WILL_LOSE)
                    // 相手にn-1手以内に負け回避のチャンスがある場合自分は必勝ではない。
                    res = EQUAL_FIGHT;
            }
        }

    for (k = 0; k < 5; k++)
    {
        if (gKomaStock[gTurn][k] == 0)
            continue;

        for (y = 0; y < 5; y++)
            for (x = 0; x < 5; x++)
            {
                if (gWhich[y][x] != NEUTRAL)
                    continue;

                opponentChance = place(y, x, k, n - 1, 1, 0);

                if (opponentChance == WILL_WIN)
                {
                    // 相手がn-1手以内に必勝の場合負ける
                    swapTurn();
                    return WILL_LOSE;
                }

                if (opponentChance != WILL_LOSE)
                    // 相手にn-1手以内に負け回避のチャンスがある場合自分は必勝ではない。
                    res = EQUAL_FIGHT;
            }
    }

    swapTurn(); // 一時的に切り替えた手番を戻す
    return res;
}

int move(int y1, int x1, int y2, int x2, int nari, int n, int dryRun, int verbose)
// n手先まで読んで勝てるならWILL_WIN、負けるならWILL_LOSE、いずれでもないならEQUAL_FIGHTを返す。
// 王を取ったときは0手先で勝ち、反則手が渡された場合は0手先で負けるとみなす。
// 引数のdryRunについて、実際に駒を動かさず駒を試しに動かしてみた結果のみが欲しいときはdryRun = 1とする。
{
    int isMovable = movable(y1, x1, y2, x2, nari, verbose);

    if (!isMovable)
        return WILL_LOSE;

    printf("move(y1=%d, x1=%d, y2=%d, x2=%d, nari=%d, n=%d, dryRun=%d )\n", y1, x1, y2, x2, nari, n, dryRun);

    int gotKoma = gBoard[y2][x2];
    if (gotKoma == KING)
        return WILL_WIN;

    if (gotKoma != NONE) // 相手の駒を取る
        gKomaStock[gTurn][getBaseKoma(gotKoma)]++;

    int koma = gBoard[y1][x1];

    // 駒を移動させる
    gBoard[y2][x2] = (nari ? getSuperKoma(koma) : koma);
    gWhich[y2][x2] = gWhich[y1][x1];
    gBoard[y1][x1] = NONE;
    gWhich[y1][x1] = NEUTRAL;
    gCnt++;

    int judgeResult = judge(n, verbose);

    if (dryRun) // dryRunなら元に戻す
    {
        gBoard[y1][x1] = (nari ? getBaseKoma(koma) : koma);
        gWhich[y1][x1] = gTurn;
        gBoard[y2][x2] = NONE;
        gWhich[y2][x2] = NEUTRAL;

        if (gotKoma != NONE)
        { // 取った駒を元に戻す
            gKomaStock[gTurn][getBaseKoma(gotKoma)]--;
            gBoard[y2][x2] = gotKoma;
            gWhich[y2][x2] = (gTurn == PLAYER ? AI : PLAYER);
        }
        gCnt--;
    }

    return judgeResult;
}

int place(int y, int x, int koma, int n, int dryRun, int verbose)
// n手先まで読んで勝てるならWILL_WIN、負けるならWILL_LOSE、いずれでもないならEQUAL_FIGHTを返す。
// 打ち歩詰め以外の反則手が渡された場合は0手先で負けるとみなす。
// 引数のdryRunについて、実際に駒を動かさず駒を試しに動かしてみた結果のみが欲しいときはdryRun = 1とする。
{
    printf("place(y=%d, x=%d, koma=%d, n=%d, dryRun=%d)\n", y, x, koma, n, dryRun);
    int isPlacable = placable(y, x, koma, verbose);

    if (!isPlacable)
        return WILL_LOSE;

    gKomaStock[gTurn][koma]--;
    gWhich[y][x] = gTurn;
    gBoard[y][x] = koma;
    gCnt++;

    int judgeResult = judge(n, verbose);

    if (dryRun)
    {
        gKomaStock[gTurn][koma]++;
        gBoard[y][x] = NONE;
        gWhich[y][x] = NEUTRAL;
        gCnt--;
    }

    return judgeResult;
}

int movable(int y1, int x1, int y2, int x2, int nari, int verbose)
// 動かせるなら1, 動かせないなら0を返す
{
    int i;
    if (!onTheBoard(y1, x1) || !onTheBoard(y2, x2))
    { // 盤上でないなら動かせない
        if (verbose)
            puts("盤上ではないので動かせない");
        return 0;
    }

    if (gWhich[y1][x1] != gTurn)
    { // 動かす側に属する駒が置かれていないなら0
        if (verbose)
            puts("動かす側に属する駒が置かれていない");
        return 0;
    }
    int koma = gBoard[y1][x1];

    int dy = y2 - y1; // 変位ベクトルのx成分
    int dx = x2 - x1; // 変位ベクトルのy成分
    int d = max(abs(dy), abs(dx));
    int ey = dy / d; // 変位方向の単位ベクトルx成分
    int ex = dx / d; // 変位方向の単位ベクトルy成分

    int isPossibleMove = 0;
    for (i = 0; i < gVL[koma]; i++)
        if (getVY(koma, i) == dy && getVX(koma, i) == dx)
            isPossibleMove = 1; // 駒の動かし方と変位が一致すれば1になる変数

    if (!isPossibleMove)
    { // 駒にとって不可能な動きなので0
        if (verbose)
            puts("駒にとって不可能な動きである。");
        return 0;
    }

    for (i = 1; i < d; i++)
        if (gWhich[y1 + ey * i][x1 + ex * i] != NONE) // 駒は別の駒を飛び越えられない
        {
            if (verbose)
                puts("駒は別の駒を飛び越えられない");
            return 0;
        }

    if (gWhich[y2][x2] == gTurn) // 動かした先に自分の駒があるなら反則
    {
        if (verbose)
            puts("動かした先に自分の駒がある");
        return 0;
    }

    if (nari)
    {
        if (!isInTekijin(y1) && !isInTekijin(y2))
        { // 敵陣内を移動しないときに成れない
            if (verbose)
                puts("敵陣内を移動しないときに成れない");
            return 0;
        }

        if (!(koma == FU || koma == GIN || koma == KAKU || koma == HISHA))
        { // 成れるコマでないときは成れない
            if (verbose)
                puts("成れる駒でないときは成れない");
            return 0;
        }
    }
    else
    {
        if (koma == FU && isInTekijin(y2))
        { // 敵陣に入る歩は必ず成らなければならない。
            if (verbose)
                puts("敵陣に入る歩は必ず成らなければならない");
            return 0;
        }
    }

    return 1;
}

int placable(int y, int x, int koma, int verbose)
// 打てるなら1, 打てないなら0を返す
{
    int i;

    if (!onTheBoard(y, x))
    { // 盤上でないなら打てない
        if (verbose)
            puts("盤上でないなら打てない");
        return 0;
    }
    if (!(koma == FU || koma == GIN || koma == KAKU || koma == HISHA || koma == KIN)) // ストックできる駒以外は打てない
    {
        if (verbose)
            puts("ストックできる駒以外は打てない");
        return 0;
    }
    if (gKomaStock[gTurn][koma] == 0)
    { // 持っていない駒は打てない
        if (verbose)
            puts("持っていない駒は打てない");
        return 0;
    }

    if (gWhich[y][x] != NEUTRAL)
    { // 駒が置かれている場所に打てない
        if (verbose)
            puts("駒が置かれている場所に打てない");
        return 0;
    }

    if (koma == FU)
    {
        for (i = 0; i < 5; i++) // 二歩は打てない
            if (gBoard[i][x] == FU && gWhich[i][x] == gTurn)
            {
                if (verbose)
                    puts("二歩");
                return 0;
            }

        if (isInTekijin(y)) // 歩を敵陣に打てない
        {
            if (verbose)
                puts("歩を敵陣に打てない");
            return 0;
        }
    }

    return 1;
}

int executeOrder(char *order) // 相手を詰ませたらCHECK_MATE, 反則ならFOUL_PLAY, いずれでもないならVALID_PLAY
{
    int i, j, res;
    int orderLen = strlen(order);

    if (!(orderLen == 4 || orderLen == 5)) // 4, 5文字以外反則
        return FOUL_PLAY;

    if (!('1' <= order[0] && order[0] <= '5' && 'A' <= order[1] && order[1] <= 'E')) // 命令の最初2文字が1A~5Eでなければ反則
        return FOUL_PLAY;

    int y1 = order[0] - '1';
    int x1 = order[1] - 'A';

    if ('1' <= order[2] && order[2] <= '5' && 'A' <= order[3] && order[3] <= 'E')
    { // 命令の3~4文字目が1A~5Eである場合はコマを動かす。
        int y2 = order[2] - '1';
        int x2 = order[3] - 'A';

        if (orderLen == 5 && order[4] != 'N') // 5文字目があるのにNじゃないなら反則
            return FOUL_PLAY;

        int nari = (orderLen == 5);
        res = move(y1, x1, y2, x2, nari, /* n = */ 2, /* dryRun = */ 0, /* verbose = */ 1);
        if (res == WILL_WIN) // 2手以内に勝てる(=相手が王手を回避できない)ときは勝利
            return CHECK_MATE;
        if (res == WILL_LOSE) // 2手以内に負けるときは反則
            return FOUL_PLAY;
    }
    else
    {
        if (orderLen == 5) // コマを置くときに成ると反則
            return FOUL_PLAY;

        order[0] = order[2];
        order[1] = order[3];
        order[2] = '\0';

        int koma = komaName2Int(order);

        if (koma == -1) // コマが指定されていない場合反則
            return FOUL_PLAY;

        res = place(y1, x1, koma, /* n = */ 2, /* dryRun = */ 0, /* verbose = */ 1);
        if (res == WILL_WIN) // 2手以内に勝てる(=相手が王手を回避できない)ときは勝利
        {
            if (koma == FU) // 打ち歩詰め
                return FOUL_PLAY;
            return CHECK_MATE;
        }
        if (res == WILL_LOSE) // 2手以内に負けるときは反則
            return FOUL_PLAY;
    }

    return VALID_PLAY;
}

int player() // プレイヤーの勝ちならPLAYER_WIN, 負けならAI_WIN, 続けるならCONTINUE_FIGHT
{
    puts("[Player Turn]");
    scanf("%s", gOrder);
    int res = executeOrder(gOrder);
    switch (res)
    {
    case CHECK_MATE:
        return PLAYER_WIN;
    case FOUL_PLAY:
        return AI_WIN;
    case VALID_PLAY:
        return CONITNUE_FIGHT;
    }
}

int ai() // プレイヤーの勝ちならPLAYER_WIN, 負けならAI_WIN, 続けるならCONTINUE_FIGHT
{
    puts("[AI Turn]");
    scanf("%s", gOrder);
    switch (executeOrder(gOrder))
    {
    case CHECK_MATE:
        return AI_WIN;
    case FOUL_PLAY:
        return PLAYER_WIN;
    case VALID_PLAY:
        return CONITNUE_FIGHT;
    }
}

void initVYVX()
{
    gVY[FU] = gFU_VY;
    gVX[FU] = gFU_VX;
    gVL[FU] = 1;
    gVY[GIN] = gGI_VY;
    gVX[GIN] = gGI_VX;
    gVL[GIN] = 5;
    gVY[KAKU] = gKK_VY;
    gVX[KAKU] = gKK_VX;
    gVL[KAKU] = 16;
    gVY[HISHA] = gHI_VY;
    gVX[HISHA] = gHI_VX;
    gVL[HISHA] = 16;
    gVY[KIN] = gKI_VY;
    gVX[KIN] = gKI_VX;
    gVL[KIN] = 6;
    gVY[KING] = gKG_VY;
    gVX[KING] = gKG_VX;
    gVL[KING] = 8;
    gVY[TOKIN] = gTK_VY;
    gVX[TOKIN] = gTK_VX;
    gVL[TOKIN] = 6;
    gVY[NARIGIN] = gNG_VY;
    gVX[NARIGIN] = gNG_VX;
    gVL[NARIGIN] = 6;
    gVY[UMA] = gUM_VY;
    gVX[UMA] = gUM_VX;
    gVL[UMA] = 20;
    gVY[RYU] = gRY_VY;
    gVX[RYU] = gRY_VX;
    gVL[RYU] = 20;
}

void initBoard()
{
    int i, j;
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++)
        {
            gBoard[i][j] = NONE;
            gWhich[i][j] = NEUTRAL;
        }

    gBoard[0][0] = KING;
    gWhich[0][0] = PLAYER;
    gBoard[0][1] = KIN;
    gWhich[0][1] = PLAYER;
    gBoard[0][2] = GIN;
    gWhich[0][2] = PLAYER;
    gBoard[0][3] = KAKU;
    gWhich[0][3] = PLAYER;
    gBoard[0][4] = HISHA;
    gWhich[0][4] = PLAYER;
    gBoard[1][0] = FU;
    gWhich[1][0] = PLAYER;

    gBoard[4][0] = HISHA;
    gWhich[4][0] = AI;
    gBoard[4][1] = KAKU;
    gWhich[4][1] = AI;
    gBoard[4][2] = GIN;
    gWhich[4][2] = AI;
    gBoard[4][3] = KIN;
    gWhich[4][3] = AI;
    gBoard[4][4] = KING;
    gWhich[4][4] = AI;
    gBoard[3][4] = FU;
    gWhich[3][4] = AI;
}

void init()
{
    initVYVX();
    initBoard();
}

void game()
{
    int res;
    init();

    while (1)
    {
        show();
        res = (gTurn == PLAYER ? player() : ai());
        if (res == PLAYER_WIN)
        {
            show();
            youWin();
            break;
        }
        if (res == AI_WIN)
        {
            show();
            youLose();
            break;
        }

        swapTurn(); // ターン交代

        if (gCnt > 150) // 150手を超えて決着が着かなければ引き分け
        {
            draw();
            break;
        }
    }
}

int main()
{
    int x;
    puts("先手なら0、後手なら1を入力してください。");
    scanf("%d", &x);
    if (x == 0)
    {
        gTurn = gSente = PLAYER;
    }
    else
        gTurn = gSente = AI;

    game();
    return 0;
}
