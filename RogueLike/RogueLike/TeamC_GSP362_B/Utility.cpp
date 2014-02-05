#include "Utility.h"

Utility::Utility()
	:heal(0), damageOnHit(0), splashRange(0), vision(0),uses(1)
{
}

Utility::~Utility()
{

}

void Utility::Special()
{

}

void Utility::create(int floor)
{
	int points = generateRarity(floor);
	int type = rand()%3;
	if(points >0)
		IntReq= (points/2)+rand()%points;
	else
		IntReq= 0;
	switch(type)
	{
	case MEDKIT: 
		Type = 3;
		while(points > 0)
		{
			int choose = rand() % 2;
			if(!choose)
			{
			heal++;
			points--;
			}
			else
			{
				if(points>=10)
				{
					uses++;
					points-=10;
				}
			}

		}
		break;
	case GRENADE: 
		Type = 4;
		while(points > 0)
		{
			int choose = rand() % 3;
			if(choose == 0)
			{
			damageOnHit++;
			points--;
			}
			else if(choose==1)
			{
				if(points>=5)
				{
					uses++;
					points-=5;
				}
			}
			else if(choose ==2)
			{
				if(points>=splashRange*5)
				{
					points -= splashRange*5;
					splashRange++;
				}
			}

		}
		break;
	case GLASSES: 
		Type = 5;
		while(points>=vision * 20)
		{
			points-=vision * 20;
			vision++;
		}
		break;
	}
}