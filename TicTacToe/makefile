CC=g++
IDIR=.
CFLAGS:=-I$(IDIR)

EXE=tictactoe.out

_DEPS = TicTacToe.h
DEPS := $(patsubst %, $(IDIR)/%, $(_DEPS))

ODIR=obj

_OBJ = main.o TicTacToe.o
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
