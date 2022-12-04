current_os := $(shell sh -c 'uname -s 2>/dev/null || echo not')

.PHONY: all clean rebuild

all: checkdir install start

clean:
	rm -rf ./BUILD
	rm -rf ./BUILD2
	rm -f 3d.tar
# 	rm -f CMakeLists.txt.user
	rm -rf ./libBin
	cd ./code_c/ && make clean

install:
	cd BUILD && cmake .. && make

uninstall: clean
	

checkdir:
	@echo "checkdir"
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


re: clean all

