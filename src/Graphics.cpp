#include <iostream>
#include "Graphics.h"
#include "Intersection.h"
#include "Vehicle.h"

std::vector<spot> Graphics::getTrafficObjectsAsSpots()
{
	std::vector<spot> spots {};
	
	// create overlay from all traffic objects
	for ( auto it : _trafficObjects )
	{
		double posx, posy;
		it->getPosition(posx, posy);
		
		if ( it->getType() == ObjectType::objectIntersection )
		{
			// cast object type from TrafficObject to Intersection
			std::shared_ptr<Intersection> intersection = std::dynamic_pointer_cast<Intersection>(it);
			
			auto &trafficLightColor = intersection->trafficLightIsGreen() == true ? *wxGREEN_BRUSH : *wxRED_BRUSH;
			spots.emplace_back(trafficLightColor, static_cast<int>(posx), static_cast<int>(posy), it->getID());
		}
		else if ( it->getType() == ObjectType::objectVehicle )
		{
			auto *brush = wxWHITE_BRUSH;
			
			auto state = dynamic_cast<Vehicle *>(it.get())->getState();
			
			switch (state)
			{
				case VehicleState::moving: brush = wxWHITE_BRUSH;
					break;
				case VehicleState::waitingForIntersection:brush = wxCYAN_BRUSH;
					break;
				case VehicleState::waitingForLight:brush = wxYELLOW_BRUSH;
					break;
			}
			
			spots.emplace_back(*brush, static_cast<int>(posx), static_cast<int>(posy), it->getID());
		}
	}
	
	return spots;
}

const std::string &Graphics::getBgFilename()
{
	return _bgFilename;
}
