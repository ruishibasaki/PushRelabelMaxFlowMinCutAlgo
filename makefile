# compiler
CC = g++

# module name
NAME = psh_rlbl

# basic directory
DIR = ./

# debug switches
#SW = -g -Wall -fPIC -fexceptions -DNDEBUG -DIL_STD
# production switches
SW = -O3

# default target- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

default: $(NAME)


# clean - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

clean::
	rm -f $(DIR)*.o $(DIR)*~ $(NAME)

# define & include the necessary modules- - - - - - - - - - - - - - - - - - -
		
OBJ = MaxFeasibleFlow.cpp main.cpp

$(NAME): $(OBJ)
	$(CC) -o  $(NAME) $(OBJ)  $(CPLEXFLAGS)  $(SW)



