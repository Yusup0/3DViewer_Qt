
#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../functional.h"

#define IS_TEST_TURN_ON 0


int eq_double(double num1, double num2) { return fabs(num1 - num2) < 1e-5; }

int size_eq_matrix(matrix_t *A, matrix_t *B) {
  return A->columns == B->columns && A->rows == B->rows;
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int result = 1;
  if (size_eq_matrix(A, B)) {
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++)
        if (!eq_double(A->matrix[i][j], B->matrix[i][j])) {
          result = 0;
          break;
        }
  } else {
    result = 0;
  }
  return result;
}

// -------------------------------------------------------

START_TEST(s21_create_matrix) {
  matrix_t res;
  create_matrix(3, 3, &res);
  ck_assert_int_eq(res.columns, 3);
  ck_assert_int_eq(res.rows, 3);
  remove_matrix(&res);
}
END_TEST

// -------------------------------------------------------

START_TEST(s21_str_replace) {
  char r[] = "privet pr";
  str_replace(r, ' ');
  ck_assert_int_eq(0, strcmp(r, "privet"));
  str_replace(r, 'i');
  ck_assert_int_eq(0, strcmp(r, "pr"));
}
END_TEST

// -------------------------------------------------------

START_TEST(s21_count_poligon) {
  char r[] = "1 2 3 4";
  ck_assert_int_eq(4, count_poligon(r));
  char r1[] = "1 2 3 fr";
  ck_assert_int_eq(3, count_poligon(r1));
}
END_TEST

// -------------------------------------------------------

START_TEST(s21_mult_matrix) {
  matrix_t matrix, matrix1, result, mult;
  create_matrix(3, 1, &matrix);
  create_matrix(1, 3, &matrix1);
  create_matrix(3, 3, &result);
  create_matrix(3, 3, &mult);
  matrix.matrix[0][0] = 1;
  matrix.matrix[1][0] = 4;
  matrix.matrix[2][0] = 1;
  matrix1.matrix[0][0] = 5;
  matrix1.matrix[0][1] = -1;
  matrix1.matrix[0][2] = 0;
  mult_matrix(&matrix, &matrix1, &mult);
  result.matrix[0][0] = 5;
  result.matrix[0][1] = -1;
  result.matrix[0][2] = 0;
  result.matrix[1][0] = 20;
  result.matrix[1][1] = -4;
  result.matrix[1][2] = 0;
  result.matrix[2][0] = 5;
  result.matrix[2][1] = -1;
  result.matrix[2][2] = 0;
  ck_assert_int_eq(1, s21_eq_matrix(&mult, &result));
  remove_matrix(&result);
  remove_matrix(&mult);
  remove_matrix(&matrix1);
  remove_matrix(&matrix);

  create_matrix(2, 3, &matrix);
  create_matrix(3, 2, &matrix1);
  create_matrix(2, 2, &result);
  create_matrix(2, 2, &mult);
  matrix.matrix[0][0] = 5;
  matrix.matrix[0][1] = -1;
  matrix.matrix[0][2] = 3;
  matrix.matrix[1][0] = 4;
  matrix.matrix[1][1] = -1;
  matrix.matrix[1][2] = -1;
  matrix1.matrix[0][0] = 1;
  matrix1.matrix[0][1] = 4;
  matrix1.matrix[1][0] = 4;
  matrix1.matrix[1][1] = 5;
  matrix1.matrix[2][0] = 1;
  matrix1.matrix[2][1] = -9;
  mult_matrix(&matrix, &matrix1, &mult);
  result.matrix[0][0] = 4;
  result.matrix[0][1] = -12;
  result.matrix[1][0] = -1;
  result.matrix[1][1] = 20;
  ck_assert_int_eq(1, s21_eq_matrix(&mult, &result));
  remove_matrix(&result);
  remove_matrix(&mult);
  remove_matrix(&matrix1);
  remove_matrix(&matrix);
}
END_TEST

// -------------------------------------------------------

