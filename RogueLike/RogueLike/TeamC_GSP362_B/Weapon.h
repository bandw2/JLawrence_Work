#pragma once
#include "Item.h"

struct Weapon : public Item
{
	int damage, attackSpeed;
	int special[5];

	Weapon();
	~Weapon();

	void Special();
	void Unspecial();

	void create(int floor);

	enum specials
	{
		POISON,
		QUAKE,
		REACH,
		WHIRLWIND,
		DEVESTATING
	};


};