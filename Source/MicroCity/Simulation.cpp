#include "Game.h"
#include "Connectivity.h"
#include "Draw.h"

enum SimulationSteps
{
	SimulateBuildings = 0,
	SimulatePower = MAX_BUILDINGS,
	SimulatePopulation,
	SimulateRefreshTiles,
	SimulateNextMonth
};

#define SIM_INCREMENT_POP_THRESHOLD 20				// Score must be more than this to grow
#define SIM_DECREMENT_POP_THRESHOLD -30				// Score must be less than this to shrink

#define AVERAGE_POPULATION_DENSITY 8
#define SIM_AVERAGING_STRENGTH 0
#define SIM_EMPLOYMENT_BOOST 10
#define SIM_UNEMPLOYMENT_PENALTY 100
#define SIM_INDUSTRIAL_OPPORTUNITY_BOOST 10
#define SIM_COMMERCIAL_OPPORTUNITY_BOOST 10
#define SIM_LOCAL_BUILDING_DISTANCE 32
#define SIM_LOCAL_BUILDING_INFLUENCE 5
#define SIM_STADIUM_BOOST 100
#define SIM_PARK_BOOST 10
#define SIM_MAX_CRIME 50
#define SIM_RANDOM_STRENGTH_MASK 31
#define SIM_POLLUTION_INFLUENCE 1
#define SIM_MAX_POLLUTION 50
#define SIM_INDUSTRIAL_BASE_POLLUTION 8
#define SIM_TRAFFIC_BASE_POLLUTION 8
#define SIM_POWERPLANT_BASE_POLLUTION 32
#define SIM_HEAVY_TRAFFIC_THRESHOLD 12

uint8_t GetNumRoadConnections(Building* building)
{
	const BuildingInfo* info = GetBuildingInfo(building->type);
	uint8_t width = pgm_read_byte(&info->width);
	uint8_t height = pgm_read_byte(&info->height);
	uint8_t count = 0;

	if(building->y > 0)
	{
		for(uint8_t i = 0; i < width; i++)
		{
			if(GetConnections(building->x + i, building->y - 1) & RoadMask)
			{
				count++;
			}
		}
	}
	if(building->y + height < MAP_HEIGHT)
	{
		for(uint8_t i = 0; i < width; i++)
		{
			if(GetConnections(building->x + i, building->y + height) & RoadMask)
			{
				count++;
			}
		}
	}
	if(building->x > 0)
	{
		for(uint8_t i = 0; i < height; i++)
		{
			if(GetConnections(building->x - 1, building->y + i) & RoadMask)
			{
				count++;
			}
		}
	}
	if(building->x + width < MAP_WIDTH)
	{
		for(uint8_t i = 0; i < height; i++)
		{
			if(GetConnections(building->x + width, building->y + i) & RoadMask)
			{
				count++;
			}
		}
	}
		
	return count;
}

uint8_t GetManhattanDistance(Building* a, Building* b)
{
	uint8_t x = a->x > b->x ? a->x - b->x : b->x - a->x;
	uint8_t y = a->y > b->y ? a->y - b->y : b->y - a->y;
	return x + y;
}

void DoBudget()
{
	// Collect taxes
	int totalPopulation = (State.residentialPopulation + State.commercialPopulation + State.residentialPopulation) * POPULATION_MULTIPLIER;
	int taxesCollected = (totalPopulation * State.taxRate) / 100;

	State.money += taxesCollected;

	// Count police and fire departments for costing
	uint8_t numPoliceDept = 0;
	uint8_t numFireDept = 0;

	for (int n = 0; n < MAX_BUILDINGS; n++)
	{
		if (State.buildings[n].type == PoliceDept)
		{
			numPoliceDept++;
		}
		else if (State.buildings[n].type == FireDept)
		{
			numFireDept++;
		}
	}

	State.money -= FIRE_AND_POLICE_MAINTENANCE_COST * numFireDept;
	State.money -= FIRE_AND_POLICE_MAINTENANCE_COST * numPoliceDept;

	// Count road tiles for cost of road maintenance
	int numRoadTiles = 0;
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			if (GetConnections(x, y) & RoadMask)
				numRoadTiles++;
		}
	}

	int roadCost = (numRoadTiles * ROAD_MAINTENANCE_COST) / 100;
	State.money -= roadCost;

