CC=gcc
CFLAGS=-Wall -g 
LDFLAGS=
LDLIBS=
TARGET=Log710H18_lab02
EXTRA_TARGET=log710h15process
VPATH=src
ODIR=obj
BINDIR=bin

objects=main.o scheduler.o list.o resource.o
extra_objects=log710h15process.o

$(TARGET): $(objects) $(EXTRA_TARGET)
	@mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/$(TARGET) $(LDFLAGS) $(objects)

$(EXTRA_TARGET): $(extra_objects)
	@mkdir -p $(BINDIR)
	$(CC) -o $(BINDIR)/$(EXTRA_TARGET) $(LDFLAGS) $(extra_objects)

main.o: scheduler.h
log710h15process.o:
scheduler.o: scheduler.h list.h resource.h
list.o: list.h
resource.o: resource.h scheduler.h

clean:
	rm $(objects) $(BINDIR)/$(TARGET)

run: $(TARGET)
	./$(BINDIR)/$(TARGET)
