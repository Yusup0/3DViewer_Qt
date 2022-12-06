current_os := $(shell sh -c 'uname -s 2>/dev/null || echo not')

.PHONY: all clean rebuild

all: checkdir install start

clean:
	rm -rf ./BUILD
	rm -f 3d.tar
	rm -rf ./libBin
	rm -rf ./log.txt
	rm -rf ./screenshots./*
	rm -rf ./gifs./*
	cd ./code_c/ && make clean

install:
	cd BUILD && cmake .. && make

uninstall:
	rm -rf ./BUILD
	rm -f 3d.tar
	rm -rf ./libBin

checkdir:
	mkdir -p BUILD

start:
	./BUILD/3DV

dist:
	tar -cf 3d.tar ./*

dvi:
ifeq ($(current_os), Linux)
	vi ./documents/doc.txt
else
	open ./documents/doc.txt
endif

tests:
	cd ./code_c && make test && ./test

gcov_report:
	cd ./code_c/ && make clean && make gcov_report
ifeq ($(current_os), Linux)
	firefox ./code_c/gcov_report/index.html
else
	open ./code_c/gcov_report/index.html
endif

check_format:
	clang-format -n ./lib/3DViewer/includes/*.hpp ./lib/3DViewer/srcs/*.cpp ./lib/QtGifImage/includes/*.h ./lib/QtGifImage/srcs/*.cpp ./code_c/*.c ./code_c/*.h ./code_c/gcov_obj/*.c 

check_fix:
	clang-format -i ./lib/3DViewer/includes/*.hpp ./lib/3DViewer/srcs/*.cpp ./lib/QtGifImage/includes/*.h ./lib/QtGifImage/srcs/*.cpp ./code_c/*.c ./code_c/*.h ./code_c/gcov_obj/*.c 

re: clean all
