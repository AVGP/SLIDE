BINNAME     = ../../bin/Apps/Desk
LIBS        = `wx-config --libs`
CXXFLAGS    = `wx-config --cxxflags`
CXX         = g++

OBJS        = main.o desk.o
SOURCES     = main.cpp desk.cpp

default: $(OBJS)
	$(CXX) -o $(BINNAME) $(LIBS) $(OBJS)
main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp
starter.o:
	$(CXX) $(CXXFLAGS) -c desk.cpp
