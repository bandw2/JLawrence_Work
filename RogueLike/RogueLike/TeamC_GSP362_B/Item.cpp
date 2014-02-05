#include "Item.h"
#include "templatedArray.h"


Item::Item()
{

}


void Item::Draw()
{

}

void Item::setPassives(int p[10])
{

}

void Item::unsetPassives(int p[10])
{

}

int Item::generateRarity(int floor)
{
	int num = rand() % 100;

	if(num == 0)
		return 5* floor;

	if(num < 10)
		return 4*floor;

	if(num < 30)
		return 3*floor;

	if(num < 60)
		return 2*floor;

	if(num<90)
		return floor;

	return .5*floor;
	
}