START_TEST(s21_pars_file) {
  points_a points_array;
  matrix_poligon polygons;
  pars_file("./test_obj.obj", &points_array, &polygons);
  ck_assert_double_eq(0, points_array.points[2].x);
  ck_assert_double_eq(0, points_array.points[2].y);
  ck_assert_double_eq(1, points_array.points[2].z);
  ck_assert_double_eq(1, points_array.points[6].x);
  ck_assert_double_eq(0, points_array.points[6].y);
  ck_assert_double_eq(1, points_array.points[6].z);
  ck_assert_double_eq(1, polygons.poligons[0].points[0]);
  ck_assert_double_eq(7, polygons.poligons[0].points[1]);
  ck_assert_double_eq(5, polygons.poligons[0].points[2]);
  ck_assert_double_eq(3, polygons.poligons[4].points[0]);
  ck_assert_double_eq(8, polygons.poligons[4].points[1]);
  ck_assert_double_eq(7, polygons.poligons[4].points[2]);
  free(points_array.points);
  free_matrix_int(&polygons);

  pars_file("./test2_obj.obj", &points_array, &polygons);
  ck_assert_double_eq(0, points_array.points[2].x);
  ck_assert_double_eq(0, points_array.points[2].y);
  ck_assert_double_eq(1, points_array.points[2].z);
  ck_assert_double_eq(1, points_array.points[6].x);
  ck_assert_double_eq(0, points_array.points[6].y);
  ck_assert_double_eq(1, points_array.points[6].z);
  ck_assert_double_eq(1, polygons.poligons[0].points[0]);
  ck_assert_double_eq(7, polygons.poligons[0].points[1]);
  ck_assert_double_eq(5, polygons.poligons[0].points[2]);
  ck_assert_double_eq(3, polygons.poligons[4].points[0]);
  ck_assert_double_eq(8, polygons.poligons[4].points[1]);
  ck_assert_double_eq(7, polygons.poligons[4].points[2]);
  free(points_array.points);
  free_matrix_int(&polygons);
}
END_TEST

// -------------------------------------------------------

START_TEST(s21_scale_obj) {
  points_a points_array;
  matrix_poligon polygons;
  pars_file("./cow.obj", &points_array, &polygons);
  scale_obj(2, &points_array);
  ck_assert_double_eq(4.13736, points_array.points[2].x);
  ck_assert_double_eq(0.810272, points_array.points[2].y);
  ck_assert_double_eq(-4.335, points_array.points[2].z);
  ck_assert_double_eq(4.36556, points_array.points[6].x);
  ck_assert_double_eq(0.837012, points_array.points[6].y);
  ck_assert_double_eq(-4.20374, points_array.points[6].z);
  free(points_array.points);
  free_matrix_int(&polygons);
}
END_TEST

// -------------------------------------------------------

START_TEST(s21_turn_obj) {
  points_a points_array;
  matrix_poligon polygons;
  pars_file("./cow.obj", &points_array, &polygons);
  turn_obj(50, &points_array, 1);
  ck_assert_double_eq(2.06868, points_array.points[2].x);
  ck_assert_double_eq(1, eq_double(-1.39998, points_array.points[2].y));
  ck_assert_double_eq(1, eq_double(-1.70359, points_array.points[2].z));
  ck_assert_double_eq(2.18278, points_array.points[6].x);
  ck_assert_double_eq(1, eq_double(-1.34112, points_array.points[6].y));
  ck_assert_double_eq(1, eq_double(-1.67165, points_array.points[6].z));
  turn_obj(50, &points_array, 2);
  ck_assert_double_eq(1, eq_double(2.634751, points_array.points[2].x));
  ck_assert_double_eq(1, eq_double(-1.399985, points_array.points[2].y));
  ck_assert_double_eq(1, eq_double(0.48965187, points_array.points[2].z));
  ck_assert_double_eq(1, eq_double(2.683622, points_array.points[6].x));
  ck_assert_double_eq(1, eq_double(-1.341115, points_array.points[6].y));
  ck_assert_double_eq(1, eq_double(0.597590, points_array.points[6].z));
  turn_obj(50, &points_array, 3);
  // printf("%lf %lf %lf\n\n", points_array.points[2].x,
  // points_array.points[2].y, points_array.points[2].z); printf("%lf %lf
  // %lf\n", points_array.points[6].x, points_array.points[6].y,
  // points_array.points[6].z);
  ck_assert_double_eq(1, eq_double(0.621135, points_array.points[2].x));
  ck_assert_double_eq(1, eq_double(-2.918229, points_array.points[2].y));
  ck_assert_double_eq(1, eq_double(0.489651, points_array.points[2].z));
  ck_assert_double_eq(1, eq_double(0.697645, points_array.points[6].x));
  ck_assert_double_eq(1, eq_double(-2.917826, points_array.points[6].y));
  ck_assert_double_eq(1, eq_double(0.597590, points_array.points[6].z));
  free(points_array.points);
  free_matrix_int(&polygons);
}
END_TEST

