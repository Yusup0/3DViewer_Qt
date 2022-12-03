#include "s21_read_object.h"
#include "logging_manager.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_DEFAULT 11

void print_polygons2(s_data *data_) {
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

// -------------------------------------------------------

void clear_pointers(s_pointers *pointers_) {
  pointers_->v = 0;
  pointers_->vn = 0;
  pointers_->vt = 0;
}

// -------------------------------------------------------

int is_digit(char ch_) {
  int is_dig = 0;
  if (ch_ >= '0' && ch_ <= '9')
    is_dig = 1;

  return (is_dig);
}

// -------------------------------------------------------

void clear_str(char *str_, unsigned const size) {
  if (*str_) {
    for (unsigned i = 0; i < size; ++i) {
      *(str_ + i) = '\0';
    }
  }
}

// -------------------------------------------------------

int free_vertex(s_vertex **vertex_) {
  int is_error = 1;

  if (vertex_ && *vertex_) {
    if ((*vertex_)->array) {
      free((*vertex_)->array);
      (*vertex_)->array = NULL;
      (*vertex_)->size = 0;
    }

    free((*vertex_));
    (*vertex_) = NULL;
    is_error = 0;
  }

  return (is_error);
}

// -------------------------------------------------------

int free_polygons(s_polygon **polygons_) {
  int is_error = 1;

  if (polygons_ && *polygons_) {
    if ((*polygons_)->p_polygon) {
      free((*polygons_)->p_polygon);
      (*polygons_)->p_polygon = NULL;
      (*polygons_)->size = 0;
    }

    free((*polygons_));
    (*polygons_) = NULL;
    is_error = 0;
  }

  return (is_error);
}

// -------------------------------------------------------

int free_data(s_data **data_) {
  int is_error = 1;

  if (data_ && *data_) {
    free_polygons(&(*data_)->polygon);
    free_vertex(&(*data_)->vertex);

    is_error = 0;

    free(*data_);
  }

  return (is_error);
}

// -------------------------------------------------------

int add_polygon(s_data **data_, s_pointers coordinates_) {
  int is_error = 1;
  s_pointers *tmp_polygons = NULL;

  if (*data_ == NULL) {
    if (!(*data_ = (s_data *)calloc(1, sizeof(s_data)))) {
      logging_line(ERROR_DYNAMIC_MEMORY, __FILE__, __LINE__, "MEMORY ERROR", 1);
      exit(EXIT_FAILURE);
    }
    (*data_)->polygon = NULL;
  }

  if ((*data_)->polygon) {
    if (((*data_)->polygon)->p_polygon == NULL) {
      ((*data_)->polygon)->limit = 40;
      ((*data_)->polygon)->size = 0;

      if (!(((*data_)->polygon)->p_polygon = (s_pointers *)calloc(
                ((*data_)->polygon)->limit + 2, sizeof(s_pointers)))) {
        logging_line(ERROR_DYNAMIC_MEMORY, __FILE__, __LINE__, "MEMORY ERROR",
                     1);
        exit(EXIT_FAILURE);
      }
    }
    if (((*data_)->polygon)->size >= ((*data_)->polygon)->limit) {

      ((*data_)->polygon)->limit *= 2;

      if (!(tmp_polygons = (s_pointers *)realloc(
                ((*data_)->polygon)->p_polygon,
                (((*data_)->polygon)->limit + 2) * sizeof(s_pointers)))) {
        logging_line(ERROR_DYNAMIC_MEMORY, __FILE__, __LINE__, "MEMORY ERROR",
                     1);
        free_polygons(&(*data_)->polygon);
        exit(EXIT_FAILURE);
      }

      ((*data_)->polygon)->p_polygon = tmp_polygons;
    }

    ((*data_)->polygon)->p_polygon[((*data_)->polygon)->size] = coordinates_;
    ++(((*data_)->polygon)->size);

  } else if ((*data_)->polygon == NULL) {
    if (!((*data_)->polygon = (s_polygon *)calloc(1, sizeof(s_polygon)))) {
      logging_line(ERROR_DYNAMIC_MEMORY, __FILE__, __LINE__, "MEMORY ERROR", 1);
      exit(EXIT_FAILURE);
    }

    ((*data_)->polygon)->limit = 40;
    ((*data_)->polygon)->size = 0;

    if (!(((*data_)->polygon)->p_polygon = (s_pointers *)calloc(
              ((*data_)->polygon)->limit + 2, sizeof(s_pointers)))) {
      logging_line(ERROR_DYNAMIC_MEMORY, __FILE__, __LINE__, "MEMORY ERROR", 1);
      exit(EXIT_FAILURE);
    }

    ((*data_)->polygon)->p_polygon[((*data_)->polygon)->size] = coordinates_;
    ++(((*data_)->polygon)->size);
  }

  return (is_error);
}

// -------------------------------------------------------

int add_vertex(s_data **data_, float num_) {
  int is_error = 1;
  float *tmp_array = NULL;

  if (*data_ == NULL) {
    if (!(*data_ = (s_data *)calloc(1, sizeof(s_data)))) {
      logging_line(ERROR_DYNAMIC_MEMORY, __FILE__, __LINE__, "MEMORY ERROR", 1);
      exit(EXIT_FAILURE);
    }
    (*data_)->vertex = NULL;
  }

  if ((*data_)->vertex) {
    if (((*data_)->vertex)->array == NULL) {
      ((*data_)->vertex)->limit = 40;
      ((*data_)->vertex)->size = 0;

      if (!(((*data_)->vertex)->array = (float *)calloc(
                ((*data_)->vertex)->limit + 2, sizeof(float)))) {
        logging_line(ERROR_DYNAMIC_MEMORY, __FILE__, __LINE__, "MEMORY ERROR",
                     1);
        exit(EXIT_FAILURE);
      }
    }
    if (((*data_)->vertex)->size >= ((*data_)->vertex)->limit) {

      ((*data_)->vertex)->limit *= 2;

      if (!(tmp_array = (float *)realloc(((*data_)->vertex)->array,
                                         (((*data_)->vertex)->limit + 2) *
                                             sizeof(float)))) {
        logging_line(ERROR_DYNAMIC_MEMORY, __FILE__, __LINE__, "MEMORY ERROR",
                     1);
        free_vertex(&(*data_)->vertex);
        exit(EXIT_FAILURE);
      }

      ((*data_)->vertex)->array = tmp_array;
    }

    ((*data_)->vertex)->array[((*data_)->vertex)->size] = num_;
    ++(((*data_)->vertex)->size);

  } else if ((*data_)->vertex == NULL) {
    if (!((*data_)->vertex = (s_vertex *)calloc(1, sizeof(s_vertex)))) {
      logging_line(ERROR_DYNAMIC_MEMORY, __FILE__, __LINE__, "MEMORY ERROR", 1);
      exit(EXIT_FAILURE);
    }

    ((*data_)->vertex)->limit = 40;
    ((*data_)->vertex)->size = 0;

    if (!(((*data_)->vertex)->array =
              (float *)calloc(((*data_)->vertex)->limit + 2, sizeof(float)))) {
      logging_line(ERROR_DYNAMIC_MEMORY, __FILE__, __LINE__, "MEMORY ERROR", 1);
      exit(EXIT_FAILURE);
    }

    ((*data_)->vertex)->array[((*data_)->vertex)->size] = num_;
    ++(((*data_)->vertex)->size);
  }

  return (is_error);
}

// -------------------------------------------------------

s_data *read_file(char const *filename_) {
  s_data *p_data = NULL;

  FILE *fin = NULL;

  fin = fopen(filename_, "r");

  if (fin) {
    // int res = 1;

    int read;
    char *line = NULL;
    size_t len = 0;
    s_pointers pointers_first = {0, 0, 0};

    while ((read = getline(&line, &len, fin)) != -1) {
      if (len > 0) {
        if (line[0] == '#') {

        } else if (line[0] == 'f' && line[1] == ' ') {
            pointers_first = string_to_polygons(line + 1, &p_data);
            add_polygon(&p_data, pointers_first);
        } else if (line[0] == 'v' && line[1] == ' ') {
          float v, t, n = 0;
          sscanf(line + 1, "%f%f%f", &v, &t, &n);
          add_vertex(&p_data, v);
          add_vertex(&p_data, t);
          add_vertex(&p_data, n);
        } else if (line[0] == 'v' && line[1] == 'n') {
        } else if (line[0] == 'v' && line[1] == 'n') {
        }
      }
    }

    free(line);
    fclose(fin);
  } else {
    logging_line(ERROR_FILE_NOT_EXISTS, filename_, __LINE__,
                 "FILE DIDN'T read ", 1);
  }

  return (p_data);
}

// -------------------------------------------------------

void parse_file_object(char *filename_, s_info *data_) {
  ft_unuased(filename_);
  ft_unuased(data_);
}

// -------------------------------------------------------

void chapter_polygon(s_pointers *pointer, char const *tmp_, int num_) {
  if (pointer) {
    if (num_ == 1) {
      pointer->v = atof(tmp_);
      if (pointer->v > 0)
          --pointer->v;
    } else if (num_ == 2) {
      pointer->vt = atof(tmp_);
      if (pointer->vt > 0)
          --pointer->vt;
    } else if (num_ == 3) {
      pointer->vn = atof(tmp_);
      if (pointer->vn > 0)
          --pointer->vn;
    }
  }
}

// -------------------------------------------------------

s_pointers string_to_polygons(char const *str_, s_data **data_) {
    s_pointers pointers_before = {0, 0, 0};
    s_pointers pointers_first = {0, 0, 0};

  if (str_) {
    unsigned n_str = strlen(str_);
    unsigned const limit = 256;
    char tmp[limit];
    clear_str(tmp, limit);
    int n_chapter = 0;
    int is_first = 0;

    s_pointers pointers = {0, 0, 0};

    int i_tmp = 0;
    int n_num = 0;
    tmp[0] = 0;

    while (*str_ == 30 || *str_ == 31 || *str_ == 32)
      ++(str_);

    for (unsigned i = 0; i < n_str; ++i) {
      if (str_[i] == ' ') {
        ++i;
      }

      if (is_digit(str_[i]) || str_[i] == '.') {
        while (is_digit(str_[i]) || str_[i] == '.') {
          tmp[i_tmp++] = str_[i++];
        }
        ++n_num;
      } else if (str_[i] == '/') {
        ++n_num;
      }

      if (str_[i] == ' ') {
        chapter_polygon(&pointers, tmp, n_num);
        n_num = 3;
      } else {
        chapter_polygon(&pointers, tmp, n_num);
      }

      if (n_num == 3) {
        pointers_before = pointers;
        ++n_chapter;
        if (n_chapter > 1) {
          add_polygon(data_, pointers_before);
          clear_pointers(&pointers_before);
        }
        if (!is_first) {
            pointers_first = pointers_before;
            is_first = 1;
        }
        add_polygon(data_, pointers);
        clear_pointers(&pointers);
        n_num = 0;
      }

      if (n_chapter == 3) {
      }

      clear_str(tmp, limit);
      tmp[0] = 0;
      i_tmp = 0;
    }
  }

  return (pointers_first);
}
