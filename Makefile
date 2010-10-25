#Makefile for SLIDE
BINNAME = bin/Slide

CXX = g++
CXXFLAGS = `wx-config --cxxflags`
LIBS = `wx-config --libs` -lX11

OBJS = main.o Slideconfig.o Slideconnection.o Slide.o Slidewindow.o Slidewindowmanager.o Logger.o


default: $(OBJS)
	$(CXX) -o $(BINNAME)  $(CXXFLAGS) $(OBJS) $(LIBS)

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

Logger.o:
	$(CXX) $(CXXFLAGS) -c Logger.cpp

clean:
	rm -f $(OBJS)

