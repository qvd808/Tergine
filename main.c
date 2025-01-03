#include "draw.h"
#include "setup.h"
#include "util.h"
#include <curses.h>
#include <math.h>
#include <ncurses.h>
#include <signal.h>
#include <stdbool.h>

int running = 1;
void exit_program_handler(int n);

struct Triangle {
  struct Point3d vec[3];
};

struct Cube {
  struct Triangle mesh[12];
};

void MultiplyMatrixVector(vec4d *input, vec4d *output, mat4x4 *m) {
  (*output)[0] = (*input)[0] * (*m)[0][0] + (*input)[1] * (*m)[1][0] +
                 (*input)[2] * (*m)[2][0] + (*m)[3][0];
  (*output)[1] = (*input)[0] * (*m)[0][1] + (*input)[1] * (*m)[1][1] +
                 (*input)[2] * (*m)[2][1] + (*m)[3][1];
  (*output)[2] = (*input)[0] * (*m)[0][2] + (*input)[1] * (*m)[1][2] +
                 (*input)[2] * (*m)[2][2] + (*m)[3][2];
  (*output)[3] = (*input)[0] * (*m)[0][3] + (*input)[1] * (*m)[1][3] +
                 (*input)[2] * (*m)[2][3] + (*m)[3][3];

  if ((*output)[3] != 0.0f) {
    (*output)[0] /= (*output)[3];
    (*output)[1] /= (*output)[3];
    (*output)[2] /= (*output)[3];
  }
}

void spinning_obj(struct Point3d **list_points, float fTheta,
                  float fElapsedTime, mat4x4 *matProj) {

  // Control rotation
  mat4x4 matRotZ = {0};
  mat4x4 matRotX = {0};

  vec4d CAMERA = {0};

  // fTheta = 3.3;
  matRotZ[0][0] = cosf(fTheta);
  matRotZ[0][1] = sinf(fTheta);
  matRotZ[1][0] = -sinf(fTheta);
  matRotZ[1][1] = cosf(fTheta);
  matRotZ[2][2] = 1;
  matRotZ[3][3] = 1;

  matRotX[0][0] = 1;
  matRotX[1][1] = cosf(fTheta * 0.5f);
  matRotX[1][2] = sinf(fTheta * 0.5f);
  matRotX[2][1] = -sinf(fTheta * 0.5f);
  matRotX[2][2] = cosf(fTheta * 0.5f);
  matRotX[3][3] = 1;

  int width = getWinWidth() * 2;
  int height = getWinHeight();
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      mvaddstr(j, i, "`");
    }
  }

  float translateZ = 2.5f;
  for (int i = 0; i < 12; i++) {

    vec4d triProjected[3];
    vec4d triRotatedZ[3] = {0};
    vec4d triRotatedZX[3] = {0};

    struct Point3d triTranslated[3];

    for (int j = 0; j < 3; j++) {

      vec4d temp = {0};
      temp[0] = list_points[i][j].x;
      temp[1] = list_points[i][j].y;
      temp[2] = list_points[i][j].z;

      MultiplyMatrixVector(&temp, &triRotatedZ[j], &matRotZ);
    }

    for (int j = 0; j < 3; j++) {

      MultiplyMatrixVector(&triRotatedZ[j], &triRotatedZX[j], &matRotX);
    }

    triTranslated[0].x = triRotatedZX[0][0];
    triTranslated[0].y = triRotatedZX[0][1];
    triTranslated[0].z = triRotatedZX[0][2] + translateZ;

    triTranslated[1].x = triRotatedZX[1][0];
    triTranslated[1].y = triRotatedZX[1][1];
    triTranslated[1].z = triRotatedZX[1][2] + translateZ;

    triTranslated[2].x = triRotatedZX[2][0];
    triTranslated[2].y = triRotatedZX[2][1];
    triTranslated[2].z = triRotatedZX[2][2] + translateZ;

    vec4d normal = {0};
    cross_product((struct Point3d *)&triTranslated, &normal);

    if ((normal[0] * (triTranslated[0].x - CAMERA[0]) +
         normal[1] * (triTranslated[0].y - CAMERA[1]) +
         normal[2] * (triTranslated[0].z - CAMERA[2])) < 0.0f) {

      for (int j = 0; j < 3; j++) {

        vec4d temp = {0};
        temp[0] = triTranslated[j].x;
        temp[1] = triTranslated[j].y;
        temp[2] = triTranslated[j].z;

        MultiplyMatrixVector(&temp, &triProjected[j], &(*matProj));
      }

      struct Point projected_p1 =
          translate_coordinate(triProjected[0][0], triProjected[0][1]);
      struct Point projected_p2 =
          translate_coordinate(triProjected[1][0], triProjected[1][1]);
      struct Point projected_p3 =
          translate_coordinate(triProjected[2][0], triProjected[2][1]);
      draw_triangle(projected_p1, projected_p2, projected_p3);
      /* drawWuTriangle(projected_p1, projected_p2, projected_p3); */
      rasterizeTriangle(projected_p1, projected_p2, projected_p3);
    }
    // free(normal);
  }
}

