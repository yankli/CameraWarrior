all: camera_warrior

EXT_FLAGS :=

ifneq ($(LOG),)
EXT_FLAGS += -DGLOG=$(LOG)
else
EXT_FLAGS += -DGLOG=4
endif

camera_warrior: *.cpp *.h
	g++ $(EXT_FLAGS) -g -std=c++11 *.cpp -o $@

clean:
	rm -rf camera_warrior
