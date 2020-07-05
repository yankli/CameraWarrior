all: camera_warrior
CC := arm-linux-gnueabihf-g++
EXT_FLAGS :=

ifneq ($(LOG),)
EXT_FLAGS += -DGLOG=$(LOG)
else
EXT_FLAGS += -DGLOG=4
endif

camera_warrior: src/*.cpp src/*.h
	$(CC) $(EXT_FLAGS) -g -std=c++11 src/*.cpp -o $@
	ln -sf camera_warrior ispreg
	ln -sf camera_warrior flashreg
	ln -sf camera_warrior flasburn
	ln -sf camera_warrior flashdump
	ln -sf camera_warrior zcv
	ln -sf camera_warrior zcu

clean:
	rm -rf camera_warrior
