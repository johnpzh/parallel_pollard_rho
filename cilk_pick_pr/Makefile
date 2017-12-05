CC = icpc
CPPFLAGS = -g -Wall -Wextra -std=c++14 -qopenmp

.PHONY: all clean

all: pr

pr: pr.cpp
	$(CC) $(CPPFLAGS) $^ -o $@

clean:
	rm -rf pr
