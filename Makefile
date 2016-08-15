VERSION	= 1.0
CC	= gcc
CFLAGS	= -g -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/glib-2.0 -I.
LDFLAGS	= -lm -lglib-2.0 -lssl -lcrypto
NAME	= libpsi-util.a
OBJ	= psi_hashing.o psi_misc.o

default: psi-util

psi-util: $(OBJ)
	/usr/bin/ar -cvq $(NAME) $(OBJ) 

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean: 
	rm $(OBJ) $(NAME)

remove:
	rm -f /usr/local/include/psi_hashing.h
	rm -f /usr/local/include/psi_misc.h
	rm -f /usr/local/include/psi_structures.h
	rm /usr/local/lib/$(NAME)

install: psi-util
	ranlib $(NAME)
	cp $(NAME) /usr/local/lib/$(NAME)	
	cp *.h /usr/local/include
