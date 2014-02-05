
#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

struct Card
{
	short Suit;
	short Number;
	int Value;
	IDirect3DTexture9* CardText;
	IDirect3DTexture9* CardBackText;
	float Rot;
	float Scale;
	float mXloc;
	float mYloc;
	float mX;
	float mY;
	bool Visable;
	RECT Me;

	D3DXMATRIX m_Matrix;
	D3DXMATRIX m_MatrixRot;
	D3DXMATRIX m_MatrixTran;
	D3DXMATRIX m_MatrixTran2;
	D3DXMATRIX m_MatrixScale;

	void lerp(float x, float &xloc, float y, float &yloc, float inter){
		xloc = xloc + inter*(x-xloc);
		yloc = yloc + inter*(y-yloc);
	}

	void Draw(ID3DXSprite* m_pD3DSprite,D3DXIMAGE_INFO m_imageInfo){
		lerp(mX,mXloc,mY,mYloc,0.005f);


		D3DXMatrixTranslation(&m_MatrixTran2,mXloc ,mYloc ,0);
		D3DXMatrixRotationZ(&m_MatrixRot, Rot);
		D3DXMatrixScaling(&m_MatrixScale, Scale, Scale, 0);
		m_Matrix = (m_MatrixScale*m_MatrixRot*m_MatrixTran2);


		m_pD3DSprite->SetTransform(&m_Matrix);
		if(Visable){
			m_pD3DSprite->Draw(CardText, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
				&D3DXVECTOR3(0,0, 0.0f),
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		else
		{	
			m_pD3DSprite->Draw(CardBackText, 0,   &D3DXVECTOR3((m_imageInfo.Width * 0.5f), (m_imageInfo.Height * 0.5f), 0.0f), 
				&D3DXVECTOR3(0,0, 0.0f),
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		D3DXMATRIX temp;
		D3DXMatrixIdentity(&temp);
		m_pD3DSprite->SetTransform(&temp);

	}

	bool operator==(const Card other){//checking if the cards number/face are the same 
		if(Number == other.Number)
			return true;
		else
			return false;
	}

	bool operator<(const Card other){
		if(Value < other.Value)
			return true;
		else
			return false;
	}

	bool operator>(const Card other){
		if(Value > other.Value)
			return true;
		else
			return false;
	}

	bool IsCursorOnMe(long aX, long aY){
		if(aX > Me.left && aX < Me.right && aY > Me.top && aY < Me.bottom){
			return true;
		}
		return false;
	}

	void CardInit(IDirect3DDevice9* device,int value,D3DXIMAGE_INFO	&m_imageInfo){
		Value = value;
		Number = (value/4)+1;
		Suit = (value%4);
		Rot = 0;
		Scale = 1;
		mX = mY = mXloc = mYloc = -50;
		Visable = false;
		Me.top = mY;
		Me.left = mX;
		Me.bottom = mY+96;
		Me.right = mX+72;


		D3DXCreateTextureFromFileEx(device, L"CardBack.png", 0, 0, 0, 0,
			D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
			D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
			&m_imageInfo, 0, &CardBackText);

		switch(value){
		case 0:
			D3DXCreateTextureFromFileEx(device, L"3-Spade.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 1:
			D3DXCreateTextureFromFileEx(device, L"3-club.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 2:
			D3DXCreateTextureFromFileEx(device, L"3-diamond.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 3:
			D3DXCreateTextureFromFileEx(device, L"3-Heart.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 4:
			D3DXCreateTextureFromFileEx(device, L"4-Spade.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 5:
			D3DXCreateTextureFromFileEx(device, L"4-club.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 6:
			D3DXCreateTextureFromFileEx(device, L"4-diamond.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 7:
			D3DXCreateTextureFromFileEx(device, L"4-Heart.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 8:
			D3DXCreateTextureFromFileEx(device, L"5-Spade.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 9:
			D3DXCreateTextureFromFileEx(device, L"5-club.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 10:
			D3DXCreateTextureFromFileEx(device, L"5-diamond.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 11:
			D3DXCreateTextureFromFileEx(device, L"5-Heart.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 12:
			D3DXCreateTextureFromFileEx(device, L"6-Spade.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 13:
			D3DXCreateTextureFromFileEx(device, L"6-club.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 14:
			D3DXCreateTextureFromFileEx(device, L"6-diamond.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 15:
			D3DXCreateTextureFromFileEx(device, L"6-Heart.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 16:
			D3DXCreateTextureFromFileEx(device, L"7-Spade.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 17:
			D3DXCreateTextureFromFileEx(device, L"7-club.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 18:
			D3DXCreateTextureFromFileEx(device, L"7-diamond.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 19:
			D3DXCreateTextureFromFileEx(device, L"7-Heart.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 20:
			D3DXCreateTextureFromFileEx(device, L"8-Spade.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 21:
			D3DXCreateTextureFromFileEx(device, L"8-club.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 22:
			D3DXCreateTextureFromFileEx(device, L"8-diamond.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 23:
			D3DXCreateTextureFromFileEx(device, L"8-Heart.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 24:
			D3DXCreateTextureFromFileEx(device, L"9-Spade.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 25:
			D3DXCreateTextureFromFileEx(device, L"9-club.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 26:
			D3DXCreateTextureFromFileEx(device, L"9-diamond.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 27:
			D3DXCreateTextureFromFileEx(device, L"9-Heart.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 28:
			D3DXCreateTextureFromFileEx(device, L"10-Spade.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 29:
			D3DXCreateTextureFromFileEx(device, L"10-club.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 30:
			D3DXCreateTextureFromFileEx(device, L"10-diamond.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 31:
			D3DXCreateTextureFromFileEx(device, L"10-Heart.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 32:
			D3DXCreateTextureFromFileEx(device, L"j-Spade.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 33:
			D3DXCreateTextureFromFileEx(device, L"j-club.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 34:
			D3DXCreateTextureFromFileEx(device, L"j-diamond.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 35:
			D3DXCreateTextureFromFileEx(device, L"j-Heart.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 36:
			D3DXCreateTextureFromFileEx(device, L"q-Spade.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 37:
			D3DXCreateTextureFromFileEx(device, L"q-club.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 38:
			D3DXCreateTextureFromFileEx(device, L"q-diamond.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 39:
			D3DXCreateTextureFromFileEx(device, L"q-Heart.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 40:
			D3DXCreateTextureFromFileEx(device, L"k-Spade.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 41:
			D3DXCreateTextureFromFileEx(device, L"k-club.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 42:
			D3DXCreateTextureFromFileEx(device, L"k-diamond.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 43:
			D3DXCreateTextureFromFileEx(device, L"k-Heart.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 44:
			D3DXCreateTextureFromFileEx(device, L"A-Spade.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 45:
			D3DXCreateTextureFromFileEx(device, L"A-club.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 46:
			D3DXCreateTextureFromFileEx(device, L"A-diamond.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 47:
			D3DXCreateTextureFromFileEx(device, L"A-Heart.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 48:
			D3DXCreateTextureFromFileEx(device, L"2-Spade.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 49:
			D3DXCreateTextureFromFileEx(device, L"2-club.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 50:
			D3DXCreateTextureFromFileEx(device, L"2-diamond.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		case 51:
			D3DXCreateTextureFromFileEx(device, L"2-Heart.png", 0, 0, 0, 0,
				D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
				&m_imageInfo, 0, &CardText);
			break;
		}
	}
};

