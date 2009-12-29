Copyright (C) <2009, 2010>  <Marcel Ribeiro Dantas>
####################################################################
#                                                                  #
#  Author:  Marcel Ribeiro Dantas <marcel at ribeirodantas.com.br> #
#  Website: http://www.ribeirodantas.com.br/blog                   #
#  This code is licensed under GNU GPLv3 or any other newer        #
#  version of this license released of your choice (GPLv3+).       #
#                                                                  #
#  A copy of the license can be read in:                           #
#  http://www.gnu.org/licenses/gpl-3.0.txt                         #
#  or in the COPYING file which should follow with this			   #
#  source file one.												   #	
#                                                                  #  
#  dtool - a dictionary tool with many features.                   # 
#  A tool for help you translating                                 # 
#  and learning more about a word in                               #  
#  a lot of different languages                                    #
#                                                                  #
####################################################################

SRC=dtool.c
OBJ=$(SRC:.c=.o)
OUT=dtool
CC=gcc ${CFLAGS}
RM=rm -f

default: all

.c.o:
        $(CC) -c $<

dtool: $(OBJ)
        $(CC) $(OBJ) -o $(OUT)

all: $(OUT)
clean: 
        $(RM) $(OBJ) $(OUT)
