BINNAME     = ../../bin/Apps/Starter
LIBS        = `wx-config --libs`
CXXFLAGS    = `wx-config --cxxflags`
CXX         = g++

OBJS        = main.o starter.o icon.o
SOURCES     = main.cpp starter.cpp icon.cpp

default: $(OBJS)
	$(CXX) -o $(BINNAME) $(LIBS) $(OBJS)
main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp
starter.o:
	$(CXX) $(CXXFLAGS) -c starter.cpp
icon.o:
	$(CXX) $(CXXFLAGS) -c icon.cpp

clean:
	rm -f $(OBJS)

