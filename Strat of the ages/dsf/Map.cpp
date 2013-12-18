#include "Map.h"
#include <fstream>
#include <queue>
using namespace std;
#include <time.h>
#include "MapGen.h"



WorldMap::WorldMap(){
	srand(time(NULL));
	Province * Prov;
	for(int i = 0; i < 100; i++)\
		for(int j = 0; j < 100; j++){
			Prov = new Province(Land,j+i*100,1.0f,1.0f);
			Provinces[j+i*100] = Prov;}

	queue<MapGenTile,deque<MapGenTile>> Mapgen;
	for(int i = 0; i < Num_Landseed; i++)//push Land seeds
		Mapgen.push(MapGenTile(rand()%10000,Land));
	for(int i = 0; i < Num_Forestseed; i++)//push Forest seeds
		Mapgen.push(MapGenTile(rand()%10000,Forest));
	for(int i = 0; i < Num_Desertseed; i++)//push Desert seeds
		Mapgen.push(MapGenTile(rand()%10000,Desert));
	for(int i = 0; i < Num_Moutainseed; i++)//push moutain seeds
		Mapgen.push(MapGenTile(rand()%10000,Mountain));
	for(int i = 0; i < Num_Waterseed; i++)//push water seeds
		Mapgen.push(MapGenTile(rand()%10000,Water));

	while(!Mapgen.empty())//while map generator not done
	{
		if(!Provinces[Mapgen.front().ProvID]->Set){//just incase some stuff gets put on other stuff
			Provinces[Mapgen.front().ProvID]->mtype = Mapgen.front().Type;
			Provinces[Mapgen.front().ProvID]->Set = true;
			for(int i = 0; i < 6; i++)//push neighbors
			{
				if(Provinces[Mapgen.front().ProvID]->connections[i] != -1)
					if(!Provinces[Provinces[Mapgen.front().ProvID]->connections[i]]->Set)
					{
						Mapgen.push(MapGenTile(Provinces[Mapgen.front().ProvID]->connections[i],Mapgen.front().Type));
					}
			}
		}
		Mapgen.pop();
	}
	


}

Province& WorldMap::getProv(int i){
	return* Provinces[i];
}

WorldMap::~WorldMap(){
	for(int i = 0; i < 10000; i++)
		if(Provinces[i]){
			delete Provinces[i];
			Provinces[i] = 0;
		}
}