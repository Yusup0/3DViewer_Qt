DIR=BUILD
# .NOPARALLEL : checkdir
# .PHONY : checkdir
.ONESHELL:


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

checkdir :
	@echo "checkdir"
	# @if [ ! -d $(DIR) ] ; then
		mkdir -p BUILD
	# fi

start:
	./BUILD/3DV

tar:
	tar -cf 3d.tar ./*

tests: checkdir install
	./BUILD/bin/testMyWidgetOpenGL


re: clean all

