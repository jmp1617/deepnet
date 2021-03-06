
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CC = gcc
	CFLAGS = -Wall -Wextra -pedantic -lglfw -lGLEW -lGL -lX11 -lpthread -lXrandr -lXi -lm
endif
ifeq ($(UNAME_S),Darwin)
	CC = clang
	CFLAGS = -Wall -Wextra -pedantic -lGLFW -lGLEW -lm -framework OpenGL
endif

default: deepnet

deepnet: deepnet.c dn_aux.c dn_math.c render.c deepnet.h
	$(CC) -o deepnet $(CFLAGS) deepnet.c dn_aux.c dn_math.c render.c deepnet.h

spec: deepnet.c dn_aux.c dn_math.c render.c deepnet.h
	$(CC) -o deepnet $(CFLAGS) -DSPEC deepnet.c dn_aux.c dn_math.c render.c deepnet.h

line: deepnet.c dn_aux.c dn_math.c render.c deepnet.h
	$(CC) -o deepnet $(CFLAGS) -DLINE deepnet.c dn_aux.c dn_math.c render.c deepnet.h

tri: deepnet.c dn_aux.c dn_math.c render.c deepnet.h
	$(CC) -o deepnet $(CFLAGS) -DTRI deepnet.c dn_aux.c dn_math.c render.c deepnet.h

clean:
	rm -rf *.gch
