#pragma once
#include "V2D.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

struct Item
{
	//texture
	int Type;
	int IntReq;

	V2D pos;
	IDirect3DTexture9* texture;

	Item();

	void Draw();
	void setPassives(int p[10]);
	void unsetPassives(int p[10]);

	int generateRarity(int floor);

	enum dTypes
	{
		RENDING,
		SLASHING,
		ENERGY,
		IMPACT,
		NONE
	};

	enum itemTypes
	{
		MELEE,
		RANGED,
		UTILITY
	};

	enum stats
	{
		STRENGTH,
		DEXTERITY,
		INTELLEGENCE,
		SPEED
	};




};