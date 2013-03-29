#| Projet PNE		 				|
#|									|
#| by : Benjamin Blois 				|
#| last modif : 29/03/13 			|
#/__________________________________/


GCC = g++
LFLAGS = -g
CFLAGS = -I=lib/ -std=c++0x -Wall -Wextra -pedantic -g

# Name of the files (more .o)
OBJS = main.o
PARAMS =

# Path where the bin will be
PGM_PATH = ../bin/
PGM_NAME = lzw

# For the generic utilisation,
# DO NOT TOUCH ANYTHING UNDER THIS LINE !!!
# ___________________________________________
    
all : $(OBJS)
$(GCC) -o $(PGM_PATH)$(PGM_NAME) $(LFLAGS) $(OBJS)

# generic method for all the .o
%.o : %.cpp
$(GCC) $(CFLAGS) -c $<

run :
$(PGM_PATH)$(PGM_NAME) $(PARAMS)

clean :
-rm -Rf *.o *~

mrproper : clean
-rm -f $(PGM_PATH)$(PGM_NAME)
