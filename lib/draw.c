#include "draw.h"
#include "util.h"
#include <curses.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void draw_rect(int x, int y, int width, int height) {

  char cBlock = (char)0x2588;

  struct winsize size;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
    printf("TIOCGWINSZ error!\n");
  }

  for (int i = 0; i < height; i++) {
    move(y + i, x);
    for (int j = 0; j < width; j++) {

      // printw("\xc3\x7f");
      mvaddstr(y + i, x + 2 * j, "^#");
      // mvaddch(y + i, x + 2*j, cBlock);
      // addch(A_REVERSE);

      if (j + x >= size.ws_col - 1) {
        break;
      }
    }
    if (i + y >= size.ws_row - 1) {
      break;
    }
  }
}

void draw_right_triangle(int x, int y, int width, int height) {

  struct winsize size;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
    printf("TIOCGWINSZ error!\n");
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < i + 1; j++) {

      // printw("\xc3\x7f");
      if (j == 0 || j == i || i == height - 1) {
        // printw(" *");
        mvaddstr(y + i, x, " *");
      } else {
        // printw(" .");
        // printw("  ");
        mvaddstr(y + i, x, "||");
      }

      if (j + x >= size.ws_col - 1) {
        break;
      }
    }

    if (i + y >= size.ws_row - 1) {
      break;
    }
  }
}

struct PointVector *draw_line_test(struct Point p1, struct Point p2) {

  struct PointVector *res = NULL;

  int dx = abs(p2.x - p1.x);
  int dy = abs(p2.y - p1.y);
  int length = MAX(dx, dy) + 1;

  res = malloc(sizeof(struct PointVector));
  if (res == NULL)
    return NULL;
  res->length = length;
  res->points = malloc(sizeof(struct Point) * length);
  if (res->points == NULL) {
    free(res);
    return NULL;
  };

  int x = p1.x;
  int y = p1.y;
  int err = dx - dy;

  for (int i = 0; i < length; i++) {
    res->points[i] = (struct Point){x, y};

    if (x == p2.x && y == p2.y)
      break;

    // e2 = 2 * (dx - dy) => 2 * (dx - dy) >= -dy
    // 2dx >= 2dy
    // dx/dy >= 1
    int e2 = 2 * err;
    if (e2 >= -dy) {
      err -= dy;
      x += 1;
    }
    // e2 = 2 * (dx - dy) => 2 * (dx - dy) <= dx
    // dx <= 2*dy
    // dy/dx >= 1/2
    if (e2 <= dx) {
      err += dx;
      y += 1;
    }
  }

  return res;
}

int draw_triangle(struct Point p1, struct Point p2, struct Point p3) {

  draw_line(p1, p2);
  draw_line(p2, p3);
  draw_line(p3, p1);

  return 1;
}

int draw_line(struct Point start, struct Point end) {
  struct winsize size;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
    printf("TIOCGWINSZ error!\n");
    return 0;
  }

  start.x *= 2;
  end.x *= 2;

  // //Draw vertical
  if (end.x - start.x == 0) {
    int dy;
    if (end.y - start.y < 0) {
      dy = -1;
    } else {
      dy = 1;
    }
    for (int i = 0; i < abs(end.y - start.y); i++) {
      mvaddstr(start.y + dy * i, start.x, "*");
    }

    return 1;
  }

  // Draw horizontal
  if (end.y - start.y == 0) {
    int dx;
    if (end.x - start.x < 0) {
      dx = -1;
    } else {
      dx = 1;
    }
    for (int i = 0; i < abs(end.x - start.x); i += 2) {
      mvaddstr(start.y, start.x + dx * i, "*");
    }
    return 1;
  }

  if (abs(end.y - start.y) < abs(end.x - start.x)) {
    if (start.x > end.x) {
      plotLineLow(end.x, end.y, start.x, start.y);
    } else {
      plotLineLow(start.x, start.y, end.x, end.y);
    }
  } else {

    if (start.y > end.y) {
      plotLineHigh(end.x, end.y, start.x, start.y);
    } else {
      plotLineHigh(start.x, start.y, end.x, end.y);
    }
  }
  return 1;
}

// Bresenhams's line algorithm or Xiaolin Wu's Line algorithm
void plotLineLow(int x0, int y0, int x1, int y1) {

  int dx = (x1 - x0);
  int dy = y1 - y0;
  int yi = 1;

  if (dy < 0) {
    yi = -1;
    dy = -dy;
  }

  int D = (2 * dy) - dx;
  int y = y0;

  for (int i = 0; i <= (x1 - x0); i += 1) {
    mvaddstr(y, x0 + i, "*");
    if (D > 0) {
      y = y + yi;
      D += (2 * (dy - dx));
    } else {
      D += 2 * dy;
    }
  }
}

