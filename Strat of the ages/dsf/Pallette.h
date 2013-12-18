
#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include "Button.h"
#include "Vector.h"
#include "Loc.h"
#include "Army.h"

struct pallette
{
	static const long LocCount = 10000;
	IDirect3DTexture9** m_Textures;
	float Rot;
	float Scale;
	Loc Locs[LocCount];
	bool Visable;
	RECT Me;
	Button m_Button[LocCount];
	ID3DXFont* m_Font;
	int NumDraw;

	double DeltaX;
	double DeltaY;

	D3DXMATRIX m_Matrix;
	D3DXMATRIX m_MatrixRot;
	D3DXMATRIX m_MatrixTran;
	D3DXMATRIX m_MatrixTran2;
	D3DXMATRIX m_MatrixScale;

	void lerp(float x, float &xloc, float y, float &yloc, float inter){
		xloc = xloc + inter*(x-xloc);
		yloc = yloc + inter*(y-yloc);
	}

	pallette(){
		float Rot = 0;
		float Scale = 1;
		bool Visable = 1;
		NumDraw = 0;


		 m_Matrix = D3DXMATRIX();
		 m_MatrixRot = D3DXMATRIX();
		 m_MatrixTran = D3DXMATRIX();
		 m_MatrixTran2 = D3DXMATRIX();
		 m_MatrixScale = D3DXMATRIX();
	}

	bool IsCursorOnMe(long aX, long aY){
		if(aX > Me.left && aX < Me.right && aY > Me.top && aY < Me.bottom){
			return true;
		}
		return false;
	}

	int IsCursorOnWho(long aX, long aY){
		for(int i = 0; i < LocCount; i++){
			m_Button[i].X = Locs[i].drwmx;
			m_Button[i].Y = Locs[i].drwmy;
			m_Button[i].Width = 33;
			m_Button[i].Height = 33;
			m_Button[i].CalcRECT();
			if(m_Button[i].IsCursorOnMe(aX,aY))
				return i;
		}
		return -1;

	}

	void DrawArmy(Army PH[],ID3DXSprite* m_pD3DSprite,D3DXIMAGE_INFO m_imageInfo,IDirect3DTexture9* a_Textures, float Rot = 0,float Scalex = 1,float Scaley = 1){
		for(int i = 0; i < 100; i++){
			if(this->IsCursorOnMe(Locs[PH[i].getProvID()].drwmx,Locs[PH[i].getProvID()].drwmy)){
				D3DXMatrixTranslation(&m_MatrixTran2,Locs[PH[i].getProvID()].drwmx ,Locs[PH[i].getProvID()].drwmy ,0);
				D3DXMatrixRotationZ(&m_MatrixRot, Rot);
				D3DXMatrixScaling(&m_MatrixScale, Scalex, Scaley, 0);
				m_Matrix = (m_MatrixScale*m_MatrixRot*m_MatrixTran2);

				m_pD3DSprite->SetTransform(&m_Matrix);
				if(PH[i].getNation())
					m_pD3DSprite->Draw(a_Textures,0,&D3DXVECTOR3(0,0,0),&D3DXVECTOR3(0,0,0),PH[i].getNation()->m_Flag);
				else
					m_pD3DSprite->Draw(a_Textures,0,&D3DXVECTOR3(0,0,0),&D3DXVECTOR3(0,0,0),D3DCOLOR_ARGB(125,255,255,255));
			}
		}
	}


	void Draw(ID3DXSprite* m_pD3DSprite,D3DXIMAGE_INFO m_imageInfo,IDirect3DTexture9* a_Textures,D3DCOLOR a_color = D3DCOLOR_ARGB(255,100,120,220), float Rot = 0,float Scalex = 1,float Scaley = 1){

		static unsigned int i = 0;
		Loc loc = Locs[i];
		lerp(loc.mx+DeltaX,loc.drwmx,loc.my+DeltaY,loc.drwmy,0.005f);
		Locs[i] = loc;
		if(this->IsCursorOnMe(loc.drwmx,loc.drwmy)){
			D3DXMatrixTranslation(&m_MatrixTran2,loc.drwmx ,loc.drwmy ,0);
			D3DXMatrixRotationZ(&m_MatrixRot, Rot);
			D3DXMatrixScaling(&m_MatrixScale, Scalex, Scaley, 0);
			m_Matrix = (m_MatrixScale*m_MatrixRot*m_MatrixTran2);

			m_pD3DSprite->SetTransform(&m_Matrix);
			m_pD3DSprite->Draw(a_Textures,0,&D3DXVECTOR3(0,0,0),&D3DXVECTOR3(0,0,0),a_color);
		}
		
		i++;
		if(i >= NumDraw)
			i = 0;
	}

};