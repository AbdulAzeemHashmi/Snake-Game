#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
using namespace std;

const int CELL   = 20;
const int ROWS   = 30;
const int COLS   = 30;
const int WINDOW_W = 600;
const int WINDOW_H = 600;

const int SCORE_PER_FOOD   = 10;
const int MAX_TOP          = 5;
const int SPEED_INTERVAL   = 5;      // foods until faster
const int BASE_DELAY_MS    = 150;
const int MIN_DELAY_MS     = 60;
const int CURSOR_BLINK_MS  = 300;

struct Node {
    int x, y;
    Node* next;
};

struct Queue {
    int x, y;
    Queue* next;
};

struct Entry {
    char name[32];
    int  score;
};

struct GameState {
    Node* snakeHead;
    Queue* growthFront;
    Queue* growthRear;
    int foodX, foodY, dirX, dirY, nameLen, score, foodsEaten, timerDelay, topCount, winW, winH;
    bool board[ROWS][COLS], gameOver, paused, askingName, cursorOn, savedMsg;
    char playerName[32];
    Entry top[MAX_TOP];       
};

/* ---------- SAFE STRING COPY ---------- */
inline void safeCopy(char dst[32], const char* src) {
    int i = 0;
    while (i < 31 && src[i]) 
        dst[i] = src[i]; ++i; 
    dst[i] = '\0';
}

/* ---------- MEMORY HELPERS ---------- */
void freeList(Node*& h) {
    while (h) { 
        Node* tmp = h; 
        h = h->next; 
        free(tmp); 
    }
}

void freeQueue(Queue*& f, Queue*& r) {
    while (f) {
        Queue* tmp = f;
        f = f->next;
        free(tmp); 
    }
    r = nullptr;
}

/* ---------- QUEUE ---------- */
void enqueue(Queue*& f, Queue*& r, int x, int y) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->x = x;
    q->y = y;
    q->next = nullptr;
    if (!r)
        f = r = q; 
    else {
        r->next = q;
        r = q; 
    }
}

bool dequeue(Queue*& f, Queue*& r, int& x, int& y) {
    if (!f) 
        return false;
    Queue* tmp = f;
    x = tmp->x; 
    y = tmp->y;
    f = f->next;
    if (!f)
        r = nullptr;
    free(tmp);
    return true;
}

/* ---------- BINARY FILE I/O (robust) ---------- */
void loadBinary(const char* filename, Entry top[MAX_TOP], int& cnt, char currentName[32], int& currentScore) {
    ifstream in(filename, ios::binary);
    if (!in) {
        cnt = 0; currentScore = 0; currentName[0] = '\0';
        return;
    }
    in.read(currentName, 32);
    in.read(reinterpret_cast<char*>(&currentScore), sizeof(int));
    in.read(reinterpret_cast<char*>(&cnt), sizeof(int));
    cnt = min(cnt, MAX_TOP);
    for (int i = 0; i < cnt; ++i)
        in.read(reinterpret_cast<char*>(&top[i]), sizeof(Entry));
    if (!in)  
        cnt = 0; 
}
void saveBinary(const char* filename, const char currentName[32], int currentScore, Entry top[MAX_TOP], int topCnt) {
    ofstream out(filename, ios::binary);
    if (!out) 
        return;
    out.write(currentName, 32);
    out.write(reinterpret_cast<const char*>(&currentScore), sizeof(int));
    topCnt = min(topCnt, MAX_TOP);
    out.write(reinterpret_cast<const char*>(&topCnt), sizeof(int));
    for (int i = 0; i < topCnt; ++i)
        out.write(reinterpret_cast<const char*>(&top[i]), sizeof(Entry));
}

/* ---------- UPDATE TOP-5 ---------- */
void updateTop(Entry top[MAX_TOP], int& cnt, const char name[32], int score) {
    int pos = cnt;
    for (int i = 0; i < cnt; ++i)
        if (score > top[i].score) {
            pos = i; 
            break; 
        }
    if (pos == MAX_TOP) 
        return;
    for (int i = min(cnt, MAX_TOP - 1); i > pos; --i)
        top[i] = top[i - 1];
    safeCopy(top[pos].name, name);
    top[pos].score = score;
    if (cnt < MAX_TOP)
        ++cnt;
}

