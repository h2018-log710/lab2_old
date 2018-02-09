CC=gcc
CFLAGS=-Wall -g 
LDFLAGS=
LDLIBS=
TARGET=Log710H18_lab02
VPATH=src
ODIR=obj
BINDIR=bin

objects=main.o scheduler.o list.o

$(TARGET): $(objects)
	@mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/$(TARGET) $(LDFLAGS) $(objects)

main.o: scheduler.h
log710h15process.o:
scheduler.o: scheduler.h list.h
list.o: list.h

clean:
	rm $(objects) $(BINDIR)/$(TARGET)

run: $(TARGET)
	./$(BINDIR)/$(TARGET)
