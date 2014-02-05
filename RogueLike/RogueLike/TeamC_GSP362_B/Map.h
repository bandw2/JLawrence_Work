#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "Tile.h"
#include "V2D.h"
#include <random>
#include <time.h>
//Justin Was here
class Map
{
	//Tile* FloorPlan;
	Tile FloorPlan[3201];
	V2D Size;
	IDirect3DTexture9* FloorTex[2];
	D3DXIMAGE_INFO m_imageInfo;
public:
	Map();
	Tile& GetTile(int,int);
	Tile& GetTile(int);
	void Draw(ID3DXSprite* m_pD3DSprite,D3DXIMAGE_INFO m_imageInfo, V2D Draw);
	void Reset();
	void ResetVision();
	void Init(LPDIRECT3DDEVICE9 device);
	void NextLevel();
	void Shutdown();
	V2D GetSize();
};
