CC = gcc
CC_FLAGS = -Wall -O0 -ggdb -c
OUTEXEC_FLAGS = -o runner
# DEBUG_FLAGS = -ggdb -o debug
SRCS = $(wildcard *.c)
OBJS := $(SRCS:%.c=%.o)

all: main clean

main: $(OBJS)
	$(CC) $(OUTEXEC_FLAGS) $?

%.o: %.c
	$(CC) $(CC_FLAGS) $<

clean:
	rm *.o