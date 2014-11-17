CFLAGS = -std=c++0x -Wall
LIBS = -lOpenCL

all: test_cl1 test_nocl1

test_cl1: test_cl1.cpp
	g++ $(CFLAGS) test_cl1.cpp -o test_cl1 $(LIBS)

test_nocl1: test_nocl1.cpp
	g++ $(CFLAGS) test_nocl1.cpp -o test_nocl1 $(LIBS)


clean:
	rm -rf *.o test_cl1 test_nocl1
