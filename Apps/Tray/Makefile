BINNAME     = ../../bin/Apps/Tray
LIBS        = `wx-config --libs` -lslide
CXXFLAGS    = `wx-config --cxxflags`
CXX         = g++

OBJS        = main.o tray.o ctrlthread.o
SOURCES     = main.cpp tray.cpp ctrlthread.cpp

default: $(OBJS)
	$(CXX) -o $(BINNAME) $(LIBS) $(OBJS)
main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp
tray.o:
	$(CXX) $(CXXFLAGS) -c tray.cpp
ctrlthread.o:
	$(CXX) $(CXXFLAGS) -c ctrlthread.cpp

clean:
	rm -f $(OBJS)