// -------------------------------------------------------

START_TEST(s21_move_obj) {
  points_a points_array;
  point_t point;
  point.x = 3;
  point.y = 5;
  point.z = -4;
  matrix_poligon polygons;
  pars_file("./cow.obj", &points_array, &polygons);
  move_obj(&point, &points_array);
  ck_assert_double_eq(1, eq_double(5.068680, points_array.points[2].x));
  ck_assert_double_eq(1, eq_double(5.405136, points_array.points[2].y));
  ck_assert_double_eq(1, eq_double(-6.167500, points_array.points[2].z));
  ck_assert_double_eq(1, eq_double(5.182780, points_array.points[6].x));
  ck_assert_double_eq(1, eq_double(5.418506, points_array.points[6].y));
  ck_assert_double_eq(1, eq_double(-6.101870, points_array.points[6].z));
  free(points_array.points);
  free_matrix_int(&polygons);
}
END_TEST

// -------------------------------------------------------

START_TEST(s21_affine_transformation) {
  points_a points_array;
  matrix_poligon polygons;
  pars_file("./cow.obj", &points_array, &polygons);
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
  matrix_move.matrix[0][3] = 3;
  matrix_move.matrix[1][3] = 5;
  matrix_move.matrix[2][3] = -4;
  affine_transformation(&points_array, matrix_move);
  ck_assert_double_eq(1, eq_double(5.068680, points_array.points[2].x));
  ck_assert_double_eq(1, eq_double(5.405136, points_array.points[2].y));
  ck_assert_double_eq(1, eq_double(-6.167500, points_array.points[2].z));
  ck_assert_double_eq(1, eq_double(5.182780, points_array.points[6].x));
  ck_assert_double_eq(1, eq_double(5.418506, points_array.points[6].y));
  ck_assert_double_eq(1, eq_double(-6.101870, points_array.points[6].z));
  free(points_array.points);
  free_matrix_int(&polygons);
  remove_matrix(&matrix_move);
}
END_TEST

// -------------------------------------------------------

START_TEST(s21_for_single_test) {
  // TODO: test for degug
}
END_TEST

// -------------------------------------------------------

void help_functions_test(TCase *tc1_1) {
  tcase_add_test(tc1_1, s21_create_matrix);
  tcase_add_test(tc1_1, s21_str_replace);
  tcase_add_test(tc1_1, s21_count_poligon);
  tcase_add_test(tc1_1, s21_pars_file);
  tcase_add_test(tc1_1, s21_mult_matrix);
  tcase_add_test(tc1_1, s21_scale_obj);
  tcase_add_test(tc1_1, s21_turn_obj);
  tcase_add_test(tc1_1, s21_move_obj);
  tcase_add_test(tc1_1, s21_affine_transformation);
}

void for_single_test(TCase *tc1_1) {
  tcase_add_test(tc1_1, s21_for_single_test);
}

// -------------------------------------------------------

int main(void) {
  Suite *s1 = suite_create("Core");
  TCase *tc1_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);

  help_functions_test(tc1_1);

  for_single_test(tc1_1);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}

// int main(void) {
//   points_a points_array;
//   matrix_poligon polygons;
//   pars_file("./test_obj.obj", &points_array, &polygons);
//   print_pol(&polygons);
//   return 0;
// }