void plotLineHigh(int x0, int y0, int x1, int y1) {

  int dx = (x1 - x0);
  int dy = y1 - y0;
  int xi = 1;

  if (dx < 0) {
    xi = -1;
    dx = -dx;
  }

  int D = (2 * dx) - dy;
  int x = x0;

  for (int i = 0; i <= y1 - y0; i++) {
    mvaddstr(y0 + i, x, "*");
    if (D > 0) {
      x = x + xi;
      D += ((dx - dy));
    } else {
      D += dx;
    }
  }
}

/* -----------------------TEST AREA---------------------------- */

int map_range(float range_start, float range_end, float value, int n) {
  // Handle edge case where range_start equals range_end
  if (range_start == range_end) {
    return 0;
  }

  // Normalize to 0-1 range
  float normalized = (value - range_start) / (range_end - range_start);

  // Clamp between 0 and 1
  if (normalized < 0)
    normalized = 0;
  if (normalized > 1)
    normalized = 1;

  // Map to target range and round to nearest integer
  return (int)(normalized * n + 0.5f); // Adding 0.5 for rounding
}

void plotPixel(int x, int y, float brightness) {
  // Select characters based on brightness level

  /* if (brightness <= 0.4) { */
  /*   const unsigned char value1 = */
  /*       ASCII_CHARS_TOP[map_range(0, 0.4, brightness, ASCII_TOP_LEN)]; */
  /*   const unsigned char value2 = */
  /*       ASCII_CHARS_BOTTOM[map_range(0.6, 1, 1 - brightness,
   * ASCII_BOTTOM_LEN)]; */
  /*   mvaddch(y + 1, x, value1); */
  /*   mvaddch(y, x, value2); */
  /* } else if (brightness >= 0.6) { */
  /*   const unsigned char value1 = */
  /*       ASCII_CHARS_TOP[map_range(0.6, 1, brightness, ASCII_TOP_LEN)]; */
  /*   const unsigned char value2 = */
  /*       ASCII_CHARS_BOTTOM[map_range(0, 0.4, 1 - brightness,
   * ASCII_BOTTOM_LEN)]; */
  /*   mvaddch(y + 1, x, value1); */
  /*   mvaddch(y, x, value2); */
  /* } else { */
  /*   const unsigned char value1 = */
  /*       ASCII_CHARS_FILL[map_range(0.4, 0.6, brightness, ASCII_FILL_LEN)]; */
  /*   const unsigned char value2 = */
  /*       ASCII_CHARS_FILL[map_range(0.4, 0.6, 1 - brightness,
   * ASCII_FILL_LEN)]; */
  /*   mvaddch(y + 1, x, value1); */
  /*   mvaddch(y, x, value2); */
  /* } */

  const unsigned char value1 =
      ASCII_CHARS[map_range(0, 1, brightness, ASCII_CHARS_LEN)];
  const unsigned char value2 =
      ASCII_CHARS[map_range(0, 1, 1 - brightness, ASCII_CHARS_LEN)];
  mvaddch(y + 1, x, value1);
  mvaddch(y, x, value2);
}

