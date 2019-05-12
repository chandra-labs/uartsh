##
 # Author: Chandra Mohan C (willvalab@gmail.com)
 # All rights reserved.
 #
 # Use of this source code is governed by a MIT-style license that can be found
 # in the LICENSE file.
 #
##-----------------------------------------------*/

EXE = main

all : $(EXE)

SRC = ../argparse.c ../uartsh.c ../uartshCommands.c
SRC += main.c rawConsole.c uartshCommandsRegistry.c

LIBS =
LIBS := $(addprefix -l, $(LIBS))

OBJS = $(SRC:.c=.o)

$(EXE): $(OBJS)
	gcc -o $@ $(OBJS) -L. $(LIBS)
	
%.o: %.c
	gcc -c $< -o $@
	
clean:
	rm -f $(OBJS) $(EXE)
