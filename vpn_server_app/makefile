CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -pthread

SRC = vpn_server.c
OUT = vpn_server

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT)

.PHONY: all clean
