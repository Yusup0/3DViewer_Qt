#include "../logging_manager.h"
#include "../s21_read_object.h"
#include <check.h>
#include <math.h>
#include <stdio.h>

unsigned len_num(unsigned num_);
int logging_line(s_error_state state_, char const *filename_, unsigned line_,
                 char const *message_, int is_on_);
int str_realloc_add_num(char **str_, unsigned num_, unsigned *limit_str_);
int str_realloc_cat(char **str_, char const *text_, unsigned *limit_str_);

int eq_double(double a_, double b_) { return (fabs(a_ - b_) < 1e-5); };

/* **** ***** **** */

int add_polygon(s_data **data_, s_pointers coordinates_);
int add_vertex(s_data **data_, float num_);
s_pointers string_to_polygons(char const *str_, s_data **data_);

void print_polygons(s_data *data_) {
  if (data_) {

    unsigned n_polygon = data_->polygon->size;
    for (unsigned i = 0; i < n_polygon; ++i) {
      if (i % 3 == 0 && i != 0)
        printf("\n");
      printf("v %d ", (int)data_->polygon->p_polygon[i].v);
    }
    printf("\n");
    for (unsigned i = 0; i < n_polygon; ++i) {
      if (i % 3 == 0 && i != 0)
        printf("\n");
      printf("vt %d ", (int)data_->polygon->p_polygon[i].vt);
    }
    printf("\n");
    for (unsigned i = 0; i < n_polygon; ++i) {
      if (i % 3 == 0 && i != 0)
        printf("\n");
      printf("vn %d ", (int)data_->polygon->p_polygon[i].vn);
    }
    printf("\n");
  }
}

/* **** ***** **** */
START_TEST(s21_for_single_test) {
  s_data *data = NULL;
  char const *one = "1//2 7//2 5//2";

  string_to_polygons(one, &data);
  // print_polygons(data);

  ck_assert_int_eq(data->polygon->p_polygon[0].v, 0);
  ck_assert_int_eq((data->polygon->p_polygon[0].vt), 0);
  ck_assert_int_eq((data->polygon->p_polygon[0].vn), 1);

  ck_assert_int_eq((data->polygon->p_polygon[1].v), 6);
  ck_assert_int_eq((data->polygon->p_polygon[1].vt), 0);
  ck_assert_int_eq((data->polygon->p_polygon[1].vn), 1);

  ck_assert_int_eq((data->polygon->p_polygon[2].v), 6);
  ck_assert_int_eq((data->polygon->p_polygon[2].vt), 0);
  ck_assert_int_eq((data->polygon->p_polygon[2].vn), 1);

  ck_assert_int_eq((data->polygon->p_polygon[3].v), 4);
  ck_assert_int_eq((data->polygon->p_polygon[3].vt), 0);
  ck_assert_int_eq((data->polygon->p_polygon[3].vn), 1);

  ck_assert_int_eq((data->polygon->p_polygon[4].v), 4);
  ck_assert_int_eq((data->polygon->p_polygon[4].vt), 0);
  ck_assert_int_eq((data->polygon->p_polygon[4].vn), 1);

  free_data(&data);
}
END_TEST;

