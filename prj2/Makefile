TARGET ?=connect4
CC ?=gcc
CFLAGS +=-Wall -Wextra -O3 -DNDEBUG -std=c99 -I.
SOURCES += $(wildcard *.c)
SOURCES += $(wildcard libs/*.c)

ifeq ($(OS),Windows_NT)
SHELL =cmd.exe
NATIVEPATH =$(subst /,\,$(1))
RM =del /f 2>nul
SOURCES :=$(call NATIVEPATH,$(SOURCES))
else
RM =rm -f
endif

OBJECTS :=$(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(TARGET) $(OBJECTS)

.PHONY: clean all
