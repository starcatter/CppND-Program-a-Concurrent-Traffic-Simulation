#ifndef VEHICLE_H
#define VEHICLE_H

#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Street;

class Intersection;

enum class VehicleState
{
	moving, waitingForIntersection, waitingForLight
};

class Vehicle : public TrafficObject, public std::enable_shared_from_this<Vehicle>
{
public:
	// constructor / desctructor
	Vehicle();
	
	// getters / setters
	void setCurrentStreet( std::shared_ptr<Street> street ){ _currStreet = street; };
	
	void setCurrentDestination( std::shared_ptr<Intersection> destination );
	
	void setState( VehicleState state );
	
	VehicleState getState();
	
	// typical behaviour methods
	void simulate();
	
	// miscellaneous
	std::shared_ptr<Vehicle> get_shared_this(){ return shared_from_this(); }

private:
	[[noreturn]] // typical behaviour methods
	void drive();
	
	std::shared_ptr<Street> _currStreet;            // street on which the vehicle is currently on
	std::shared_ptr<Intersection> _currDestination; // destination to which the vehicle is currently driving
	double _posStreet;                              // position on current street
	double _speed;                                  // ego speed in m/s
	
	VehicleState _state {VehicleState::moving};
	std::mutex _state_mtx;           // mutex for state updates
};

#endif
