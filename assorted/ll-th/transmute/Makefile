all: cmute

mod.mo: mod.c
	$(CC) -Wall -c -o $@ $^

cmute: code-transmute.c tests.c mod.mo
	$(CC) -Wall -Xlinker -T mute.lds $^ -o $@ -lbfd -lopcodes

clean:
	rm -rf *~ *.mo *.o cmute
