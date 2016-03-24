CC = g++
CFLAGS = -Wall -Werror

all: myDCT myIDCT

myDCT: myDCT.cpp common.o
	$(CC) $(CFLAGS) -o myDCT myDCT.cpp common.o
	
myIDCT: myIDCT.cpp common.o
	$(CC) $(CFLAGS) -o myIDCT myIDCT.cpp common.o
	
common.o:
	$(CC) $(CFLAGS) -c common.cpp
	
clean:
	rm -f common.o
	rm myDCT
	rm myIDCT