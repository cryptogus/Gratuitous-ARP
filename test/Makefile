all: garp-test get-network-info-test

garp_poc.o: garp_poc.c

getinfo.o: getinfo.c

garp-test: garp_poc.o
	$(CC) $^ $(LOADLIBES) -o $@

get-network-info-test: getinfo.o
	$(CC) $^ $(LOADLIBES) -o $@

clean:
	rm -f garp-test get-network-info-test *.o
