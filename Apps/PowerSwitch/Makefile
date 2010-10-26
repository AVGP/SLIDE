BIN      = ../../bin/Apps/powerswitch
CXX      = g++
CXXFLAGS = 

OBJS     = main.o

default: main.o
	$(CXX) -o $(BIN) main.o

main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp


clean:
	rm -f *.o
