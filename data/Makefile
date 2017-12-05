CC = icpc
CPPFLAGS = -g -Wall -Wextra -std=c++14

.PHONY: all clean

all: combine

combine: combine.cpp
	$(CC) $(CPPFLAGS) $^ -o $@

clean:
	rm -rf combine
