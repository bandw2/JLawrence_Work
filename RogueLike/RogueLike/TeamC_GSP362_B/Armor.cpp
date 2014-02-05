#include "Armor.h"

Armor::Armor()
	:value(0)
{
	Type = 2;
	for(int i = 0; i < 4; ++i)
	{
		statBoost[i]=0;
		resists[i]=0;
	}
}

Armor::Armor(int val)
	:value(val)
{
	for(int i = 0; i < 4; ++i)
		resists[i]=0;
}

Armor::~Armor()
{
	//shutdown texture
}

void Armor::Init()
{
	//init texture
}

void Armor::create( int floor)
{
	int choose;
	int points = 0;
	points = generateRarity(floor);
	if(points >0)
		IntReq= (points/2)+rand()%points;
	else
		IntReq= 0;
	int type = rand()%4;
	switch(type)
	{
	case LIGHT: 
		while(points > 0)
		{
		choose = rand() % 3;
			if(!choose)
			{
				if(points>=2);
				{
					value++;
					points-=2;
				}
			}
			else if(choose ==1)
			{
				statBoost[SPEED]++;
				points--;
			}
			else
			{
				if(points>=resists[SLASHING]*resists[SLASHING])
				{
					points -= resists[SLASHING]*resists[SLASHING];
					resists[SLASHING]++;
				}
			}
		}
		break;
	case MEDIUM:
		while(points > 0)
		{
		choose = rand() % 3;
			if(!choose)
			{
					value++;
					points--;
			}
			else if(choose==1)
			{
				statBoost[DEXTERITY]++;
				points--;
			}
			else
			{
			if(points>=resists[RENDING]*resists[RENDING])
				{
					points -= resists[RENDING]*resists[RENDING];
					resists[RENDING]++;
				}
			}
		}
		break;
	case HEAVY: 
		while(points > 0)
		{
		choose = rand() % 3;
			if(!choose)
			{
					value+=2;
					points--;
			}
			else if(choose==1)
			{
				if(points >=2)
				{
				statBoost[STRENGTH]++;
				points-=2;
				}
			}
			else
			{
				if(points>=resists[IMPACT]*resists[IMPACT])
				{
					points -= resists[IMPACT]*resists[IMPACT];
					resists[IMPACT]++;
				}
			}
		}
		break;
	case HAZARDSUIT: 
		while(points > 0)
		{
		choose = rand() % 6;
			if(!choose)
			{
				if(points>=4);
				{
					value++;
					points-=4;
				}
			}
			else if(choose==1)
			{
				statBoost[SPEED]++;
				points--;
			}
			else if(choose==2)
			{
				if(points>=resists[IMPACT]*resists[IMPACT])
				{
					points -= resists[IMPACT]*resists[IMPACT];
					resists[IMPACT]++;
				}
			}
			else if(choose ==3)
			{
				if(points>=resists[RENDING]*resists[RENDING])
				{
					points -= resists[RENDING]*resists[RENDING];
					resists[RENDING]++;
				}
			}
			else if(choose == 4)
			{
				if(points>=resists[RENDING]*resists[RENDING])
				{
					points -= resists[RENDING]*resists[RENDING];
					resists[RENDING]++;
				}
			}

			else if(choose == 5)
			{
				
				if(points>=resists[ENERGY]*2)
				{
					points -= resists[ENERGY]*2;
					resists[ENERGY]++;
				}
			}
		}
		break;
	}
}