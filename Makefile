CPP = g++
#Utilise pour avoir acces aux ajouts du c++11
CC_FLAGS = -O3 -Wall --std=c++0x

CPP_FILES = $(wildcard src/*.cpp)
OBJ_FILES = $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

all : $(OBJ_FILES)
	$(CPP) -o $@ $^
	echo pne done

obj/%.o: src/%.cpp
	$(CPP) $(CC_FLAGS) -c -o $@ $<

clean :
	rm $(OBJ_FILES)