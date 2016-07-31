LDFLAGS=-lpthread $(shell pkg-config --libs dbus-1)
CFLAGS=$(shell pkg-config --cflags dbus-1)
CC=gcc
DEPS = bkd.h
OBJS = bkd.o dbus-service.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)


bkd-dbus-service: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)
clean:
	rm -f *.o a.out bkd-dbus-service
