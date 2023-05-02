CXX =g++
CXXFLAGS=-O2
RM=rm

all: deeperindigo avg probs

deeperindigo: deeperindigo.o
	$(CXX) $(CXXFLAGS) -o deeperindigo deeperindigo.o

deeperindigo.o: deeperindigo.cpp
	$(CXX) $(CXXFLAGS) -c deeperindigo.cpp

avg: avg.o
	$(CXX) $(CXXFLAGS) -o avg avg.o

avg.o: avg.cpp
	$(CXX) $(CXXFLAGS) -c avg.cpp

probs: probs.o
	$(CXX) $(CXXFLAGS) -o probs probs.o

probs.o: probs.cpp
	$(CXX) $(CXXFLAGS) -c probs.cpp

clean: 
	$(RM)  deeperindigo avg probs deeperindigo.o avg.o probs.o
