
#pragma once
#include "Item.h"

struct Utility : public Item
{
	int vision, heal, damageOnHit, splashRange, uses;
	Utility();
	~Utility();
	
	void Init();
	void Use();
	void Special();
	void Unspecial();

	void create(int floorLevel);

	enum types{
		MEDKIT,
		GRENADE,
		GLASSES
	};


};