EXECS = Simulation
OBJS =  Event.o Patient.o SurgeryCenter.o Hospital.o min_cost_maxflow.o Test.o #MaxFlow.o BasicGreedy.o ExpectationGreedy.o RandomMaxFlow.o Log.o # DeterministicMaxFlow.o MeldMaxFlow.o CombinedMaxFlow.o OnlineMaxFlow.o
#### use next two lines for Mac
#CC = g++
#CCFLAGS = -std=c++14 -stdlib=libc++ -g

#### use next two lines for mathcs* machines:
CC = g++
CCFLAGS = -std=c++11 -Wall -Wno-deprecated -Werror=return-type -g

all: $(EXECS)

Simulation: $(OBJS)  
	$(CC) $(CCFLAGS) $^ -o $@

%.o: %.cpp *.h
	$(CC) $(CCFLAGS) -I $(BOOST_ROOT) -c $<

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $<

clean:
	/bin/rm -f a.out $(OBJS) $(EXECS)
