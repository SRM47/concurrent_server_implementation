CC = gcc
CFLAGS = -Wall -Werror

# The target executable
TARGET = server

# Source files
SRCS = server.c
SRCS += queue.c  # Add other source files if needed

# Object files
OBJS = $(SRCS:.c=.o)

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@