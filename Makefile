CC = gcc

CFLAGS = -Wall -Wextra -pedantic -lglfw -lGLEW -lGL -lX11 -lpthread -lXrandr -lXi -lm

default: deepnet

deepnet: deepnet.c dn_aux.c dn_math.c render.c deepnet.h
	$(CC) $(CFLAGS) -o deepnet deepnet.c dn_aux.c dn_math.c render.c deepnet.h

