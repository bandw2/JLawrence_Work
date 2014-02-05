#include "templatedArray.h"
struct Tile
{
	int ID;
	int adjacency[4];

	enum{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	//texture;
};

class Floor
{
private:
	bool grid [10][10]; //map grid
	int numRooms;
	int floorNumber;
	tArray<Tile*> graph; //individual tiles
public:

	Floor();
	Floor(int number);
	~Floor();


	void setGrid(int x, int y, bool set);
	tArray<Tile> getGraph();

};