#ifndef LOGGING_MANAGER_H
#define LOGGING_MANAGER_H

#define ft_unuased(data) (void)(data)

typedef enum s_error_state {
    ERROR_OK = 0,
    ERROR_FILE_NOT_EXISTS,
    ERROR_DYNAMIC_MEMORY,
    ERROR_SEGMENTATION,
    ERROR_ANOTHER,
    ERROR_END,
} s_error_state;

// -------------------------------------------------------

int logging(s_error_state state_, char const *message_, int is_on_);
int logging_line(s_error_state state_, char const *filename_, unsigned line_,
                 char const *message_, int is_on_);
char *create_str(unsigned n_str);
int free_str(char **str_);

#endif // LOGGING_MANAGER_H
