CC=gcc
CFLAGS=-g -Wall -Wextra -Werror  # -std=c11  # -pedantic 
GCOVFLAGS:=-fprofile-arcs -ftest-coverage   -fsanitize=address
LDFLAGS:=-lcheck --coverage -fsanitize=address

ifeq ($(shell uname), Linux)
LDFLAGS +=-pthread -lgcov -lcheck -fsanitize=address -lrt -lm # -lsubunit -lrt -lm 
endif

SOURCES:= functional.c logging_manager.c

OBJECTS = $(addprefix objects/,$(SOURCES:.c=.o))
OBJECTS_GCOV = $(addprefix gcov_obj/,$(SOURCES:.c=.o))

.PHONY: all clean rebuild lint

all: functional.a 

build: test gcov_report functional_gcov.a 

functional_gcov.a: $(OBJECTS_GCOV)
	@echo "!!! -- !!! functional_gcov" $@ " ---- " ${OBJECTS_GCOV}
	ar -src $@ $(OBJECTS_GCOV)

functional.a: $(OBJECTS)
	ar -src $@ $(OBJECTS)

test: functional_gcov.a gcov_obj/functional_test.o
	# $(CC) gcov_obj/s21_read_object_test.o gcov_obj/s21_functional_test.o  functional_gcov.a -g -o $@ $(LDFLAGS) $(GCOVFLAGS)
	$(CC) gcov_obj/functional.o  gcov_obj/functional_test.o functional_gcov.a -g -o $@ $(LDFLAGS) $(GCOVFLAGS)
	./test

gcov_report: rebuild test
	lcov -c -d gcov_obj/. -o gcov_obj/coverage.info
	genhtml gcov_obj/coverage.info --output-directory gcov_report

gcov_obj/%.o: %.c
	@echo "!!! gcov_obj"
	$(CC) $(CFLAGS) $(GCOVFLAGS) -c $< -o $@

objects/%.o: %.c
	@echo "!!! objects"
	$(CC) -g -Wall -Werror -Wextra  -c $< -o $@


rebuild: clean all

clean:
	rm -f */*.o
	rm -f report
	rm -f gcov_obj/*.gc*
	rm -f ./test
	rm -f -r gcov_report/*
	rm -f -r ./*gcno
	rm -f gcov_obj/*.info
	rm -f .DS_Store
	rm -f *.a
	rm -f ./a.out

check:
	clang_format -n ./*.c

