#include "functional.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void scale_obj(double scale, points_a *points_str) {
  matrix_t matrix_scale;
  create_matrix(4, 4, &matrix_scale);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (i == j)
        matrix_scale.matrix[i][j] = scale;
      else
        matrix_scale.matrix[i][j] = 0;
    }
  }
  matrix_scale.matrix[3][3] = 1;
  affine_transformation(points_str, matrix_scale);
  remove_matrix(&matrix_scale);
}

/*          ***** ***** ***** *****          */

void turn_obj(double rotation, points_a *points_str, int axis) {
  matrix_t matrix_turn;
  create_matrix(4, 4, &matrix_turn);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (i == j)
        matrix_turn.matrix[i][j] = 1;
      else
        matrix_turn.matrix[i][j] = 0;
    }
  }
  matrix_rotation(&matrix_turn, rotation, axis);
  affine_transformation(points_str, matrix_turn);
  remove_matrix(&matrix_turn);
}

/*          ***** ***** ***** *****          */

void matrix_rotation(matrix_t *matrix_turn, double rotation, int axis) {
  double gradus = rotation * M_PI / 180.0;
  if (axis == 1) {
    matrix_turn->matrix[1][1] = cos(gradus);
    matrix_turn->matrix[1][2] = sin(gradus);
    matrix_turn->matrix[2][1] = -sin(gradus);
    matrix_turn->matrix[2][2] = cos(gradus);
  } else if (axis == 2) {
    matrix_turn->matrix[0][0] = cos(gradus);
    matrix_turn->matrix[0][2] = -sin(gradus);
    matrix_turn->matrix[2][0] = sin(gradus);
    matrix_turn->matrix[2][2] = cos(gradus);
  } else if (axis == 3) {
    matrix_turn->matrix[0][0] = cos(gradus);
    matrix_turn->matrix[0][1] = sin(gradus);
    matrix_turn->matrix[1][0] = -sin(gradus);
    matrix_turn->matrix[1][1] = cos(gradus);
  }
}

/*          ***** ***** ***** *****          */

void move_obj(point_t *move_point, points_a *points_str) {
  matrix_t matrix_move;
  create_matrix(4, 4, &matrix_move);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (i == j)
        matrix_move.matrix[i][j] = 1;
      else
        matrix_move.matrix[i][j] = 0;
    }
  }
  matrix_move.matrix[0][3] = move_point->x;
  matrix_move.matrix[1][3] = move_point->y;
  matrix_move.matrix[2][3] = move_point->z;
  affine_transformation(points_str, matrix_move);
  remove_matrix(&matrix_move);
}

/*          ***** ***** ***** *****          */

void affine_transformation(points_a *points_str, matrix_t matrix_affin) {
  matrix_t matrix_point, result;
  create_matrix(4, 1, &matrix_point);
  create_matrix(4, 1, &result);
  matrix_point.matrix[3][0] = 1;
  for (int i = 0; i < points_str->count_points; i++) {
    matrix_point.matrix[0][0] = points_str->points[i].x;
    matrix_point.matrix[1][0] = points_str->points[i].y;
    matrix_point.matrix[2][0] = points_str->points[i].z;
    mult_matrix(&matrix_affin, &matrix_point, &result);
    points_str->points[i].x = result.matrix[0][0];
    points_str->points[i].y = result.matrix[1][0];
    points_str->points[i].z = result.matrix[2][0];
  }
  remove_matrix(&matrix_point);
  remove_matrix(&result);
}

/*          ***** ***** ***** *****          */

int create_matrix(int rows, int columns, matrix_t *result) {
  int back = 0;
  if (rows > 0 && columns > 0) {
    result->rows = rows;
    result->columns = columns;
    if (!(result->matrix = (double **)calloc(rows,  sizeof(double*))))
      back = 1;
    for (int i = 0; i < rows; i++)
      if (!(result->matrix[i] = (double *)calloc(columns , sizeof(double))))
        back = 1;
  } else {
    back = 1;
  }
  return back;
}

/*          ***** ***** ***** *****          */

