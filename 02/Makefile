
objects = 02
all: $(objects)
CC=gcc

warn = $(shell grep -v \# ../warn_config | tr \\n ' ')
override CFLAGS += -g -Wextra -Wall -Wpedantic -Werror $(warn)

$(objects): %: %.c
	@$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	@rm -rf *.exe

test:
	bash ../runtest.sh 02
