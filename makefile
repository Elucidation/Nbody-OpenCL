CFLAGS = -std=c++0x -Wall
LIBS = -lOpenCL

all: test_cl1 test_nocl1 test_cl2 test_cl3_2d

test_cl1: test_cl1.cpp
	g++ $(CFLAGS) test_cl1.cpp -o test_cl1 $(LIBS)

test_nocl1: test_nocl1.cpp
	g++ $(CFLAGS) test_nocl1.cpp -o test_nocl1 $(LIBS)

test_cl2: test_cl2.cpp
	g++ $(CFLAGS) test_cl2.cpp -o test_cl2 $(LIBS)

test_cl3_2d: test_cl3_2d.cpp
	g++ $(CFLAGS) test_cl3_2d.cpp -o test_cl3_2d $(LIBS)


clean:
	rm -rf *.o test_cl1 test_nocl1 test_cl2 test_cl3_2d
