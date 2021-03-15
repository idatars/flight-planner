#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>

class Flight {
	int duration;
	int price;
	std::string name;
	std::string from;
	std::string to;
public:
	Flight(int d, int p, const std::string& name, const std::string& from, const std::string& to);
	std::string getFrom();
	std::string getTo();
	int getDuration();
	int getPrice();
	std::string getName();
};

class FlightPlan {
	friend void findRoute(const std::string& dest, const std::string& orig, std::vector<std::string> visited,
		std::map<std::string, std::vector<std::shared_ptr<Flight>>>& graph,
		std::shared_ptr<FlightPlan> sofar,
		std::vector<std::shared_ptr<FlightPlan>>& routes);
	std::vector<std::shared_ptr<Flight>> steps;
	int duration = 0;
	int price = 0;
	int pain = 0;

public:
	FlightPlan();
	//std::vector<std::shared_ptr<Flight>> steps;
	std::vector<std::shared_ptr<Flight>> getSteps();
	int getDuration();
	int getPrice();
	int getPain();
};

class FlightPlanner {
	std::map<std::string, std::vector<std::shared_ptr<Flight>>> data;

public:
	FlightPlanner();
	void addAirport(const std::string& name);
	void addFlight(const std::string& from, const std::string& to, int duration, int price, const std::string& name);
	std::vector<std::shared_ptr<FlightPlan>> planTrip(const std::string& from, const std::string& to);
};

class FlightNameException {
};

void findRoute(const std::string& dest, const std::string& orig, std::vector<std::string> visited,
	std::map<std::string, std::vector<std::shared_ptr<Flight>>>& graph,
	std::shared_ptr<FlightPlan> sofar,
	std::vector<std::shared_ptr<FlightPlan>>& routes);
