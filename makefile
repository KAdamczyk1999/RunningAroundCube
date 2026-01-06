SOURCE_FILES=$(wildcard src/*.c) \
			 $(wildcard src/Geometry/*.c) \
			 $(wildcard src/View/*.c) \
			 $(wildcard src/LinearAlgebra/*.c) \

windows:
	gcc -g --std=c99 -I./include -L./lib $(SOURCE_FILES) -D WINDOWS -lglfw3dll -o bin/running_around_cube

linux:
	gcc -g --std=c99 -I./include $(SOURCE_FILES) -D LINUX -lglfw -lm -o bin/running_around_cube


run:
	./bin/running_around_cube
	
clean:
	rm ./bin/running_around_cube*