int main() {

  signal(SIGINT, exit_program_handler);
  init_program();

  // Initialize the cube in space
  struct Cube cube;
  // SOUTH
  struct Point3d tri1[3] = {
      {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}};
  struct Point3d tri2[3] = {
      {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}};

  // EAST
  struct Point3d tri3[3] = {
      {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
  struct Point3d tri4[3] = {
      {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}};

  // NORTH
  struct Point3d tri5[3] = {
      {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}};
  struct Point3d tri6[3] = {
      {1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}};

  // WEST
  struct Point3d tri7[3] = {
      {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}};
  struct Point3d tri8[3] = {
      {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};

  // TOP
  struct Point3d tri9[3] = {
      {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}};
  struct Point3d tri10[3] = {
      {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}};

  // BOTTOM
  struct Point3d tri11[3] = {
      {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};
  struct Point3d tri12[3] = {
      {1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}};

  struct Point3d *list_points[12];
  list_points[0] = tri1;
  list_points[1] = tri2;
  list_points[2] = tri3;
  list_points[3] = tri4;
  list_points[4] = tri5;
  list_points[5] = tri6;
  list_points[6] = tri7;
  list_points[7] = tri8;
  list_points[8] = tri9;
  list_points[9] = tri10;
  list_points[10] = tri11;
  list_points[11] = tri12;

  float fElapsedTime = 0.01;
  float fTheta = 0;

  // Projection Matrix
  float fNear = 0.1f;
  float fFar = 1000.0f;
  float fFov = 90.0f;
  float fAspectRatio = getAspectRatio();
  float fFovRad = 1.0f / tan(fFov * 0.5 / 180.0f * 3.14159f);

  mat4x4 matProj = {0};
  matProj[0][0] = fAspectRatio * fFovRad;
  matProj[1][1] = fFovRad;
  matProj[2][2] = fFar / (fFar - fNear);
  matProj[3][2] = (-fFar * fNear) / (fFar - fNear);
  matProj[2][3] = 1.0f;
  matProj[3][3] = 0.0f;

  while (running) {
    attrset(COLOR_PAIR(3));
    fTheta += 1.0f * fElapsedTime;
    spinning_obj(list_points, fTheta, fElapsedTime, &matProj);

    /* struct Point p1 = {.x = 0, .y = 0}; */
    /* struct Point p2 = {.x = 8, .y = 8}; */
    /* struct Point p3 = {.x = 0, .y = 8}; */
    /**/
    /* attrset(COLOR_PAIR(3)); */
    /* draw_triangle_fill(p1, p2, p3); */
    /* attrset(COLOR_PAIR(1)); */
    /* p1.x += 11; */
    /* p3.x += 11; */
    /* draw_line(p1, p3); */
    /* draw_line(p1, p3); */

    refresh();
    erase();
    usleep(5000);
  }

  endwin();
  return 0;
}

void exit_program_handler(int n) { running = 0; }
