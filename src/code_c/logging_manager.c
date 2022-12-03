#include "logging_manager.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *create_str(unsigned n_str) {
  char *str = NULL;
  if (!(str = (char *)calloc(n_str + 1, sizeof(char)))) {
    exit(EXIT_FAILURE);
  }

  return (str);
}

// -------------------------------------------------------

int free_str(char **str_) {
  int is_error = 1;
  if (str_ && *str_) {
    free(*str_);
    *str_ = NULL;
    is_error = 0;
  }

  return (is_error);
}

// -------------------------------------------------------

unsigned len_num(unsigned num_) {
  unsigned i = 1;

  while ((num_ /= 10) > 0) {
    ++i;
  }

  return (i);
}

// -------------------------------------------------------

int str_realloc_add_num(char **str_, unsigned num_, unsigned *limit_str_) {
  int is_error = 1;
  if (str_ && *str_) {
    unsigned n_num = len_num(num_);
    unsigned n_str = strlen(*str_);
    char *result_string = NULL;

    if (n_num + n_str >= *limit_str_) {
      *limit_str_ *= 2;
      if (!((result_string) = (char *)realloc((*str_), *limit_str_ + 1))) {
        free_str(str_);
        exit(EXIT_FAILURE);
      }

      *str_ = result_string;
      sprintf(*str_, "%s%d", *str_, num_);
    } else {
      sprintf(*str_, "%s%d", *str_, num_);
    }
    is_error = 0;
  }

  return (is_error);
}

// -------------------------------------------------------

int str_realloc_cat(char **str_, char const *text_, unsigned *limit_str_) {
  int is_error = 1;
  if (str_ && *str_ && text_) {
    unsigned n_text = strlen(text_);
    unsigned n_str = strlen(*str_);
    char *result_string = NULL;

    if (n_text + n_str >= *limit_str_) {
      *limit_str_ *= 2;
      if (!((result_string) = (char *)realloc((*str_), *limit_str_ + 1))) {
        free_str(str_);
        exit(EXIT_FAILURE);
      }
      *str_ = result_string;

      strncat(*str_, text_, *limit_str_);
    } else {
      strncat(*str_, text_, *limit_str_);
    }
    is_error = 0;
  }

  return (is_error);
}

// -------------------------------------------------------

int logging_line(s_error_state state_, char const *filename_, unsigned line_,
                 char const *message_, int is_on_) {
  int is_error = 1;
  unsigned n_message = 100;
  char *message = create_str(n_message);

  if (state_ != ERROR_OK) {
    str_realloc_cat(&message, message_, &n_message);
    str_realloc_cat(&message, "  filename ---> ", &n_message);
    str_realloc_cat(&message, filename_, &n_message);
    str_realloc_cat(&message, " [", &n_message);
    str_realloc_add_num(&message, line_, &n_message);
    str_realloc_cat(&message, "] .", &n_message);
    is_error = logging(state_, message, is_on_);
  } else {
    is_error = logging(state_, message_, is_on_);
  }
  free_str(&message);

  return (is_error);
}

// -------------------------------------------------------

int logging(s_error_state state_, char const *message_, int is_on_) {
  int res = 0;

  if (is_on_) {
    char const *filename = "./log.txt";
    int is_seg = 0;

    FILE *fin = NULL;

    fin = fopen(filename, "a");
    if (fin) {
      res = 1;

      long ttime = time(0);
      char const *current_time = ctime(&ttime);

      if (message_ == NULL || strlen(message_) == 0) {
        switch (state_) {
        case ERROR_OK:
          message_ = "[OK] < ---- > ";
          break;
        case ERROR_FILE_NOT_EXISTS:
          message_ = "[Error] File not exist. ";
          break;
        case ERROR_SEGMENTATION:
          message_ = "[Error] SEGMENTATION fail. ";
          is_seg = 1;
          break;
        case ERROR_END:
          message_ = "[OK] CLOSE APPLICATION";
          is_seg = 1;
          break;
        default:
          message_ = "[Error AnOtHeR]  ";
          is_seg = 1;
          break;
        }
      }

      if (is_seg) {
        fprintf(fin, "%s  ___  %s\n\n", message_, current_time);
      } else {
        fprintf(fin, "%s  ___  %s", message_, current_time);
      }
    }
    fclose(fin);

  } else {
    res = 1;
  }

  return (res);
}
