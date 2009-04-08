Copyright (C) <2009>  <Marcel Ribeiro Dantas>
###################################################################
#                                                                 #
#  Author: Marcel Ribeiro Dantas <vuln at slackware-rn.com.br>    #
#  Website: http://www.slackware-rn.com.br/~vuln                  #
#  This code is licensed under GNU GPLv3 and any other            #
#  version of this license released by FSF.                       #
#                                                                 #
#  A copy of this license can be read in:                         #
#  http://www.gnu.org/licenses/gpl-3.0.txt                        #
#                                                                 #
#  dtool - a dictionary tool with many features.                  #
#  A tool for help you translating                                #
#  and learning more about a word in                              #
#  a lot of different languages                                   #
#                                                                 #
###################################################################

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
