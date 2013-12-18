#pragma once
#include "province.h"
#include "Army.h"

enum {Land,Forest,Desert,Mountain,Water};

#define Num_Landseed 50
#define Num_Forestseed 25
#define Num_Desertseed 25
#define Num_Moutainseed 3
#define Num_Waterseed 15
#define Num_Nations 100

//Complete placeholder values
#define LandWeight 15
#define ForestWeight 20
#define DesertWeight 25
#define MountainWeight 60
#define WaterWeight 5
#define WaterLandWeight 60

class WorldMap{
	Province* Provinces[10000];
	
public:
	enum {Land,Forest,Desert,Mountain,Water};
	Province& getProv(int i);
	WorldMap();
	void Init(Army[]);
	~WorldMap();
};