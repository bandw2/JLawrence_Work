#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "Map.h"
#include "V2D.h"
#include "Button.h"
#include <Windows.h>
#include <iostream>
using namespace std;

struct Enemy
{
	V2D PosLoc;
	V2D PosDrw;
	V2D DrwLoc;
	V2D CurLoc;

	int Type;

	int Speed;
	int Dmg;
	int Armor;
	int HitPoints;

	int AccumulatedSpeed;
	bool dead;

	float Scale;

	D3DXMATRIX m_Matrix;
	D3DXMATRIX m_MatrixRot;
	D3DXMATRIX m_MatrixTran;
	D3DXMATRIX m_MatrixTran2;
	D3DXMATRIX m_MatrixScale;

	Button ME;


	Enemy();
	void CreateEnemy(int level);
	void SetLoc(LPDIRECT3DDEVICE9 device,Map&);
	void Draw(ID3DXSprite* m_pD3DSprite,IDirect3DTexture9* MonTex ,D3DXIMAGE_INFO m_imageInfo,V2D Disp);
	void Move(Map World);
	void Lerp(float &inX, float &inY, float DestX, float DestY, float interp);
};