START_TEST(s21_string_to_polygon_test) {

  // f  1//2  7//2  5//2
  // f  1//2  3//2  7//2
  // f  1//6  4//6  3//6
  // f  1//6  2//6  4//6
  // f  3//3  8//3  7//3
  // f  3//3  4//3  8//3
  // f  5//5  7//5  8//5
  // f  5//5  8//5  6//5
  // f  1//4  5//4  6//4
  // f  1//4  6//4  2//4
  // f  2//1  6//1  8//1
  // f  2//1  8//1  4//1

  s_data *data = NULL;
  char const *one = "1//2 7//2 5//2";
  char const *two = "1//2 3//2 7//2";
  char const *three = "11.94 3.82 8.88 ";
  char const *four = "11.94/3.28/9.92 3.82/4.82/94.2 8.88/0.1/1 ";
  char const *five = "11.94//9.92 3.82//94.2 8.88//1 ";

  string_to_polygons(one, &data);
  string_to_polygons(two, &data);
  string_to_polygons(three, &data);
  string_to_polygons(four, &data);
  string_to_polygons(five, &data);

  ck_assert_int_eq(eq_double(data->polygon->p_polygon[0].v, 0), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[0].vt, 0), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[0].vn, 2), 1);

  ck_assert_int_eq(eq_double(data->polygon->p_polygon[1].v, 6), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[1].vt, 0), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[1].vn, 2), 1);

  ck_assert_int_eq(eq_double(data->polygon->p_polygon[2].v, 4), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[2].vt, 0), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[2].vn, 2), 1);

  ck_assert_int_eq(eq_double(data->polygon->p_polygon[3].v, 0), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[3].vt, 0), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[3].vn, 2), 1);

  ck_assert_int_eq(eq_double(data->polygon->p_polygon[4].v, 2), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[4].vt, 0), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[4].vn, 2), 1);

  ck_assert_int_eq(eq_double(data->polygon->p_polygon[5].v, 6), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[5].vt, 0), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[5].vn, 2), 1);

  ck_assert_int_eq(eq_double(data->polygon->p_polygon[6].v, 10.94), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[6].vt, 0), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[6].vn, 0), 1);

  ck_assert_int_eq(eq_double(data->polygon->p_polygon[7].v, 2.82), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[7].vt, 0), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[7].vn, 0), 1);

  ck_assert_int_eq(eq_double(data->polygon->p_polygon[8].v, 7.88), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[8].vt, 0), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[8].vn, 0), 1);

  ck_assert_int_eq(eq_double(data->polygon->p_polygon[9].v, 10.94), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[9].vt, 3.28), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[9].vn, 9.92), 1);

  ck_assert_int_eq(eq_double(data->polygon->p_polygon[10].v, 2.82), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[10].vt, 4.82), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[10].vn, 94.2), 1);

  ck_assert_int_eq(eq_double(data->polygon->p_polygon[11].v, 7.88), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[11].vt, 0.1), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[11].vn, 1), 1);

  ck_assert_int_eq(eq_double(data->polygon->p_polygon[12].v, 10.94), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[12].vt, 0), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[12].vn, 9.92), 1);

  ck_assert_int_eq(eq_double(data->polygon->p_polygon[13].v, 2.82), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[13].vt, 0), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[13].vn, 94.2), 1);

  ck_assert_int_eq(eq_double(data->polygon->p_polygon[14].v, 7.88), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[14].vt, 0), 1);
  ck_assert_int_eq(eq_double(data->polygon->p_polygon[14].vn, 1), 1);

  free_data(&data);
}
END_TEST;

/* **** ***** **** */

START_TEST(s21_vertex_test) {
  unsigned limit = 40;
  s_data *data = NULL;

  add_vertex(&data, 10.9);
  ck_assert_int_eq(eq_double(data->vertex->array[0], 10.9), 1);

  add_vertex(&data, 9.8);
  ck_assert_int_eq(eq_double(data->vertex->array[1], 9.8), 1);

  add_vertex(&data, 8.7);
  ck_assert_int_eq(eq_double(data->vertex->array[2], 8.7), 1);

  add_vertex(&data, 7.6);
  ck_assert_int_eq(eq_double(data->vertex->array[3], 7.6), 1);

  ck_assert_int_eq(data->vertex->limit, limit);
  ck_assert_int_eq(data->vertex->size, 4);

  for (int i = 0; i < 100; ++i) {
    add_vertex(&data, 19.88);
    ck_assert_int_eq(data->vertex->size, i + 5);
  }

  free_data(&data);
}
END_TEST;

/* **** ***** **** */

START_TEST(s21_vertex2_test) {
  unsigned limit = 40;
  s_data *data = NULL;

  add_vertex(&data, 10.9);
  ck_assert_int_eq(eq_double(data->vertex->array[0], 10.9), 1);

  add_vertex(&data, 9.8);
  ck_assert_int_eq(eq_double(data->vertex->array[1], 9.8), 1);

  add_vertex(&data, 8.7);
  ck_assert_int_eq(eq_double(data->vertex->array[2], 8.7), 1);

  add_vertex(&data, 7.6);
  ck_assert_int_eq(eq_double(data->vertex->array[3], 7.6), 1);

  ck_assert_int_eq(data->vertex->limit, limit);
  ck_assert_int_eq(data->vertex->size, 4);

  for (int i = 0; i < 100; ++i) {
    add_vertex(&data, 19.88);
    ck_assert_int_eq(data->vertex->size, i + 5);
  }

  free_data(&data);
}
END_TEST;

