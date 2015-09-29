
CFLAGS   = -W -Wall -O3 -std=c99
CXXFLAGS = -W -Wall -O3 -std=c++11

.PHONY : clean default

default : asi-test-ORIG asi-test asi-test-RE asi-test-c-compatibility

asi-test-ORIG : asi-test.o asi-enum-strings.o
	$(CXX) $^ libASICamera2.a -lusb-1.0 -lpthread -o $@

asi-test : asi-test.o libUSB.o asi-enum-strings.o
	$(CXX) $^ -lusb-1.0 libASICamera2_patched.a -lpthread -o $@

asi-test-RE : asi-test.o libUSB.o libASICamera2_ReverseEngineered.o asi-enum-strings.o
	$(CXX) $^ -lusb-1.0 -lpthread -o $@

asi-test-c-compatibility : asi-test-c-compatibility.o
	$(CXX) $^ libASICamera2.a -lusb-1.0 -lpthread -o $@

asi-test.o : asi-test.cc asi-enum-strings.h

asi-test-c-compatibility.o : asi-test-c-compatibility.c

asi-enum-strings.o : asi-enum-strings.cc asi-enum-strings.h

libUSB.o : libUSB.c

libASICamera2_ReverseEngineered.o : libASICamera2_ReverseEngineered.c

libASICamera2_patched.a : libASICamera2.a

clean :
	$(RM) asi-test-ORIG asi-test-RE asi-test asi-test-c-compatibility
	$(RM) asi-test.o libUSB.o asi-test-c-compatibility.o libASICamera2_ReverseEngineered.o asi-enum-strings.o
	$(RM) usb_control_transfer.log
	$(RM) libASICamera2_patched.a
	$(RM) *~