#ifdef _WIN32
	printf("Budget for %d:\n", State.year + 1899);
	printf("Population: %d\n", totalPopulation);
	printf("Taxes collected: $%d\n", taxesCollected);
	printf("Police cost: %d x $%d = $%d\n", numPoliceDept, FIRE_AND_POLICE_MAINTENANCE_COST, FIRE_AND_POLICE_MAINTENANCE_COST * numPoliceDept);
	printf("Fire cost: %d x $%d = $%d\n", numFireDept, FIRE_AND_POLICE_MAINTENANCE_COST, FIRE_AND_POLICE_MAINTENANCE_COST * numFireDept);
	printf("Road maintenance: %d tiles = $%d\n", numRoadTiles, roadCost);
#endif

}

void SimulateBuilding(Building* building)
{
	if (building->type == Residential || building->type == Commercial || building->type == Industrial)
	{
		int8_t populationDensityChange = 0;

		if (building->hasPower)
		{
			int score = 0;
			
			// random effect
			score += (GetRand() & SIM_RANDOM_STRENGTH_MASK) - (SIM_RANDOM_STRENGTH_MASK / 2);
			
			// tend towards average population density
			score += (AVERAGE_POPULATION_DENSITY - building->populationDensity) * SIM_AVERAGING_STRENGTH;
			
			// general population effect
			switch(building->type)
			{
				case Residential:
				if(State.residentialPopulation < State.industrialPopulation)
				{
					score += SIM_EMPLOYMENT_BOOST;
				}
				else if(State.residentialPopulation > State.industrialPopulation + State.commercialPopulation)
				{
					score -= SIM_UNEMPLOYMENT_PENALTY;
				}
				break;
				case Industrial:
				if(State.industrialPopulation < State.residentialPopulation || State.industrialPopulation < State.commercialPopulation)
				{
					score += SIM_INDUSTRIAL_OPPORTUNITY_BOOST;
				}
				break;
				case Commercial:
				if(State.commercialPopulation < State.residentialPopulation || State.commercialPopulation < State.industrialPopulation)
				{
					score += SIM_COMMERCIAL_OPPORTUNITY_BOOST;
				}
				break;
			}
			
			// If at least 3 road tiles are adjacent then assume that it is connected to the road network
			bool isRoadConnected = GetNumRoadConnections(building) >= 3;
			
			uint8_t closestPoliceStationDistance = 24;
			int16_t pollution = 0;
			
			// influence from local buildings
			if(isRoadConnected)
			{
				for(int n = 0; n < MAX_BUILDINGS; n++)
				{
					Building* otherBuilding = &State.buildings[n];
					
					if(building != otherBuilding && otherBuilding->type && (otherBuilding->hasPower || otherBuilding->type == Park))
					{
						uint8_t distance = GetManhattanDistance(building, otherBuilding);
						
						if(otherBuilding->type == PoliceDept && distance < closestPoliceStationDistance)
						{
							closestPoliceStationDistance = distance;
						}
						
						int buildingPollution = 0;
						
						if(otherBuilding->type == Industrial)
						{
							buildingPollution = SIM_INDUSTRIAL_BASE_POLLUTION + otherBuilding->populationDensity - distance;
						}
						else if(otherBuilding->type == Powerplant)
						{
							buildingPollution = SIM_POWERPLANT_BASE_POLLUTION - distance;
						}
						else if(otherBuilding->heavyTraffic)
						{
							buildingPollution = SIM_TRAFFIC_BASE_POLLUTION - distance;
						}
						
						if(buildingPollution > 0)
							pollution += buildingPollution;
						
						if(distance <= SIM_LOCAL_BUILDING_DISTANCE && GetNumRoadConnections(otherBuilding) >= 3)
						{
							switch(otherBuilding->type)
							{
								case Industrial:
								if(otherBuilding->populationDensity >= building->populationDensity && (building->type == Residential || building->type == Commercial))
								{
									score += SIM_LOCAL_BUILDING_INFLUENCE;
								}
								break;
								case Residential:
								if(otherBuilding->populationDensity >= building->populationDensity && (building->type == Commercial || building->type == Industrial))
								{
									score += SIM_LOCAL_BUILDING_INFLUENCE;
								}
								break;
								case Commercial:
								if(otherBuilding->populationDensity >= building->populationDensity && building->type == Residential)
								{
									score += SIM_LOCAL_BUILDING_INFLUENCE;
								}
								break;
								case Stadium:
								if(building->type == Residential || building->type == Commercial)
								{
									score += SIM_STADIUM_BOOST;
								}
								break;
								case Park:
								if(building->type == Residential)
								{
									score += SIM_PARK_BOOST;
								}
								break;
								default:
								break;
							}
						}
					}
				}
			}
			
			// negative effect from pollution
			if (building->type == Residential)
			{
				if (pollution > SIM_MAX_POLLUTION)
					pollution = SIM_MAX_POLLUTION;
				score -= pollution * SIM_POLLUTION_INFLUENCE;
#if _WIN32
//				printf("Pollution: %d\n", pollution * SIM_POLLUTION_INFLUENCE);
#endif
			}
			
			// simulate crime based on how far the closest police station is and how populated the area is
			int crime = (building->populationDensity * (closestPoliceStationDistance - 16));
			if(crime > SIM_MAX_CRIME)
			{
				crime = SIM_MAX_CRIME;
			}
			if(crime > 0)
			{
				score -= crime;
			}
			
			// increase or decrease population density based on score
			if (building->populationDensity < MAX_POPULATION_DENSITY && score >= SIM_INCREMENT_POP_THRESHOLD)
			{
				populationDensityChange = 1;
			}
			else if(building->populationDensity > 0 && score <= SIM_DECREMENT_POP_THRESHOLD)
			{
				populationDensityChange = -1;
			}
			
			building->heavyTraffic = building->populationDensity > SIM_HEAVY_TRAFFIC_THRESHOLD;

			if(populationDensityChange != 0)
			{
				RefreshBuildingTiles(building);
			}
		}
		else
		{
			building->heavyTraffic = false;
			if (building->populationDensity > 0)
			{
				populationDensityChange = -1;
			}
		}

		building->populationDensity += populationDensityChange;
		switch (building->type)
		{
		case Residential:
			State.residentialPopulation += populationDensityChange;
			break;
		case Industrial:
			State.industrialPopulation += populationDensityChange;
			break;
		case Commercial:
			State.commercialPopulation += populationDensityChange;
			break;
		}
	}
}

void CountPopulation()
{
	State.residentialPopulation = State.industrialPopulation = State.commercialPopulation = 0;

	for (int n = 0; n < MAX_BUILDINGS; n++)
	{
		switch (State.buildings[n].type)
		{
		case Residential:
			State.residentialPopulation += State.buildings[n].populationDensity;
			break;
		case Industrial:
			State.industrialPopulation += State.buildings[n].populationDensity;
			break;
		case Commercial:
			State.commercialPopulation += State.buildings[n].populationDensity;
			break;
		default:
			break;
		}
	}
}

void Simulate()
{
	if (State.simulationStep < MAX_BUILDINGS)
	{
		SimulateBuilding(&State.buildings[State.simulationStep]);
	}
	else switch (State.simulationStep)
	{
	case SimulatePower:
		CalculatePowerConnectivity();
		break;
	case SimulateRefreshTiles:
		ResetVisibleTileCache();
		break;
	case SimulatePopulation:
		CountPopulation();
		break;
	case SimulateNextMonth:
	{
		State.simulationStep = 0;
		State.month++;
		if (State.month >= 12)
		{
			State.month = 0;
			State.year++;

			DoBudget();
		}
	}
	return;
	}

	State.simulationStep++;
}