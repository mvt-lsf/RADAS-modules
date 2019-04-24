CC = gcc
CFLAGS = -I./headers -IC:/ADLINK/WD-DASK/Include -g
ODIR = obj
LDIR = -LC:/ADLINK/WD-DASK/Lib
LIBS = -lWD-Dask64

HDIR = headers
_DEPS = card.h config.h main.h
DEPS = $(patsubst %,$(HDIR)/% ,$(_DEPS))

BIN = ./bin

_OBJ = main.o card.o config.o
OBJ = $(patsubst %,$(ODIR)/% ,$(_OBJ))

$(ODIR)/%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main : $(OBJ)
	$(CC) -g -o $(BIN)/main.exe $(OBJ) $(CFLAGS) $(LDIR) $(LIBS)