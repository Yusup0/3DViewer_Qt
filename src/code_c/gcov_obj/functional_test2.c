
#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../functional.h"

#define IS_TEST_TURN_ON 0

// void trim_str_num(char *p_res_, unsigned n_limit_);
// int equal_double(double a_, double b_) { return (fabs(a_ - b_) < 1e-7); }
// char *calculate_notation(char const *p_str_);
// void remove_zeros(char *p_str_, unsigned limit_str_, int *p_is_error_);

// -------------------------------------------------------

// char *str_to_postfix(char *str_);
// char *string_number(char const *p_str_, unsigned *pi_str_, int *p_is_error_);
// int add_math_function(char const *p_src_, unsigned *pi_src_, s_stack **p_s_);
// int is_check_valid_postfix(char *p_str_);

// -------------------------------------------------------

START_TEST(s21_create_matrix) {
    matrix_t res;

    create_matrix(3, 3, &res);

    print_matrix(&res);
    remove_matrix(&res);
}
END_TEST

// -------------------------------------------------------


// -------------------------------------------------------


START_TEST(s21_for_single_test) {
    // TODO: test for degug
}
END_TEST

// -------------------------------------------------------

void help_functions_test(TCase *tc1_1) {
  tcase_add_test(tc1_1, s21_create_matrix);
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
