a5q2: flight_planner.o flight.o
	g++ flight_planner.o flight.o -o a5q2
flight_planner.o: flight_planner.cc flight.h
	g++ -std=c++14 -c flight_planner.cc
flight.o: flight.cc flight.h
	g++ -std=c++14 -c flight.cc