/* ---------- GAME LOGIC ---------- */
bool isCollision(Node* head, int x, int y) {
    for (Node* cur = head; cur; cur = cur->next)
        if (cur->x == x && cur->y == y) return true;
    return false;
}

void placeFood(GameState& gs) {
    int freeCount = 0;
    for (int y = 0; y < ROWS; ++y)
        for (int x = 0; x < COLS; ++x)
            if (!gs.board[y][x]) 
                ++freeCount;
    if (freeCount == 0) { 
        gs.gameOver = true; 
        return; 
    }

    int target = rand() % freeCount, idx = 0;
    for (int y = 0; y < ROWS; ++y)
        for (int x = 0; x < COLS; ++x)
            if (!gs.board[y][x]) {
                if (idx == target) {
                    gs.foodX = x; 
                    gs.foodY = y; 
                    return; 
                }
                ++idx;
            }
}

void moveSnake(GameState& gs) {
    if (!gs.snakeHead) {
        gs.gameOver = true; 
        return; 
    }

    int newX = gs.snakeHead->x + gs.dirX, newY = gs.snakeHead->y + gs.dirY;

    if (newX < 0 || newX >= COLS || newY < 0 || newY >= ROWS) {
        gs.gameOver = true; 
        return;
    }
    
    if (isCollision(gs.snakeHead, newX, newY)) {
        gs.gameOver = true; 
        return;
    }

    Node* tail = gs.snakeHead;
    while (tail->next) 
        tail = tail->next;
    gs.board[tail->y][tail->x] = false;

    Node* newHead = (Node*)malloc(sizeof(Node));
    newHead->x = newX; newHead->y = newY;
    newHead->next = gs.snakeHead;
    gs.snakeHead = newHead;
    gs.board[newY][newX] = true;

    if (newX == gs.foodX && newY == gs.foodY) {
        gs.score += SCORE_PER_FOOD;
        ++gs.foodsEaten;
        enqueue(gs.growthFront, gs.growthRear, tail->x, tail->y);
        placeFood(gs);
        gs.timerDelay = max(MIN_DELAY_MS, BASE_DELAY_MS - (gs.foodsEaten / SPEED_INTERVAL) * 10);
    } 
    else {
        int gx, gy;
        if (!dequeue(gs.growthFront, gs.growthRear, gx, gy)) {
            Node* prev = nullptr;
            for (Node* cur = gs.snakeHead; cur != tail; cur = cur->next) 
                prev = cur;
            if (prev) 
                prev->next = nullptr;
            else 
                gs.snakeHead = nullptr;
            free(tail);
        }
    }
}

/* ---------- DRAW HELPERS ---------- */
static int drawLine;

