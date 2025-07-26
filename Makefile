CC = gcc
CFLAGS = -Wall -Wextra -std=gnu89 -O2 -MMD
OBJ = main.o strbuf.o wctable.o
DEP = $(OBJ:.o=.d)
TARGET = program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

debug: CFLAGS = -Wall -Wextra -g -O0 -MMD
debug: clean $(TARGET)
	@echo "Debug build complete. Run with: gdb ./$(TARGET)"

-include $(DEP)

clean:
	rm -f $(OBJ) $(DEP) $(TARGET)