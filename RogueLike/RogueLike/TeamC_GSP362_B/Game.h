#include "Player.h"
#include "Entity.h"
#include "Enemy.h"
#include "Item.h"
#include "Armor.h"
#include "RangedWeapon.h"
#include "Weapon.h"
#include "templatedArray.h"
#include "Floor.h"
#include "Map.h"

class Game
{
private:
	Player player;
	tArray<Item> allTheItems;
	tArray<Enemy> theEnemies;
	Floor* currentFloor;
	bool isRunning;
	//Graphics stuff
	Map World;
public:

	Game();
	~Game();

	void init();
	void input(char in);
	void update();
	void draw();
	
	//just in case we need them
	Player getPlayer();
	tArray<Item> getItems();
	tArray<Enemy> getEnemies();
	//getGraphics()
	Floor* floor();









};