// -------------------------------------------------------
START_TEST(s21_read_file2_test) {
  s_data *data = read_file("./gcov_obj/test2_obj.obj");

  float vertex[] = {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0,
                    1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0};

  int vertex_points[] = {0, 6, 6, 4, 4, 0, 0, 2, 2, 6, 6, 0, 0, 3, 3, 2, 2, 0,
                         0, 1, 1, 3, 3, 0, 2, 7, 7, 6, 6, 2, 2, 3, 3, 7, 7, 2,
                         4, 6, 6, 7, 7, 4, 4, 7, 7, 5, 5, 4, 0, 4, 4, 5, 5, 0,
                         0, 5, 5, 1, 1, 0, 1, 5, 5, 7, 7, 1, 1, 7, 7, 3, 3, 1};

  unsigned n_coord = data->vertex->size;
  unsigned n_vertex_points = data->polygon->size;

  ck_assert_int_eq(n_coord, 24);
  // ck_assert_int_eq(n_vertex_points, 72);
  ft_unuased(n_vertex_points);
  print_polygons(data);

  ck_assert_uint_eq(data->polygon->p_polygon[0].v, 0);
  ck_assert_uint_eq(data->polygon->p_polygon[1].v, 6);
  ck_assert_uint_eq(data->polygon->p_polygon[2].v, 6);
  ck_assert_uint_eq(data->polygon->p_polygon[3].v, 4);
  ck_assert_uint_eq(data->polygon->p_polygon[4].v, 4);
  ck_assert_uint_eq(data->polygon->p_polygon[5].v, 0);
  ck_assert_uint_eq(data->polygon->p_polygon[6].v, 0);
  ck_assert_uint_eq(data->polygon->p_polygon[7].v, 2);
  ck_assert_uint_eq(data->polygon->p_polygon[8].v, 2);
  ck_assert_uint_eq(data->polygon->p_polygon[9].v, 6);
  ck_assert_uint_eq(data->polygon->p_polygon[10].v, 6);
  ck_assert_uint_eq(data->polygon->p_polygon[11].v, 0);
  ck_assert_uint_eq(data->polygon->p_polygon[12].v, 0);
  ck_assert_uint_eq(data->polygon->p_polygon[13].v, 3);
  ck_assert_uint_eq(data->polygon->p_polygon[14].v, 3);
  ck_assert_uint_eq(data->polygon->p_polygon[15].v, 2);
  ck_assert_uint_eq(data->polygon->p_polygon[16].v, 2);
  ck_assert_uint_eq(data->polygon->p_polygon[17].v, 0);
  ck_assert_uint_eq(data->polygon->p_polygon[18].v, 0);
  ck_assert_uint_eq(data->polygon->p_polygon[19].v, 1);
  ck_assert_uint_eq(data->polygon->p_polygon[20].v, 1);
  ck_assert_uint_eq(data->polygon->p_polygon[21].v, 3);
  ck_assert_uint_eq(data->polygon->p_polygon[22].v, 3);
  ck_assert_uint_eq(data->polygon->p_polygon[23].v, 0);
  ck_assert_uint_eq(data->polygon->p_polygon[24].v, 2);
  ck_assert_uint_eq(data->polygon->p_polygon[25].v, 7);
  ck_assert_uint_eq(data->polygon->p_polygon[26].v, 7);
  ck_assert_uint_eq(data->polygon->p_polygon[27].v, 6);
  ck_assert_uint_eq(data->polygon->p_polygon[28].v, 6);
  ck_assert_uint_eq(data->polygon->p_polygon[29].v, 2);

  // for (unsigned i = 0; i < n_coord; ++i) {
  //     ck_assert_int_eq(eq_double(data->vertex->array[i], vertex[i]), 1);
  // }

  // for (unsigned i = 0; i < n_vertex_points; ++i) {
  //     ck_assert_int_eq((int)data->polygon->p_polygon[i].v,
  //     vertex_points2[i]);
  // }

  ft_unuased(vertex);
  ft_unuased(vertex_points);

  free_data(&data);
}
END_TEST

START_TEST(s21_read_file_test) {
  s_data *data = read_file("./gcov_obj/test_obj.obj");

  float vertex[] = {0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0,
                    1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0};

  int vertex_points[] = {0, 6, 6, 4, 4, 0, 0, 2, 2, 6, 6, 0, 0, 3, 3, 2, 2, 0,
                         0, 1, 1, 3, 3, 0, 2, 7, 7, 6, 6, 2, 2, 3, 3, 7, 7, 2,
                         4, 6, 6, 7, 7, 4, 4, 7, 7, 5, 5, 4, 0, 4, 4, 5, 5, 0,
                         0, 5, 5, 1, 1, 0, 1, 5, 5, 7, 7, 1, 1, 7, 7, 3, 3, 1};

  unsigned n_coord = data->vertex->size;
  unsigned n_vertex_points = data->polygon->size;

  ck_assert_int_eq(n_coord, 24);
  ck_assert_int_eq(n_vertex_points, 72);

  for (unsigned i = 0; i < n_coord; ++i) {
    ck_assert_int_eq(eq_double(data->vertex->array[i], vertex[i]), 1);
  }

  for (unsigned i = 0; i < n_vertex_points; ++i) {
    ck_assert_int_eq((int)data->polygon->p_polygon[i].v, vertex_points[i]);
  }

  ft_unuased(vertex);
  ft_unuased(vertex_points);

  free_data(&data);
}
END_TEST

// -------------------------------------------------------