void drawEntry(const char* name, int score) {
    char buf[64];
    sprintf(buf, "%s %d", name, score);
    glRasterPos2f(30, drawLine);
    for (char* c = buf; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    drawLine -= 20;
}

/* ---------- GLUT CALLBACKS ---------- */
GameState gs;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    /* ----- NAME INPUT ----- */
    if (gs.askingName) {
        glColor3f(1, 1, 1);
        glRasterPos2f(200, 320);
        const char* msg = "Enter your name:";
        for (const char* c = msg; *c; ++c)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

        glRasterPos2f(200, 290);
        for (int i = 0; i < gs.nameLen; ++i)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, gs.playerName[i]);
        if (gs.cursorOn) 
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '_');
        glutSwapBuffers();
        return;
    }

    /* ----- GRID BACKGROUND ----- */
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_LINES);
    for (int x = 0; x <= COLS; ++x) {
        glVertex2f(x * CELL, 0);
        glVertex2f(x * CELL, WINDOW_H);
    }
    for (int y = 0; y <= ROWS; ++y) {
        glVertex2f(0, y * CELL);
        glVertex2f(WINDOW_W, y * CELL);
    }
    glEnd();

    /* ----- LIVE SCORE + RANK ----- */
    char buf[64];
    sprintf(buf, "Score: %d", gs.score);
    glColor3f(1, 1, 1);
    glRasterPos2f(10, 10);
    for (char* c = buf; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    int rank = 1;
    for (int i = 0; i < gs.topCount; ++i)
        if (gs.score < gs.top[i].score)
            ++rank;
    sprintf(buf, "Rank: %d", rank);
    glRasterPos2f(10, 30);
    for (char* c = buf; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    /* ----- GAME OVER SCREEN ----- */
    if (gs.gameOver) {
        glColor3f(1, 0, 0);
        glRasterPos2f(30, 320);
        const char* msg = "GAME OVER!  Press 'r' to restart, 'q' to quit.";
        for (const char* c = msg; *c; ++c)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

        if (gs.savedMsg) {
            glColor3f(0, 1, 0);
            glRasterPos2f(30, 300);
            const char* saved = "Scores saved!";
            for (const char* c = saved; *c; ++c)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
        }

        glColor3f(1, 1, 0);
        glRasterPos2f(30, 280);
        const char* hs = "Top-5 Scores:";
        for (const char* c = hs; *c; ++c)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

        drawLine = 260;
        for (int i = 0; i < gs.topCount; ++i)
            drawEntry(gs.top[i].name, gs.top[i].score);

        glutSwapBuffers();
        return;
    }

    /* ----- PAUSE MESSAGE + KEY LEGEND ----- */
    if (gs.paused) {
        glColor3f(1, 1, 0);
        glRasterPos2f(250, 300);
        const char* msg = "PAUSED – Press 'p' to resume";
        for (const char* c = msg; *c; ++c)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

        glColor3f(0.8f, 0.8f, 0.8f);
        glRasterPos2f(250, 280);
        const char* leg = "Arrow keys: move  |  q: quit  |  r: restart";
        for (const char* c = leg; *c; ++c)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }

    /* ----- RAINBOW SNAKE ----- */
    int len = 0, idx = 0;
    for (Node* cur = gs.snakeHead; cur; cur = cur->next) ++len;
    float hueStep = 1.0f / (len ? len : 1), hue = 0.0f;
    glBegin(GL_QUADS);
    for (Node* cur = gs.snakeHead; cur; cur = cur->next, ++idx) {
        /* head and tail contrast */
        if (idx == 0) 
            glColor3f(1, 1, 0);                // head
        else if (cur->next == nullptr) 
            glColor3f(0, 0, 1); // tail
        else {
            glColor3f(sin(hue * 6.28f) * 0.5f + 0.5f,
                      sin(hue * 6.28f + 2.09f) * 0.5f + 0.5f,
                      sin(hue * 6.28f + 4.18f) * 0.5f + 0.5f);
        }
        glVertex2f(cur->x * CELL + 1, cur->y * CELL + 1);
        glVertex2f((cur->x + 1) * CELL - 1, cur->y * CELL + 1);
        glVertex2f((cur->x + 1) * CELL - 1, (cur->y + 1) * CELL - 1);
        glVertex2f(cur->x * CELL + 1, (cur->y + 1) * CELL - 1);
        hue += hueStep;
    }
    glEnd();

    /* ----- FOOD ----- */
    glColor3f(1, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(gs.foodX * CELL + 2, gs.foodY * CELL + 2);
    glVertex2f((gs.foodX + 1) * CELL - 2, gs.foodY * CELL + 2);
    glVertex2f((gs.foodX + 1) * CELL - 2, (gs.foodY + 1) * CELL - 2);
    glVertex2f(gs.foodX * CELL + 2, (gs.foodY + 1) * CELL - 2);
    glEnd();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    gs.winW = w; gs.winH = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_W, WINDOW_H, 0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int, int) {
    if (gs.askingName) {
        if (key == 13) {                       // Enter
            gs.playerName[gs.nameLen] = '\0';
            gs.askingName = false;
            return;
        }
        if (key == 8 && gs.nameLen > 0) {      // Backspace
            --gs.nameLen;
            return;
        }
        if (gs.nameLen < 31 && key >= 32 && key <= 126) {
            gs.playerName[gs.nameLen++] = key;
            return;
        }
        return;
    }

    switch (key) {
    case 'q':
    case 'Q': exit(0);
    case 'p': 
    case 'P': if (!gs.gameOver) 
                  gs.paused = !gs.paused; 
                  break;
    case 'r': case 'R':
        if (gs.gameOver) {
            updateTop(gs.top, gs.topCount, gs.playerName, gs.score);
            saveBinary("scores.bin", gs.playerName, gs.score, gs.top, gs.topCount);

            freeList(gs.snakeHead);
            freeQueue(gs.growthFront, gs.growthRear);

            loadBinary("scores.bin", gs.top, gs.topCount,
                       gs.playerName, gs.score);

            gs.snakeHead = (Node*)malloc(sizeof(Node));
            int sx = rand() % (COLS - 4) + 2, sy = rand() % (ROWS - 4) + 2;
            gs.snakeHead->x = sx;
            gs.snakeHead->y = sy;
            gs.snakeHead->next = nullptr;

            gs.growthFront = gs.growthRear = nullptr;
            gs.dirX = 1; gs.dirY = 0;
            gs.gameOver = false;
            gs.paused = false;
            gs.score = 0;
            gs.foodsEaten = 0;
            gs.timerDelay = BASE_DELAY_MS;
            gs.savedMsg = true;

            for (int y = 0; y < ROWS; ++y)
                for (int x = 0; x < COLS; ++x)
                    gs.board[y][x] = false;
            gs.board[sy][sx] = true;
            placeFood(gs);
        }
        break;
    }
}

void special(int key, int, int) {
    if (gs.gameOver || gs.paused || gs.askingName) 
        return;
    int nx = gs.dirX, ny = gs.dirY;
    switch (key) {
        case GLUT_KEY_UP: 
            nx = 0;
            ny = -1; 
            break;
        case GLUT_KEY_DOWN:
            nx = 0;
            ny =  1;
            break;
        case GLUT_KEY_LEFT:  
            nx = -1; 
            ny =  0; 
            break;
        case GLUT_KEY_RIGHT: 
            nx =  1;
            ny =  0; 
            break;
    }
    if (!(nx == -gs.dirX && ny == -gs.dirY)) 
        gs.dirX = nx; gs.dirY = ny;
}

void timer(int) {
    if (!gs.gameOver && !gs.paused && !gs.askingName)
        moveSnake(gs);
    if (!gs.askingName) {
        gs.cursorOn = !gs.cursorOn;
        glutTimerFunc(gs.timerDelay, timer, 0);
    } 
    else {
        gs.cursorOn = !gs.cursorOn;
        glutTimerFunc(CURSOR_BLINK_MS, timer, 0);
    }
    glutPostRedisplay();
}

/* ---------- INITIALIZATION ---------- */
void initGameState() {
    srand(static_cast<unsigned>(time(nullptr)));
    loadBinary("scores.bin", gs.top, gs.topCount, gs.playerName, gs.score);

    gs.askingName = true;
    gs.nameLen = 0;
    gs.playerName[0] = '\0';
    gs.winW = WINDOW_W;
    gs.winH = WINDOW_H;

    gs.snakeHead = (Node*)malloc(sizeof(Node));
    int sx = rand() % (COLS - 4) + 2, sy = rand() % (ROWS - 4) + 2;
    gs.snakeHead->x = sx;
    gs.snakeHead->y = sy;
    gs.snakeHead->next = nullptr;

    gs.growthFront = gs.growthRear = nullptr;
    gs.dirX = 1; gs.dirY = 0;
    gs.gameOver = false;
    gs.paused = false;
    gs.score = 0;
    gs.foodsEaten = 0;
    gs.timerDelay = BASE_DELAY_MS;
    gs.cursorOn = true;
    gs.savedMsg = false;

    for (int y = 0; y < ROWS; ++y)
        for (int x = 0; x < COLS; ++x)
            gs.board[y][x] = false;
    gs.board[sy][sx] = true;
    placeFood(gs);
}

/* ---------- MAIN ---------- */
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_W, WINDOW_H);
    glutCreateWindow("Snake Game");

    initGameState();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutTimerFunc(gs.timerDelay, timer, 0);

    glutMainLoop();
    return 0;
}
