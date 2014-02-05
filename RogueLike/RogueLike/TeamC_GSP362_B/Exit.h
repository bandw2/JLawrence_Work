#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


#include "V2D.h"
#include "Map.h"
struct Exit{
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

	Exit();
	void Init(Map& world);
	void Draw(ID3DXSprite* m_pD3DSprite,D3DXIMAGE_INFO m_imageInfo, V2D Draw, IDirect3DTexture9* Exit);
	void Lerp(float &inX, float &inY, float DestX, float DestY, float interp);
};