START_TEST(s21_add_polygon_test) {
  unsigned limit = 40;
  s_data *data = NULL;

  s_pointers coord = {3.8, 8, 4.3};
  s_pointers coord2 = {39.1, 81, 49.3};
  s_pointers coord3 = {399, 8.1, 849.3};

  add_polygon(&data, coord);

  ck_assert_int_eq(data->polygon->limit, limit);
  ck_assert_int_eq(data->polygon->size, 1);

  ck_assert_float_eq(data->polygon->p_polygon->v, 3.8);
  ck_assert_float_eq(data->polygon->p_polygon->vt, 8);
  ck_assert_float_eq(data->polygon->p_polygon->vn, 4.3);

  add_polygon(&data, coord2);
  ck_assert_int_eq(data->polygon->size, 2);

  ck_assert_float_eq(data->polygon->p_polygon[1].v, 39.1);
  ck_assert_float_eq(data->polygon->p_polygon[1].vt, 81);
  ck_assert_float_eq(data->polygon->p_polygon[1].vn, 49.3);

  add_polygon(&data, coord3);
  ck_assert_int_eq(data->polygon->size, 3);

  ck_assert_float_eq(data->polygon->p_polygon[2].v, 399);
  ck_assert_float_eq(data->polygon->p_polygon[2].vt, 8.1);
  ck_assert_float_eq(data->polygon->p_polygon[2].vn, 849.3);

  for (int i = 0; i < 100; ++i) {
    add_polygon(&data, coord3);
    ck_assert_int_eq(data->polygon->size, i + 4);
  }

  free_data(&data);
  // free_polygons(&data);
}
END_TEST

// -------------------------------------------------------

START_TEST(s21_string_test) {
  unsigned n_str = 10;
  char *str = create_str(n_str);

  str_realloc_cat(&str, "there is no ", &n_str);
  ck_assert_str_eq(str, "there is no ");

  str_realloc_cat(&str, "spoon! <--->", &n_str);
  ck_assert_str_eq(str, "there is no spoon! <--->");

  int res = free_str(&str);

  ck_assert_int_eq(res, 0);
}
END_TEST

// -------------------------------------------------------

START_TEST(s21_len_num) {
  unsigned num = 150;
  unsigned res = len_num(num);

  ck_assert_int_eq(res, 3);

  num = 15;
  res = len_num(num);
  ck_assert_int_eq(res, 2);

  num = 5;
  res = len_num(num);
  ck_assert_int_eq(res, 1);

  num = 0;
  res = len_num(num);
  ck_assert_int_eq(res, 1);

  num = 3494839480;
  res = len_num(num);
  ck_assert_int_eq(res, 10);
}
END_TEST

// -------------------------------------------------------

START_TEST(s21_add_num) {
  unsigned n_str = 10;

  char *str = create_str(n_str);

  str_realloc_cat(&str, "there ", &n_str);
  str_realloc_add_num(&str, 15, &n_str);
  str_realloc_cat(&str, " no ", &n_str);
  str_realloc_add_num(&str, 5800, &n_str);
  str_realloc_cat(&str, "n.", &n_str);

  ck_assert_str_eq(str, "there 15 no 5800n.");
  free_str(&str);
}
END_TEST

// -------------------------------------------------------

START_TEST(s21_error_test) {
  int res = logging(ERROR_OK, "", 0);
  ck_assert_int_eq(res, 1);

  res = logging(ERROR_OK, "", 1);
  ck_assert_int_eq(res, 1);

  res = logging(ERROR_OK, "Testing message", 1);
  ck_assert_int_eq(res, 1);

  res = logging_line(ERROR_FILE_NOT_EXISTS, "testing.txt", __LINE__, "", 1);
  ck_assert_int_eq(res, 1);
}
END_TEST

// -------------------------------------------------------

void logging_test(TCase *tc1_1) {
  tcase_add_test(tc1_1, s21_string_test);
  tcase_add_test(tc1_1, s21_error_test);
}

// -------------------------------------------------------

void read_object_test(TCase *tc1_1) {
  tcase_add_test(tc1_1, s21_add_polygon_test);
  tcase_add_test(tc1_1, s21_vertex_test);
  tcase_add_test(tc1_1, s21_vertex2_test);

  tcase_add_test(tc1_1, s21_string_test);
  tcase_add_test(tc1_1, s21_len_num);
  tcase_add_test(tc1_1, s21_add_num);

  tcase_add_test(tc1_1, s21_string_to_polygon_test);

    tcase_add_test(tc1_1, s21_read_file_test); 
  tcase_add_test(tc1_1, s21_for_single_test);
}
// -------------------------------------------------------

void single_test(TCase *tc1_1) { 
  tcase_add_test(tc1_1, s21_read_file2_test);
}

// -------------------------------------------------------

int main(void) {
  Suite *s1 = suite_create("Core");
  TCase *tc1_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);

  // logging_test(tc1_1);
  single_test(tc1_1);
  // read_object_test(tc1_1);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
