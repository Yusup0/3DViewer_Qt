#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

typedef struct point_struct {
  double x;
  double y;
  double z;
} point_t;

typedef struct points_array {
  point_t *points;
  int count_points;
  double max_size;
} points_a;

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

typedef struct matrix_int {
  int *points;
  int columns;
} matrix_int;

typedef struct matrix_poligon {
  matrix_int *poligons;
  int rows;
} matrix_poligon;

/*          ***** ***** ***** *****          */

void scale_obj(double scale, points_a *points_str);
int create_matrix(int rows, int columns, matrix_t *result);
void remove_matrix(matrix_t *A);
int mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result);
void move_obj(point_t *move_point, points_a *points_str);
void promer();
int pars_file(char const *file, points_a *points_array,
              matrix_poligon *polygons);
int count_poligon(char *string);
char *str_replace(char *str, char symbol);
void free_matrix_int(matrix_poligon *polygons);
void create_matrix_int(matrix_poligon *polygons, int memory_t, int columns);
void expand_matrix_int(matrix_poligon *polygons, int *memory_t, int columns);
void turn_obj(double rotation, points_a *points_str, int axis);
void matrix_rotation(matrix_t *matrix_turn, double rotation, int axis);
void affine_transformation(points_a *points_str, matrix_t matrix_affin);
void max_z_pol(points_a *points_array, matrix_poligon *polygons);
void sort_poligon(matrix_poligon *polygons);
void add_max_size(point_t point, double *max);
void hoare_sort(matrix_poligon *polygons, int start, int end);
void turn_obj(double rotation, points_a *points_str, int axis);

#endif // FUNCTIONAL_H
