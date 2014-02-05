#pragma once
#include "Item.h"

struct Armor : public Item
{
	int value;
	int statBoost[4];
	int resists[4];

	Armor();
	Armor(int val);
	~Armor();

	void Init();

	void create(int floor);

	enum armorStrength
	{
		LIGHT,
		MEDIUM,
		HEAVY,
		HAZARDSUIT
	};



};