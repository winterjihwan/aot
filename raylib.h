#ifndef RAYLIB_H
#define RAYLIB_H

#define FPS 30
#define WINDOW_W 500
#define WINDOW_H 500

#define CELL_W 100
#define CELL_H 100
#define CELL_W_COUNT (WINDOW_W / CELL_W)
#define CELL_H_COUNT (WINDOW_H / CELL_H)
#define CELL_COUNT (CELL_W_COUNT * CELL_H_COUNT)

#define COLOR_WHITE (Color){.r = 255, .g = 255, .b = 255}
#define COLOR_BLACK (Color){.r = 0, .g = 0, .b = 0}
#define COLOR_YELLOW (Color){.r = 255, .g = 252, .b = 127}

#define THETA(n) (n / 360.0 * 2 * M_PI)
#define OOB(x, y) (x < 0 || x > WINDOW_W || y < 0 || y > WINDOW_H)
#define NCP 1
#define GET_CELL(y, x) RAY_MAP[y * CELL_W_COUNT + x]
#define GET_CELL_COOR(y_coor, x_coor)                                          \
  RAY_MAP[(int)y_coor / 100 * CELL_W_COUNT + (int)x_coor / 100]

typedef struct {
  float x, y;
} Vec2;

#endif
