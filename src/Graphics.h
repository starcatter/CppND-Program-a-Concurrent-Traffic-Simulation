#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <vector>

#include "TrafficObject.h"

#include "MyApp.hpp"

class Graphics
{
public:
	// constructor / desctructor
	
	// getters / setters
	void setBgFilename( std::string filename ){ _bgFilename = filename; }
	
	const std::string &getBgFilename();
	
	// typical behaviour methods
	void setTrafficObjects( std::vector<std::shared_ptr<TrafficObject>> &trafficObjects ){ _trafficObjects = trafficObjects; };
	
	std::vector<spot> getTrafficObjectsAsSpots();

private:
	// member variables
	std::vector<std::shared_ptr<TrafficObject>> _trafficObjects;
	std::string _bgFilename;
};

#endif
