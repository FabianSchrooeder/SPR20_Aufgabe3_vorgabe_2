BIN = scheduler
SRC = \
	main.c	\
	src/colors.c	\
	src/FCFS.c 	\
	src/HRRN.c	\
	src/LCFS.c	\
	src/PRIOP.c	\
	src/queue.c	\
	src/RR.c	\
	src/MLF.c	\
	src/SRTNP.c	

CC	     = gcc
CFLAGS   = -g -Wall -Wextra -std=c99 -Wno-unused-parameter

GDB    = gdb
RM     = rm -f

OBJS = $(SRC:%.c=%.o)

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@


# pull in dependencies for .o files
-include $(OBJS:.o=.d)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

debug: $(BIN)
	$(GDB) ./$(BIN)

run: $(BIN)
	./$(BIN)

clean:
	$(RM) $(BIN) $(OBJS) *.d

.PHONY: clean debug run
