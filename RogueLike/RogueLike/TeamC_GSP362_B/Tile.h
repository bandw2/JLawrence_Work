#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "V2D.h"
#include <Windows.h>
#include <iostream>
using namespace std;



//Justin Was here
struct Tile
{
	bool Traversable;//if it is yes, it is not a wall
	bool Visable;
	bool Vacant;
	int Neighbors[4];
	int Type;
	V2D Loc;
	V2D CurrentLoc;
	float Scale;
	bool Visited;
	int Distance;


	D3DXMATRIX m_Matrix;
	D3DXMATRIX m_MatrixRot;
	D3DXMATRIX m_MatrixTran;
	D3DXMATRIX m_MatrixTran2;
	D3DXMATRIX m_MatrixScale;
	




	D3DXIMAGE_INFO m_imageInfo;

	Tile();
	Tile(int my_x, int my_y, int x, int y, int type);
	bool SET(int my_x, int my_y, int x, int y, int type);//my_x-Y is the coords of the tile, x-y are the dimensions of the map, rand to randomly allocate the tile.
	bool isTraversable();//returns true if traversable
	bool isVisable();
	int GetType();
	void Draw(ID3DXSprite* m_pD3DSprite,D3DXIMAGE_INFO m_imageInfo, V2D Draw,IDirect3DTexture9* FloorTex[]);//draws itself.
	void setVisited(bool);
	V2D getLoc();
	void setDistance(int);
	void SetVisable(bool);
	void Lerp(float &inX, float &inY, float DestX, float DestY, float interp);
	int getDistance();
	bool getVisited();
	//	friend Entity;
};