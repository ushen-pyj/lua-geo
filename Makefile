CC ?= gcc
LUA_VERSION ?= 5.4
LUA_INC ?= /usr/include/lua$(LUA_VERSION)
TARGET = geo.so
SRCS = geo.c lua-geo.c
CFLAGS += -O2 -Wall -Wextra -fPIC -I$(LUA_INC)
LDFLAGS += -shared -lm

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SRCS) geo.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET) *.o

test: $(TARGET)
	lua test.lua
