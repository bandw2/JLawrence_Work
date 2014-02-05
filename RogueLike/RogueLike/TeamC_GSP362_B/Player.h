
#pragma once
#include "Armor.h"
#include "Item.h"
#include "RangedWeapon.h"
#include "Utility.h"
#include "Weapon.h"
#include "templatedArray.h"
#include "V2D.h"
#include <Windows.h>

struct Player
{

	Weapon* meleeWeapon;
	RangedWeapon* rangedWeapon;
	Armor* armor;
	Utility* utilityItem;
	void* inventory[5];
	int strength; //abilty to use heavy weapons
	int dexterity; //abilty to hit 
	int intellegence; //chance to do random stuff
	int speed; //abilty to take turns more often
	int hitpoints; //hp
	int skillPoints; //num skillpoints
	int maxInventorySize;
	int Ammo;
	V2D pos;
	int VisionDistance;
	int AccumulatedSpeed;
	/*Graphics texture;*/ //all textures will be 64x64 pixels

	//draw things
	float scaleFactor, rotationFactor;
	IDirect3DTexture9* texture;

	D3DXIMAGE_INFO m_imageInfo;
	D3DXMATRIX m_Matrix;
	D3DXMATRIX m_MatrixRot;
	D3DXMATRIX m_MatrixTran;
	D3DXMATRIX m_MatrixTran2;
	D3DXMATRIX m_MatrixScale;

	

	Player();
	~Player();

	void Init(LPDIRECT3DDEVICE9 device);
	//void Update();

	void MeleeAttack();
	void RangedAttack();
	void Use();
	void Move(int x, int y);


	void setTexture(LPDIRECT3DDEVICE9 device);
	void Draw(ID3DXSprite* m_pD3DSprite,D3DXIMAGE_INFO m_imageInfo, V2D Draw);

	

};