
CFLAGS   = -W -Wall -O3 -std=c99
CXXFLAGS = -W -Wall -O3 -std=c++11

.PHONY : clean run

asi-test : asi-test.o libUSB.o
	$(CXX) $^ -lusb-1.0 libASICamera2_patched.a -lpthread -o $@

asi-test-RE : asi-test.o libUSB.o libASICamera2_ReverseEngineered.o
	$(CXX) $^ -lusb-1.0 -lpthread -o $@

asi-test.o : asi-test.cc

libUSB.o : libUSB.c

libASICamera2_ReverseEngineered.o : libASICamera2_ReverseEngineered.c

clean :
	$(RM) asi-test asi-test-RE asi-test.o libUSB.o libASICamera2_ReverseEngineered.o *~
