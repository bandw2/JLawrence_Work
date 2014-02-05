#include "item.h"
#include "V2D.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

struct Loot{
	V2D PosLoc;
	V2D PosDrw;
	void* m_Item;

	float Scale;

	D3DXMATRIX m_Matrix;
	D3DXMATRIX m_MatrixRot;
	D3DXMATRIX m_MatrixTran;
	D3DXMATRIX m_MatrixTran2;
	D3DXMATRIX m_MatrixScale;

	Loot(){
		Scale = 1.0f;
	}

		void Place(Map& World){
		int x=0;
		int y=0;
		bool NotDone = true;
		while(NotDone){
			x = rand()%(int)World.GetSize().x;
			y = rand()%(int)World.GetSize().y;
			if(World.GetTile(x,y).isTraversable()){
				PosLoc = V2D(x,y);
				PosDrw = V2D(x,y);
				NotDone = false;
			}
		}

	}

	void Draw(ID3DXSprite* m_pD3DSprite,D3DXIMAGE_INFO m_imageInfo, V2D Draw, IDirect3DTexture9* Tex){	
	Lerp(PosDrw.x,PosDrw.y,Draw.x,Draw.y,0.001f);


	D3DXMatrixTranslation(&m_MatrixTran2,(PosLoc.x*(64*Scale)-(PosDrw.x*64)) ,(PosLoc.y*(64*Scale)-(PosDrw.y*64)) ,0);
	D3DXMatrixRotationZ(&m_MatrixRot, 0.0f);
	D3DXMatrixScaling(&m_MatrixScale, Scale, Scale, 0);
	m_Matrix = (m_MatrixScale*m_MatrixRot*m_MatrixTran2);

	
	m_pD3DSprite->SetTransform(&m_Matrix);
		m_pD3DSprite->Draw(Tex, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
			&D3DXVECTOR3(0,0, 0.0f),
			D3DCOLOR_ARGB(255, 255, 255, 255));
	D3DXMATRIX temp;
	D3DXMatrixIdentity(&temp);
	m_pD3DSprite->SetTransform(&temp);

	}

	void Lerp(float &inX, float &inY, float DestX, float DestY, float interp){
		inX = inX + interp*(DestX-inX);
		inY = inY + interp*(DestY-inY);
	}
};