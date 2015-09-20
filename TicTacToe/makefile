CC=g++
IDIR=.
CFLAGS:=-I$(IDIR) -std=c++11

EXE=tictactoe.out

_DEPS = TicTacToe.h Address.h Socket.h
DEPS := $(patsubst %, $(IDIR)/%, $(_DEPS))

ODIR=obj

_OBJ = main.o TicTacToe.o Address.o Socket.o
OBJ := $(patsubst %, $(ODIR)/%, $(_OBJ))

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(EXE)

.PHONY: folder

folder:
	mkdir $(ODIR)
