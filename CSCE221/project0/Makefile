driver.out: driver.o AmazonTruck.o
	g++ -Wall -std=c++11 Driver.o AmazonTruck.o -o driver.out

driver.o: Driver.cpp
	g++ -Wall -std=c++11 -c Driver.cpp

AmazonTruck.o: AmazonTruck.cpp AmazonTruck.h
	g++ -Wall -std=c++11 -c AmazonTruck.cpp

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~*.hgch *#
	rm -f *~ *.h.gch *#

val:
	valgrind ./driver

run:
	./driver.out