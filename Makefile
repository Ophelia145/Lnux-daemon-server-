CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = daemon

SRC = daemon.c server.c control_processes.c read_ELF.c 
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

client: client.o
	$(CC) $(CFLAGS) -o client client.o

rebuild:
	$(clean) $(all)

clean:
	rm -rf $(OBJ) $(TARGET) client.o

