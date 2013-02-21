LIBS=libswlib.so
BINS=swconnect sw1

all: $(BINS)

$(LIBS): swlib.c tlv.c
	$(CC) -Wall -shared $^ -o $@

swconnect: $(LIBS) swconnect.c
	$(CC) -Wall -L. -lswlib $^ -o $@

sw1: $(LIBS) sw1.c
	$(CC) -Wall -L. -lswlib $^ -o $@

clean:
	rm -f *~ $(LIBS) $(BINS)
