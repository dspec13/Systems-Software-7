compile:
	g++ -Werror -Wall -g -c -lssl -lcrypto Hash.cpp
	g++ -Werror -Wall -g -o useHash Hash.o useHash.cpp

checkpoint:
	cp Hash.h checkpoint7.c
	cat Hash.cpp >> checkpoint7.c
	~jloew/CSE109/submitCheckpoint.pl 7
