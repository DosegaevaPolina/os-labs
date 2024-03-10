CC = gcc
CFLAGS = -std=c99 -Wall -pedantic
MATH_FLAG = -lm
PIC_FLAG = -fPIC
SHARED_FLAG = -shared
DYNAMIC_LOADING_FLAG = -ldl

all : linking_realization1 linking_realization2 dynamic

linking_realization2 : linking_main dymanic_realization2
		$(CC) $(CFLAGS) linking_main.o -L. -lrealization2 -o linking_realization2 $(MATH_FLAG)

linking_realization1 : linking_main dynamic_realization1
		$(CC) $(CFLAGS) linking_main.o -L. -lrealization1 -o linking_realization1 $(MATH_FLAG)

linking_main: linking_main.c
		$(CC) $(CFLAGS) -c linking_main.c

# realization2 : realization2.c pi.h
# 		$(CC) $(CFLAGS) -o $(SHARED_FLAG) $(PIC_FLAG) librealization2.so realization2.c

# realization1 : realization1.c pi.h
# 		$(CC) $(CFLAGS) -o $(SHARED_FLAG) $(PIC_FLAG) librealization1.so realization1.c

dynamic : dynamic_main dynamic_realization1 dymanic_realization2
		$(CC) $(CFLAGS) dynamic_main.o -o dynamic $(DYNAMIC_LOADING_FLAG)

dynamic_main : dynamic_main.c 
		$(CC) $(CFLAGS) -c dynamic_main.c

dymanic_realization2 : realization2.c pi.h
		$(CC) $(CFLAGS) $(PIC_FLAG) $(SHARED_FLAG) -o librealization2.so realization2.c $(MATH_FLAG)

dynamic_realization1 : realization1.c pi.h
		$(CC) $(CFLAGS) $(PIC_FLAG) $(SHARED_FLAG) -o librealization1.so realization1.c $(MATH_FLAG)

clean : 
		rm *.o *.so linking_realization1 linking_realization2 dynamic
