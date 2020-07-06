all: camera_warrior
CC := arm-linux-gnueabihf-g++
EXT_FLAGS := -I./src/include

USER := "$(shell uname -n)"
EXT_FLAGS += -D__BUSER__=\"$(USER)\"

ifneq ($(LOG),)
EXT_FLAGS += -DGLOG=$(LOG)
else
EXT_FLAGS += -DGLOG=4
endif

camera_warrior: src/*.cpp src/drivers/*.cpp src/include/*.h libzcam.so
	$(CC) $(EXT_FLAGS) -g -std=c++11 $^ -L. -lzcam -o $@

ifneq ($(wildcard src/core/*.cpp),)
libzcam.so: src/core/*.cpp src/include/*.h
	$(CC) $(EXT_FLAGS) -fPIC -shared -g -std=c++11 $^ -o $@
endif

install: camera_warrior libzcam.so FORCE
	-@mkdir -p install; \
	cd install; \
	cp -rf ../boards ./; \
	cp -rf ../cameras ./; \
	cp -rf ../bin .; \
	cp ../camera_warrior . \
	cp ../libzcam.so .
	-cd install; \
	ln -sf camera_warrior ispreg; \
	ln -sf camera_warrior flashreg; \
	ln -sf camera_warrior flashburn; \
	ln -sf camera_warrior flashdump; \
	ln -sf camera_warrior zcv; \
	ln -sf camera_warrior zcu

clean:
	rm -rf camera_warrior
	rm -rf libzcam.so
	rm -rf install

.PHONY: clean FORCE
