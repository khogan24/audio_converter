CFLAGS= -Wall -Werror
all:
	gcc main.c $(CFLAGS) -o audio_converter.o

clean:
	rm -rf *.o