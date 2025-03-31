cc = cc
ldflags = \
	-L/usr/local/lib \
	-lX11 \
	-lm \
	-pthread \
	$(LDFLAGS)
cflags = \
	-g \
	-O3 \
	-std=c11 \
	-I/usr/local/include \
	-Wall \
	-Wwrite-strings \
	-Wextra \
	-Werror \
	-Wpedantic \
	-D_POSIX_C_SOURCE=200809L \
	-D_TIME_BITS=64 \
	-D_FILE_OFFSET_BITS=64 \
	 $(CFLAGS)
src = $(shell find src -name '*.c')
headers = $(shell find src -name '*.h')
lib = $(patsubst src/%,lib/%,$(patsubst %.c,%.o,$(src)))
app = inet-forth
bin = bin/$(app)

.PHONY: all run self-test run-examples test clean

all: bin/$(app)

run: bin/$(app)
	./bin/$(app)

self-test: bin/$(app)
	./bin/$(app) self-test

run-examples: bin/$(app)
	bash run-examples.sh

test: self-test run-examples

bin/$(app): $(lib) lib/$(app).o
	mkdir -p $(dir $@); $(cc) $^ $(ldflags) -o $@

lib/%.o: src/%.c $(headers)
	mkdir -p $(dir $@); $(cc) -c $(cflags) $< -o $@

clean:
	rm -rf lib bin
