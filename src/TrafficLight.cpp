#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */


template<typename T>
T MessageQueue<T>::receive()
{
	// FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait()
	// to wait for and receive new messages and pull them from the queue using move semantics.
	// The received object should then be returned by the receive function.
	std::unique_lock<std::mutex> lock(_mutex, std::defer_lock);
	
	_condition.wait(lock);
	
	auto &msg = _queue.front();
	_queue.pop_front();
	
	return std::move(msg);
}

template<typename T>
void MessageQueue<T>::send( T &&msg )
{
	// FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex>
	// as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_queue.emplace_back(msg);
	}
	
	_condition.notify_one();
}


/* Implementation of class "TrafficLight" */


int TrafficLight::minimumCycleDurationSeconds = 4;
int TrafficLight::maximumCycleDurationSeconds = 6;

TrafficLight::TrafficLight()
		: TrafficObject(ObjectType::objectIntersection), _currentPhase(TrafficLightPhase::red)
{
}

TrafficLight::~TrafficLight()
{

}

void TrafficLight::waitForGreen()
{
	// FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop
	// runs and repeatedly calls the receive function on the message queue.
	// Once it receives TrafficLightPhase::green, the method returns.
	while ( true )
	{
		auto light = _queue.receive();
		if ( light == TrafficLightPhase::green ){ return; }
	}
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _currentPhase;
}

int TrafficLight::pickCycleDurationMilliseconds()
{
	std::random_device rd;
	
	std::mt19937 eng(rd());
	// set _id based seed to increase chance of different results
	eng.seed(( std::chrono::system_clock::now().time_since_epoch().count() * _id ) / 1000);
	
	std::uniform_int_distribution<> distr(minimumCycleDurationSeconds * 1000, maximumCycleDurationSeconds * 1000);
	
	return distr(eng);
}

void TrafficLight::simulate()
{
	// FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class.
	threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

[[noreturn]] // virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
	// FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles
	// and toggles the current phase of the traffic light between red and green and sends an update method
	// to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds.
	// Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.
	
	std::chrono::time_point<std::chrono::system_clock> lastUpdate = std::chrono::system_clock::now();
	int cycleDuration = pickCycleDurationMilliseconds();
	
	while ( true )
	{
		// sleep at every iteration to reduce CPU usage
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		
		long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now() - lastUpdate).count();
		if ( timeSinceLastUpdate >= cycleDuration )
		{
			if ( _currentPhase == TrafficLightPhase::green )
			{
				{
					std::lock_guard<std::mutex> lock(_mutex);
					//std::cout << "TrafficLight #" << _id << "::_queue.send(TrafficLightPhase::red)" << std::endl;
					_currentPhase = TrafficLightPhase::red;
				}
				_queue.send(TrafficLightPhase::red);
			}
			else
			{
				{
					std::lock_guard<std::mutex> lock(_mutex);
					//std::cout << "TrafficLight #" << _id << "::_queue.send(TrafficLightPhase::green)" << std::endl;
					_currentPhase = TrafficLightPhase::green;
				}
				_queue.send(TrafficLightPhase::green);
			}
			
			// reset stop watch for next cycle
			cycleDuration = pickCycleDurationMilliseconds();
			lastUpdate = std::chrono::system_clock::now();
		}
	}
}
