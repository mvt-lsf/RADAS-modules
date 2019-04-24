CC = gcc
CFLAGS = -I./headers -IC:/ADLINK/WD-DASK/Include
ODIR = ./obj
LDIR = -LC:/ADLINK/WD-DASK/Lib
LIBS = -lWD-Dask64
DEPS = card.h main.h

BIN = ./bin

_OBJ = main.o card.o
OBJ = ($(ODIR)/%,$(_OBJ))

%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main : $(OBJ)
	$(CC) -o $(BIN)/main.exe main.o card.o $(CFLAGS) $(LDIR) $(LIBS)