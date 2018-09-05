#ifndef DKNB_GRAPHICS_UTIL
#define DKNB_GRAPHICS_UTIL

#include <windows.h>

static const COLORREF RED = RGB(0xFF, 0, 0);
static const COLORREF GREEN = RGB(0, 0xFF, 0);
static const COLORREF BLUE = RGB(0, 0, 0xFF);
static const COLORREF YELLOW = RGB(0xFF, 0xFF, 0);
static const COLORREF MAGENTA = RGB(0xFF, 0, 0xFF);
static const COLORREF BLACK = RGB(0, 0, 0);
static const COLORREF WHITE = RGB(0xFF, 0xFF, 0xFF);

void gr_dot(HDC hdc, int x, int y, COLORREF color = BLACK) {
  SetPixel(hdc, x, y, color);
}

void gr_fill_rect(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color = GREEN) {
  for (int x = x1; x < x2; ++x) {
    for (int y = y1; y < y2; ++y) {
      gr_dot(hdc, x, y, color);
    }
  }
}

void gr_line(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color = BLACK) {

  int dx = abs(x1 - x2), dy = abs(y1 - y2);
  int s, step;

  if (dx > dy) {
    if (x1 > x2) {
      step = y1 > y2 ? 1 : -1;
      s = x1; x1 = x2; x2 = s;
      s = y1; y1 = y2; y2 = s;
    } else {
      step = y1 < y2 ? 1 : -1;
    }
    gr_dot(hdc, x1, y1, color);
    s = dx >> 1;
    while (++x1 <= x2) {
      if ((s = s - dy) < 0) {
        s = s + dx;
        y1 = y1 + step;
      }
      gr_dot(hdc, x1, y1, color);
    }
  } else { // dx <= dy
    if (y1 > y2) {
      step = x1 > x2 ? 1 : -1;
      s = y1; y1 = y2; y2 = s;
      s = x1; x1 = x2; x2 = s;
    } else {
      step = x1 < x2 ? 1 : -1;
    }
    gr_dot(hdc, x1, y1, color);
    s = dy >> 1;
    while (++y1 <= y2) {
      if ((s = s - dx) < 0) {
        s = s + dy;
        x1 = x1 + step;
      }
      gr_dot(hdc, x1, y1, color);
    }
  }
}

void gr_circle(HDC hdc, int xc, int yc, int r, COLORREF color = BLACK) {
  int x = r, y = 0;
  while (x >= y) {
    gr_dot(hdc, xc + x, yc + y, color);
    gr_dot(hdc, xc + x, yc - y, color);
    gr_dot(hdc, xc - x, yc + y, color);
    gr_dot(hdc, xc - x, yc - y, color);
    gr_dot(hdc, xc + y, yc + x, color);
    gr_dot(hdc, xc + y, yc - x, color);
    gr_dot(hdc, xc - y, yc + x, color);
    gr_dot(hdc, xc - y, yc - x, color);
    if ((r -= (y++ << 1) - 1) < 0) {
      r += (x-- - 1) << 1;
    }
  }
}

#endif //  DKNB_GRAPHICS_UTIL
