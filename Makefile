#Makefile for SLIDE
BINNAME = ./bin/Slide

CXX = g++
CXXFLAGS =
LIBS = -lX11

OBJS = Logger.o main.o Slideconfig.o Slideconnection.o Slide.o Slidewindow.o Slidewindowmanager.o



default: $(OBJS)
	$(CXX) -o $(BINNAME) $(LIBS) $(OBJS)


Logger.o:
	$(CXX) $(CXXFLAGS) -c Logger.cpp

main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp

Slideconfig.o:
	$(CXX) $(CXXFLAGS) -c Slideconfig.cpp

Slideconnection.o:
	$(CXX) $(CXXFLAGS) -c Slideconnection.cpp

Slide.o:
	$(CXX) $(CXXFLAGS) -c Slide.cpp
	
Slidewindow.o:
	$(CXX) $(CXXFLAGS) -c Slidewindow.cpp

Slidewindowmanager.o:
	$(CXX) $(CXXFLAGS) -c Slidewindowmanager.cpp



clean:
	rm -f $(OBJS)