void remove_matrix(matrix_t *A) {
  if (A->matrix != NULL) {
    for (int i = 0; i < A->rows; i++)
      free(A->matrix[i]);
    free(A->matrix);
  }
}

/*          ***** ***** ***** *****          */

int mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int back = 0;
  double summ = 0;
  if (A->columns == B->rows) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < B->columns; j++) {
        for (int k = 0; k < A->columns; k++) {
          summ += A->matrix[i][k] * B->matrix[k][j];
        }
        result->matrix[i][j] = summ;
        summ = 0;
      }
    }
  } else {
    back = 2;
  }
  return back;
}

/*          ***** ***** ***** *****          */

int pars_file(char const *file, points_a *points_array, matrix_poligon *polygons) {
  FILE *fp;
  char string[1000] = {0};
  char sep[4] = {" "};
  char *istr = NULL;
  char *trash = NULL;
  int count1 = 1;
  int memory_v = 512;
  int count2 = 0;
  int count3 = 0;
  int memory_t = 512;
  int is_error = 1;

  points_array->points = (point_t *)calloc(memory_v, sizeof(point_t));
  polygons->poligons = (matrix_int *)calloc(memory_t, sizeof(matrix_int));
  if ((fp = fopen(file, "r")) != NULL) {
      points_array->max_size = 0;
    while (fgets(string, 1000, fp) != NULL) {
      istr = strtok(string, sep);
      if (istr[0] == 'v' && istr[1] == '\0') {
        if (count1 >= memory_v) {
          memory_v *= 2;
          points_array->points = (point_t *)realloc(points_array->points,
                                                    memory_v * sizeof(point_t));
        }
        points_array->points[count1].x = strtod(strtok(NULL, sep), &trash);
        points_array->points[count1].y = strtod(strtok(NULL, sep), &trash);
        points_array->points[count1].z = strtod(strtok(NULL, sep), &trash);
        add_max_size(points_array->points[count1],  &points_array->max_size);
        count1++;
      } else if (istr[0] == 'f' && istr[1] == '\0') {
        count3 = count_poligon(string + 2);
        polygons->poligons[count2].points = (int *)calloc(count3, sizeof(int));
        for (int i = 0; i < count3; i++)
          polygons->poligons[count2].points[i] =
              strtod(str_replace(strtok(NULL, sep), '/'), &trash);
        polygons->poligons[count2].columns = count3;
        count2++;
        if (count2 >= memory_t) {
          memory_t *= 2;
          polygons->poligons = (matrix_int *)realloc(
              polygons->poligons, (memory_t) * sizeof(matrix_int));
        }
      }
    }
    points_array->count_points = count1;
    polygons->rows = count2;
    fclose(fp);
  } else {
    is_error = 0;
  }
  return (is_error);
}

/*          ***** ***** ***** *****          */

void add_max_size(point_t point, double *max) {
    if (fabs(point.x) > *max) *max = fabs(point.x);
    if (fabs(point.y) > *max) *max = fabs(point.y);
    if (fabs(point.z) > *max) *max = fabs(point.z);
}

/*          ***** ***** ***** *****          */

int count_poligon(char *string) {
  int res = string[0] >= '0' && string[0] <= '9' ? 1 : 0;
  for (int i = 0; string[i]; i++)
    if (string[i] == ' ' && string[i + 1] >= '0' && string[i + 1] <= '9')
      res++;
  return res;
}

/*          ***** ***** ***** *****          */

char *str_replace(char *str, char symbol) {
  for (int i = 0; str[i]; i++) {
    if (str[i] == symbol) {
      str[i] = '\0';
      break;
    }
  }
  return str;
}

/*          ***** ***** ***** *****          */

void free_matrix_int(matrix_poligon *polygons) {
    if (polygons) {
        for (int i = 0; i < polygons->rows; i++) {
            if (polygons->poligons[i].points) {
                free(polygons->poligons[i].points);
                polygons->poligons[i].points = NULL;
            }
        }
        free(polygons->poligons);
        polygons->poligons = NULL;
    }
}
