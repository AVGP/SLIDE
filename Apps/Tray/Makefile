BINNAME     = ../../bin/Apps/Tray
LIBS        = `wx-config --libs`
CXXFLAGS    = `wx-config --cxxflags`
CXX         = g++

OBJS        = main.o tray.o
SOURCES     = main.cpp tray.cpp

default: $(OBJS)
	$(CXX) -o $(BINNAME) $(LIBS) $(OBJS)
main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp
tray.o:
	$(CXX) $(CXXFLAGS) -c tray.cpp

clean:
	rm -f $(OBJS)

