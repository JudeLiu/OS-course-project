all: MyCopy ForkCopy PipeCopy MyShell MoreShell DupShell MergesortSingle MergesortMulti BurgerBuddies
clear:
	rm *.o
MyCopy.o : MyCopy.cpp header.h
	g++ -c MyCopy.cpp -o MyCopy.o
MyCopy: MyCopy.o
	g++ MyCopy.o -o MyCopy

ForkCopy.o : ForkCopy.cpp header.h
	g++ -c ForkCopy.cpp -o ForkCopy.o
ForkCopy: ForkCopy.o
	g++ ForkCopy.o -o ForkCopy

PipeCopy.o : PipeCopy.cpp header.h
	g++ -c PipeCopy.cpp -o PipeCopy.o
PipeCopy: PipeCopy.o
	g++ PipeCopy.o -o PipeCopy

MyShell.o : MyShell.cpp header.h
	g++ -c MyShell.cpp -o MyShell.o
MyShell: MyShell.o
	g++ MyShell.o -o MyShell

MoreShell.o : MoreShell.cpp header.h
	g++ -c MoreShell.cpp -o MoreShell.o
MoreShell: MoreShell.o
	g++ MoreShell.o -o MoreShell

DupShell.o : DupShell.cpp header.h
	g++ -c DupShell.cpp -o DupShell.o
DupShell: DupShell.o
	g++ DupShell.o -o DupShell

MergesortSingle.o: MergesortSingle.cpp header.h
	g++ -c MergesortSingle.cpp -o MergesortSingle.o
MergesortSingle: MergesortSingle.o
	g++ MergesortSingle.o -o MergesortSingle

MergesortMulti.o: MergesortMulti.cpp header.h
	g++ -c MergesortMulti.cpp -o MergesortMulti.o -pthread
MergesortMulti: MergesortMulti.o
	g++ MergesortMulti.o -o MergesortMulti -pthread

BurgerBuddies.o : BurgerBuddies.cpp header.h
	g++ -c BurgerBuddies.cpp -o BurgerBuddies.o -pthread
BurgerBuddies: BurgerBuddies.o
	g++ BurgerBuddies.o -o BBQ -pthread

