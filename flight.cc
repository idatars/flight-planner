#include "flight.h"
#include <cmath>

Flight::Flight(int d, int p, const std::string& name, const std::string& from, const std::string& to) {
	duration = d;
	price = p;
	this->name = name;
	this->from = from;
	this->to = to;
}
std::string Flight::getFrom() { return from; }
std::string Flight::getTo() { return to; }
std::string Flight::getName() { return name; }
int Flight::getDuration() { return duration; }
int Flight::getPrice() { return price; }

FlightPlan::FlightPlan() { }
std::vector<std::shared_ptr<Flight>> FlightPlan::getSteps() { return steps; }
int FlightPlan::getDuration() { return duration; }
int FlightPlan::getPrice() { return price; }
int FlightPlan::getPain() { return pow(1.2, duration) * price; }

FlightPlanner::FlightPlanner() { }

void FlightPlanner::addAirport(const std::string& name) {
	for (std::map<std::string, std::vector<std::shared_ptr<Flight>>>::iterator it = data.begin(); it != data.end(); ++it) {
		if ((*it).first == name) throw FlightNameException();
	}
	std::vector<std::shared_ptr<Flight>> h;
	data.emplace(name, h);
}

bool member(const std::string& s, std::map<std::string, std::vector<std::shared_ptr<Flight>>> map) {
	for (std::map<std::string, std::vector<std::shared_ptr<Flight>>>::iterator it = map.begin(); it != map.end(); ++it) {
		if ((*it).first == s) return true;
	}
	return false;
}

bool member(const std::string& s, std::vector<std::shared_ptr<Flight>> lof) {
	for (std::vector<std::shared_ptr<Flight>>::iterator it = lof.begin(); it != lof.end(); ++it) {
		if ((*it)->getName() == s) return true;
	}
	return false;
}

void FlightPlanner::addFlight(const std::string& from, const std::string& to, int duration, int price, const std::string& name) {
	if (!member(from, data) || !member(to, data)) throw FlightNameException();
	if (member(name, data[from]) || member(name, data[to])) throw FlightNameException();
	data[from].emplace_back(new Flight(duration, price, name, from, to));
	data[to].emplace_back(new Flight(duration, price, name, to, from));
}

bool member(std::vector<std::string> los, const std::string& s) {
	for (std::vector<std::string>::iterator it = los.begin(); it != los.end(); ++it) {
		if (*it == s) return true;
	}
	return false;
}

void findRoute(const std::string& dest, const std::string& orig, std::vector<std::string> visited,
	std::map<std::string, std::vector<std::shared_ptr<Flight>>>& graph,
	std::shared_ptr<FlightPlan> sofar,
	std::vector<std::shared_ptr<FlightPlan>>& routes) {
	int origsize = routes.size();
	if (orig == dest) {
		sofar->pain = sofar->getPain();
		if (!routes.empty()) {
			std::vector<std::shared_ptr<FlightPlan>>::iterator before = routes.begin();
			std::vector<std::shared_ptr<FlightPlan>>::iterator it = routes.begin();
			std::vector<std::shared_ptr<FlightPlan>>::iterator tmp = routes.begin();
			while (it != routes.end()) {
				if (it == routes.begin() && sofar->getPain() <= (*it)->getPain()) {
					routes.insert(it, sofar);
					break;
				}
				else if (it == routes.begin()) {
					++it;
					if (it == routes.end()) break;
				}
				else {
					before = tmp;
					tmp = it;
					++it;
					if (it == routes.end()) break;
					if (sofar->getPain() >= (*before)->getPain() && sofar->getPain() <= (*it)->getPain()) {
						routes.insert(it, sofar);
						break;
					}
				}
			}
			if (origsize == routes.size() && sofar->getPain() <= routes.back()->getPain()) routes.insert(routes.end(), sofar);
			if (origsize == routes.size() && sofar->getPain() > routes.back()->getPain()) routes.push_back(sofar);
		}
		else routes.push_back(sofar);
	}
	else {
		visited.push_back(orig);
		if (graph[orig].empty()) {

		}
		else {
			for (std::vector<std::shared_ptr<Flight>>::iterator it = graph[orig].begin(); it != graph[orig].end(); ++it) {
				if (!member(visited, (*it).get()->getTo())) {
					std::shared_ptr<FlightPlan> copy(new FlightPlan);
					copy->steps = sofar->steps;
					copy->duration = sofar->duration;
					copy->price = sofar->price;
					copy->steps.push_back(*it);
					copy->duration += (*it).get()->getDuration();
					copy->price += (*it).get()->getPrice();
					findRoute(dest, (*it).get()->getTo(), visited, graph, copy, routes);
				}
			}
		}
	}
}

std::vector<std::shared_ptr<FlightPlan>> FlightPlanner::planTrip(const std::string& from, const std::string& to) {
	if (!member(from, data) || !member(to, data)) throw FlightNameException();
	std::vector<std::shared_ptr<FlightPlan>> garbage;
	std::vector<std::string> visited;
	std::shared_ptr<FlightPlan> sofar (new FlightPlan);
	findRoute(to, from, visited, data, sofar, garbage);

	return garbage;
}

/*void findRouteList(std::vector<std::shared_ptr<Flight>> lof, const std::string& dest,
	std::vector<std::string> visited,
	std::map<std::string, std::vector<std::shared_ptr<Flight>>>& graph,
	std::shared_ptr<FlightPlan> sofar,
	std::vector<std::shared_ptr<FlightPlan>>& routes) {
	if (lof.empty()) {

	}
	else {
		for (std::vector<std::shared_ptr<Flight>>::iterator it = lof.begin(); it != lof.end(); ++it) {
			if (!member(visited, (*it).get()->getFrom())) {
				std::shared_ptr<FlightPlan> copy = sofar;
				copy->steps.push_back(*it);
				findRoute(dest, (*it).get()->getTo(), visited, graph, copy, routes);
			}
		}
	}
}*/

