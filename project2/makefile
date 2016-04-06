obj = mem-sim
all:${obj}
mem-sim.o:./include/header.h main.cpp
	g++ -I./include -c main.cpp -o mem-sim.o -pthread
mem-sim:mem-sim.o
	g++ -I./include mem-sim.o -o mem-sim -pthread
clean:
	-rm -f *.o
	-rm -f ${obj}