void drawWuLine(struct Point p1, struct Point p2) {
  p1.x *= 2;
  p2.x *= 2;
  float x0 = p1.x, y0 = p1.y;
  float x1 = p2.x, y1 = p2.y;

  // Determine if line is steep
  const bool steep = fabs(y1 - y0) > fabs(x1 - x0);
  if (steep) {
    // Swap x and y coordinates
    float temp = x0;
    x0 = y0;
    y0 = temp;
    temp = x1;
    x1 = y1;
    y1 = temp;
  }

  // Ensure line is drawn left to right
  if (x0 > x1) {
    float temp = x0;
    x0 = x1;
    x1 = temp;
    temp = y0;
    y0 = y1;
    y1 = temp;
  }

  const float dx = x1 - x0;
  const float dy = y1 - y0;
  float gradient = (dx == 0) ? 1 : dy / dx;

  // Handle first endpoint
  float xend = round(x0);
  float yend = y0 + gradient * (xend - x0);
  float xgap = 1 - fmod(x0 + 0.5, 1.0);
  int xpxl1 = (int)xend;
  int ypxl1 = (int)yend;

  if (steep) {
    plotPixel(ypxl1, xpxl1, (1 - fmod(yend, 1.0)) * xgap);
    plotPixel(ypxl1 + 1, xpxl1, fmod(yend, 1.0) * xgap);
  } else {
    plotPixel(xpxl1, ypxl1, (1 - fmod(yend, 1.0)) * xgap);
    plotPixel(xpxl1, ypxl1 + 1, fmod(yend, 1.0) * xgap);
  }

  float intery = yend + gradient; // First y-intersection for the main loop

  // Handle second endpoint
  xend = round(x1);
  yend = y1 + gradient * (xend - x1);
  xgap = fmod(x1 + 0.5, 1.0);
  int xpxl2 = (int)xend;
  int ypxl2 = (int)yend;

  if (steep) {
    plotPixel(ypxl2, xpxl2, (1 - fmod(yend, 1.0)) * xgap);
    plotPixel(ypxl2 + 1, xpxl2, fmod(yend, 1.0) * xgap);
  } else {
    plotPixel(xpxl2, ypxl2, (1 - fmod(yend, 1.0)) * xgap);
    plotPixel(xpxl2, ypxl2 + 1, fmod(yend, 1.0) * xgap);
  }

  if (steep) {
    for (int x = xpxl1 + 1; x < xpxl2; x++) {
      plotPixel((int)intery, x, 1 - fmod(intery, 1.0));
      plotPixel((int)intery + 1, x, fmod(intery, 1.0));
      intery += gradient;
    }
  } else {
    for (int x = xpxl1 + 1; x < xpxl2; x++) {
      plotPixel(x, (int)intery, 1 - fmod(intery, 1.0));
      plotPixel(x, (int)intery + 1, fmod(intery, 1.0));
      intery += gradient;
    }
  }
}

void drawWuTriangle(struct Point p1, struct Point p2, struct Point p3) {

  drawWuLine(p1, p2);
  drawWuLine(p2, p3);
  drawWuLine(p3, p1);
}

void test(struct Point p1, struct Point p2) {
  // y =  ax + b
  // a = y2 - y1/x2 - x1 => y = (y2 - y1)x/(x2 - x1) + b
  // => (x2 - x1) * y = (y2 - y1) * x + (x2 - x1)* b
  // Let B = -(x2 - x1), A = (y2 - y1), C = (x2 - x1) * b
  // => -By = Ax + C
  // => 0 = Ax + By + C
  /*
   * Source: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
   * The point is always start with p1.x, p1.y. We should choose whether we want
   * (p1.x + 1, p1.y) or (p1.x + 1, p1.y + 1). We shoudl choose based on which
   * point is closer to the original line. Therefore evaluate at f(x_0 + 1, y_0
   * + 1/2). if the function is positve, the line below midpoint hench f(x_0 +
   * 1, y_0 + 1) else vice versa. To do integer, we need f(x_i + 1, y_i + 1/2) -
   * f(x_0, y_0). If (x_0 + 1, y_0) is choosen, the change in D_i will be
   * delta_y else change is delta_y - delta_x
   *
   */

  drawWuLine(p1, p2);
}

int draw_triangle_fill(struct Point p1, struct Point p2, struct Point p3) {

  test(p1, p2);
  return 1;
}

// Helper function to compute the edge function
float edgeFunction(struct Point v0, struct Point v1, struct Point p) {
  return (p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x);
}

// Rasterize a filled triangle
void rasterizeTriangle(struct Point p0, struct Point p1, struct Point p2) {
  // Bounding box of the triangle
  p0.x *= 2;
  p1.x *= 2;
  p2.x *= 2;
  int minX = fmin(fmin(p0.x, p1.x), p2.x);
  int minY = fmin(fmin(p0.y, p1.y), p2.y);
  int maxX = fmax(fmax(p0.x, p1.x), p2.x);
  int maxY = fmax(fmax(p0.y, p1.y), p2.y);

  // Ensure bounds are within screen dimensions
  struct winsize size;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) < 0) {
    printf("TIOCGWINSZ error!\n");
    return;
  }
  minX = fmax(0, minX);
  minY = fmax(0, minY);
  maxX = fmin(size.ws_col - 1, maxX);
  maxY = fmin(size.ws_row - 1, maxY);

  // Precompute the edge function values for the triangle edges
  float area = edgeFunction(p0, p1, p2);

  // Iterate over the bounding box
  for (int y = minY; y <= maxY; y++) {
    for (int x = minX; x <= maxX; x++) {
      struct Point p = {x, y};

      // Compute barycentric coordinates
      float w0 = edgeFunction(p1, p2, p);
      float w1 = edgeFunction(p2, p0, p);
      float w2 = edgeFunction(p0, p1, p);

      // Check if the point is inside the triangle
      if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
        // Normalize weights for interpolation if needed
        w0 /= area;
        w1 /= area;
        w2 /= area;

        // Draw pixel
        mvaddch(y, x, '*');
      }
    }
  }
}
