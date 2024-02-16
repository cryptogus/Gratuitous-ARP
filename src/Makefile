all: garp-test get-network-info-test

#main.o: main.c

garp_poc.o: garp_poc.c

ethhdr.o: ethhdr.h

getinfo.o: getinfo.c

garp-test: garp_poc.o
	$(CC) $^ $(LOADLIBES) -o $@

get-network-info-test: getinfo.o
	$(CC) $^ $(LOADLIBES) -o $@

clean:
	rm -f garp-test get-network-info-test *.o
