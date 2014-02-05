#include "RangedWeapon.h"

RangedWeapon::RangedWeapon()
:damage(1), range(3), fireRate(1){
	 Type = 0;
}

RangedWeapon::~RangedWeapon()
{
}

void RangedWeapon::Special()
{
}

void RangedWeapon::create(int floor)
{
	int points = generateRarity(floor);
	//damageType
	//roll for special
	Type = 1;
	if(points >0)
		IntReq= (points/2)+rand()%points;
	else
		IntReq= 0;

	range += rand()%4;
	while(points > 0)
	{
		int choose = rand() % 3;
		if(!choose)
		{
			damage++;
			points--;
		}
		else if(choose ==1)
		{
			if(points >= fireRate*4)
			{
				points-= fireRate*4;
				fireRate++;
			}
		}
		else
		{
			if(points >= 10*range)
			{
				points -= 10*range;
				range++;
			}
		}

	}
}