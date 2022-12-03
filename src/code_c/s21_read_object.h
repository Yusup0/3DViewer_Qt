#ifndef S21_READ_OBJECT
#define S21_READ_OBJECT

#define SIZE_VERTEX 3

#include <stdio.h>

#include "viewer_info.h"

typedef struct s_vertex {
  float *array;
  unsigned size;
  unsigned limit;
  float max;
} s_vertex;

/* **** ***** **** */

typedef struct s_pointers {
  float v;
  float vt;
  float vn;
} s_pointers;

/* **** ***** **** */
typedef struct s_polygon {
  s_pointers *p_polygon;
  unsigned size;
  unsigned limit;
} s_polygon;


/* **** ***** **** */

typedef struct s_data {
  s_vertex *vertex;
  s_polygon *polygon;
} s_data;

/* **** ***** **** */

s_data *read_file(char const *filename_);
int free_polygons(s_polygon **polygons_);
int free_data(s_data **data_);
s_pointers string_to_polygons(char const *str_, s_data **data_);

/* **** ***** **** */

void parse_file_object(char *filename_, s_info *data_);

#endif // S21_READ_OBJECT
