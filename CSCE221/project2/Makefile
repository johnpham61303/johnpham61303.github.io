all: output.out

mytest.out: mytest.o dtree.o
	g++ -g -Wall -std=c++11 mytest.o dtree.o -o mytest.out

output.out: driver.o dtree.o
	g++ -Wall -std=c++17 driver.o dtree.o -o output.out

driver.o: driver.cpp dtree.h
	g++ -Wall -std=c++17 -c driver.cpp

dtree.o: dtree.cpp dtree.h
	g++ -Wall -std=c++17 -c dtree.cpp

clean:
	rm -f *.o
	rm -f *.out
	rm -f *~ *.h.gch *#

run: output.out
	./output.out

val: output.out
	valgrind -s --track-origins=yes ./output.out

gdb: output.out
	gdb ./output.out

zip:
	zip -r project2.zip *.cpp *.h