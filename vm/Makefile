CC=cc
OPT=-O3
STD=-std=c89
LDFLAGS=
WARNING=-Wall -Wextra -Wpedantic -Wfloat-equal -Wundef -Wshadow \
		-Wpointer-arith -Wcast-align -Wstrict-prototypes -Wmissing-prototypes \
		-Wstrict-overflow=2 -Wwrite-strings -Waggregate-return -Wcast-qual \
		-Wswitch-enum -Wunreachable-code -Wformat -Wformat-security -Wvla \
		-Wmissing-declarations

FLAGS=-fstack-protector-all -fPIE -pipe
CFLAGS=$(WARNING) $(STD) $(OPT) $(FLAGS)

SRC = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJS = $(patsubst %.c,%.o,$(SRC))

.PHONY: release
release: OPT=-O3 -D_FORTIFY_SOURCE=2 -mtune=native -Wno-unused-value -flto
release: all

.PHONY: debug
debug: OPT=-O0 -ggdb3 -DDEBUG -Werror
debug: all

.PHONY: sanitize
sanitize: OPT=-O0 -ggdb3 -DDEBUG -Werror -fsanitize=address,undefined,leak
sanitize: all

.PHONY: all
all: vm

vm: $(OBJS)
	$(CC) -o vm $(OBJS) $(CFLAGS) $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: lint
lint:
	clang-tidy *.c *.h

.PHONY: fmt
fmt:
	clang-format -i *.c *.h

.PHONY: clean
clean:
	rm -f vm
	rm -f *.o
