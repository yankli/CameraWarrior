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

camera_warrior: src/*.cpp src/include/*.h  libzcamdrv.so
	$(CC) $(EXT_FLAGS) -g -std=c++11 $^ -L. -lzcam -Wl,--no-as-needed -lzcamdrv -o $@

ifneq ($(wildcard src/core/*.cpp),)
libzcam.so: src/core/*.cpp src/include/*.h
	$(CC) $(EXT_FLAGS) -fPIC -shared -g -std=c++11 $^ -o $@
endif
ifneq ($(wildcard src/drivers/*.cpp),)
libzcamdrv.so: src/drivers/*.cpp src/include/*.h libzcam.so
	$(CC) $(EXT_FLAGS) -fPIC -shared -g -L. -lzcam -std=c++11 $^ -o $@
endif

new: src/drivers/zcam_drv_ar0143.cpp
	cp ./src/drivers/zcam_drv_ar0143.cpp ./src/drivers/zcam_drv_$(N).cpp
	cp ./bin/ar0143_verify.sh ./bin/$(N)_verify.sh
	sed -i 's/ar0143/$(N)/g' ./src/drivers/zcam_drv_$(N).cpp

install: camera_warrior libzcam.so FORCE
	-@mkdir -p install; \
	cd install; \
	cp -rf ../boards ./; \
	cp -rf ../cameras ./; \
	cp -rf ../bin .; \
	cp ../camera_warrior . ;\
	cp ../*.so .
	-cd install; \
	ln -sf camera_warrior ispreg; \
	ln -sf camera_warrior flashreg; \
	ln -sf camera_warrior flashburn; \
	ln -sf camera_warrior flashdump; \
	ln -sf camera_warrior zcv; \
	ln -sf camera_warrior zcu

clean:
	rm -rf camera_warrior
	rm -rf *.so
	rm -rf install

.PHONY: clean FORCE
