all: moto

include Makefile.inc

clean:
	rm -fv *.a *.o moto

moto: main.o image.o bmp.o flatland.o
	$(LINK) -o $@ $?

