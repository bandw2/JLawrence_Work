#include "Exit.h"




Exit::Exit(){
	Scale = 1;
	Loc = V2D(0,0);
	CurrentLoc = V2D(0,0);

}

void Exit::Draw(ID3DXSprite* m_pD3DSprite,D3DXIMAGE_INFO m_imageInfo, V2D Draw, IDirect3DTexture9* Exit){	
	Lerp(CurrentLoc.x,CurrentLoc.y,Draw.x,Draw.y,0.01f);


	D3DXMatrixTranslation(&m_MatrixTran2,(Loc.x*(64*Scale)-(CurrentLoc.x*64)) ,(Loc.y*(64*Scale)-(CurrentLoc.y*64)) ,0);
	D3DXMatrixRotationZ(&m_MatrixRot, 0.0f);
	D3DXMatrixScaling(&m_MatrixScale, Scale, Scale, 0);
	m_Matrix = (m_MatrixScale*m_MatrixRot*m_MatrixTran2);

	
	m_pD3DSprite->SetTransform(&m_Matrix);
	m_pD3DSprite->Draw(Exit, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
		&D3DXVECTOR3(0,0, 0.0f),
		D3DCOLOR_ARGB(255, 255, 255, 255));
	
	D3DXMATRIX temp;
	D3DXMatrixIdentity(&temp);
	m_pD3DSprite->SetTransform(&temp);

}

void Exit::Lerp(float &inX, float &inY, float DestX, float DestY, float interp){
		inX = inX + interp*(DestX-inX);
		inY = inY + interp*(DestY-inY);
}

void Exit::Init(Map& World){
	int x=0;
	int y=0;
	bool NotDone = true;
	while(NotDone){
		x = rand()%(int)World.GetSize().x;
		y = rand()%(int)World.GetSize().y;
		if(World.GetTile(x,y).isTraversable()){
			Loc = V2D(x,y);
			CurrentLoc = V2D(x,y);
			NotDone = false;
		}
	}
}