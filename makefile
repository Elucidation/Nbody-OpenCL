CFLAGS = -std=c++0x -Wall
LIBS = -lOpenCL

test_cl1: test_cl1.cpp
	g++ $(CFLAGS) test_cl1.cpp -o test_cl1 $(LIBS)

clean:
	rm -rf *.o test_cl1
