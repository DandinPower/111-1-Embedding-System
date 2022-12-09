.PHONY: all check clean

CC = gcc
CFLAGS = -std=gnu99 -Wall -g

OBJS = main.o
EXEC = main

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJS)
	$(CC) $^ -o $(EXEC)

check: $(EXEC)
	@./$(EXEC); \
	if [ $$? -eq 0 ] ; then \
		echo "PASS."; \
	else \
		echo "FAIL."; \
	fi

clean:
	rm -f $(OBJS) main
