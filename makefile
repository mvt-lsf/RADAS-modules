CC = gcc
CFLAGS = -I./headers -IC:/ADLINK/WD-DASK/Include
LDIR = -LC:/ADLINK/WD-DASK/Lib
LIBS = -lWD-Dask64

main : 
	$(CC) -o ./bin/main.exe main.c $(CFLAGS) $(LDIR) $(LIBS)