all: principal

principal: blobsFront.o blobsBack.o
	gcc blobsFront.o blobsBack.o -o TPE

blobsFront.o: blobsFront.c
	gcc -c blobsFront.c

blobsBack.o: blobsBack.c
	gcc -c blobsBack.c

limpiar:
	rm -rf *o
