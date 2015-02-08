
CFLAGS   = -W -Wall -O3
CXXFLAGS = -W -Wall -O3 -std=c++11

.PHONY : clean run

asi-test : asi-test.o libUSB.o
	$(CXX) $^ -lusb-1.0 libASICamera2.a -lpthread -o $@

asi-test.o : asi-test.cc

libUSB.o : libUSB.c

clean :
	$(RM) asi-test *~ *.o
