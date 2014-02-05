#pragma once
#include "Item.h"

struct RangedWeapon : public Item
{
	int damage, range, fireRate;
	int special[5];


	RangedWeapon();
	~RangedWeapon();

	void Init();
	void Draw();
	void Attack();
	void Special();
	void Unspecial();

	void create(int floor);

	enum specials
	{
		POISON,
		ACCURATE, //roll 2x for hit
		DEVESTATING, //roll 2x for damage
		EXPLODING_SHOT,
		VISION
	};


};