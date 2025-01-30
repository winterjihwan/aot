#ifndef RAYLIB_H
#define RAYLIB_H

// Static
#define TEX_MAX 15

// Game
#define FPS 30
#define WINDOW_W 500
#define WINDOW_H 500
#define FOV 40
#define FAR_CLIPPING_PLANE 150
#define BRIGHTNESS_FACTOR 100

// Minimap
#define MN_SCALE 5

// Cell
#define CELL_W 100
#define CELL_H 100
#define CELL_COUNT_W (WINDOW_W / CELL_W)
#define CELL_COUNT_H (WINDOW_H / CELL_H)
#define CELL_COUNT (CELL_COUNT_W * CELL_COUNT_H)
#define GET_CELL(y, x) RAY_MAP[y * CELL_COUNT_W + x]
#define GET_CELL_COOR(y_coor, x_coor)                                          \
  RAY_MAP[(int)y_coor / CELL_H * CELL_COUNT_W + (int)x_coor / CELL_W]

// Math
#define THETA(n) ((n % 360) / 360.0 * 2 * M_PI)
#define THETAF(n) (fmodf(n, 360) / 360.0 * 2 * M_PI)
#define VELOCITY 5
#define CLAMPF(n, l, h) (n < l ? n : (n > h ? h : n))
#define DISTF(v1, v2) (sqrt(powf(v1.x - v2.x, 2) + powf(v1.y - v2.y, 2)))

// Guard
#define OOB(x, y) (x < 0 || x >= WINDOW_W || y < 0 || y >= WINDOW_H)

typedef struct {
  float x, y;
} Vec2;

#endif
