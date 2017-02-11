LIBNAME=libSprinkler
TARGET=${LIBNAME}.so

LDFLAGS = -L/usr/local/lib
LIBS  = -lwiringPi

all:
	gcc -I. -std=c99 -shared -Wl,-soname,${LIBNAME} -o ${TARGET} -fPIC ${LDFLAGS} ${LIBS} sprinkler.c

clean:
	rm -f ${TARGET}
