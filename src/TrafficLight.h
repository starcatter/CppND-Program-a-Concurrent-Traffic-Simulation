#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

enum class TrafficLightPhase
{
	red, green
};

// FP.3 Define a class „MessageQueue“ which has the public methods send and receive. 
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. 
// Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase. 
// Also, there should be an std::condition_variable as well as an std::mutex as private members.



template<class T>
class MessageQueue
{
public:
	T receive();
	
	void send( T &&msg );

private:
	std::deque<T> _queue;
	std::condition_variable _condition;
	std::mutex _mutex;
	
};


// FP.1 : Define a class „TrafficLight“ which is a child class of TrafficObject. 
// The class shall have the public methods „void waitForGreen()“ and „void simulate()“ 
// as well as „TrafficLightPhase getCurrentPhase()“, where TrafficLightPhase is an enum that 
// can be either „red“ or „green“. Also, add the private method „void cycleThroughPhases()“. 
// Furthermore, there shall be the private member _currentPhase which can take „red“ or „green“ as its value. 



class TrafficLight : public TrafficObject
{
	static int minimumCycleDurationSeconds;
	static int maximumCycleDurationSeconds;
public:
	// constructor / desctructor
	TrafficLight();
	
	virtual ~TrafficLight();
	
	// getters / setters
	TrafficLightPhase getCurrentPhase();
	
	// typical behaviour methods
	void waitForGreen();
	
	void simulate();

private:
	// helper to decide how long until light changes
	int pickCycleDurationMilliseconds();
	
	// typical behaviour methods
	
	[[noreturn]] void cycleThroughPhases();
	
	TrafficLightPhase _currentPhase = TrafficLightPhase::green;
	
	// FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase
	// and use it within the infinite loop to push each new TrafficLightPhase into it by calling
	// send in conjunction with move semantics.
	
	MessageQueue<TrafficLightPhase> _queue;
	std::condition_variable _condition;
	std::mutex _mutex;
	
	
};

#endif
