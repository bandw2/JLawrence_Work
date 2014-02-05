#include "Weapon.h"

Weapon::Weapon()
	:damage(1),attackSpeed(1)
{
	Type = 0;
	for(int i = 0; i <5; i++)
		special[5]=0;
}

Weapon::~Weapon()
{

}

//void Weapon::Init()
//{
//	//initialize all passives to 0
//	for(int i = 0; i < sizeof(special)/sizeof(special[0]); ++i)
//		special[i] = 0;
//}
//
//void Weapon::Draw()
//{
//
//}

void Weapon::Special()
{
}

void Weapon::Unspecial()
{

}

void Weapon::create(int floor)
{
	int points = generateRarity(floor);
	//damageType
	//roll for special
	if(points >0)
		IntReq= (points/2)+rand()%points;
	else
		IntReq= 0;
	while(points > 0)
	{
		int choose = rand() % 2;
		if(!choose)
		{
			damage++;
			points--;
		}
		else
		{
			if(points >= attackSpeed*4)
			{
				points-= attackSpeed*4;
				attackSpeed++;
			}
		}
	}
}


