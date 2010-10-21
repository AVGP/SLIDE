#Makefile for SLIDE
BINNAME = Slide

CXX = g++
CXXFLAGS = `wx-config --cxxflags`
LIBS = `wx-config --libs` -lX11

OBJS = Slideconfig.o Slideconnection.o Slide.o Slidewindow.o Slidewindowmanager.o


default: $(OBJS)
	$(CXX) -o $(BINNAME)  $(CXXFLAGS) $(OBJS) $(LIBS)


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

