client: client.o api.o
	g++ -o client client.o api.o -pthread

api.o: api.h metadata.h
client.o: api.h client.h addrTree.h metadata.h host.h
mdServer1.o: mdServer1.h



clean:
	rm client client.o api.o