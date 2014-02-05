#include "PlaceHolder.h"

Enemy::Enemy(){
	ME = Button(0,0,64,64);

	CurLoc = V2D(0,0);
	DrwLoc = V2D(0,0);
	PosLoc = V2D(0,0);
	PosDrw = V2D(0,0);
	HitPoints = 1;
	Dmg = 1;
	Armor = 0;
	Speed = 1;

	AccumulatedSpeed = 0;
	dead = false;

	Type = rand()%3;

	Scale = 1.0f;

	D3DXMatrixIdentity( &m_Matrix);
	D3DXMatrixIdentity(  &m_MatrixRot);
	D3DXMatrixIdentity(  &m_MatrixTran);
	D3DXMatrixIdentity(  &m_MatrixTran2);
	D3DXMatrixIdentity(  &m_MatrixScale);

}

void Enemy::SetLoc(LPDIRECT3DDEVICE9 device,Map &World){
	int x=0;
	int y=0;
	bool NotDone = true;
	while(NotDone){
		x = rand()%(int)World.GetSize().x;
		y = rand()%(int)World.GetSize().y;
		if(World.GetTile(x,y).isTraversable()){
			PosLoc = V2D(x,y);
			PosDrw = V2D(x,y);
			CurLoc = V2D(x,y);	
			DrwLoc = V2D(x,y);
			NotDone = false;
		}
	}
	ME.MoveButton(PosDrw.x*(64*Scale)-(DrwLoc.x*64),(PosDrw.y*(64*Scale)-(DrwLoc.y*64)));
}

void Enemy::Draw(ID3DXSprite* m_pD3DSprite,IDirect3DTexture9* MonTex,D3DXIMAGE_INFO m_imageInfo,V2D Disp){
			
	Lerp(DrwLoc.x,DrwLoc.y,Disp.x,Disp.y,0.01f);
	Lerp(PosDrw.x,PosDrw.y,PosLoc.x,PosLoc.y,0.01f);
	float x, y;
	x = (PosDrw.x*(64*Scale)-(DrwLoc.x*64));
	y = (PosDrw.y*(64*Scale)-(DrwLoc.y*64));
	ME.MoveButton(x,y);

	D3DXMatrixTranslation(&m_MatrixTran2, x ,y ,0);

	D3DXMatrixRotationZ(&m_MatrixRot, 0.0f);
	D3DXMatrixScaling(&m_MatrixScale, Scale, Scale, 0);
	m_Matrix = (m_MatrixScale*m_MatrixRot*m_MatrixTran2);

	m_pD3DSprite->SetTransform(&m_Matrix);
	m_pD3DSprite->Draw(MonTex, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
		&D3DXVECTOR3(0,0, 0.0f),
		D3DCOLOR_ARGB(255, 255, 255, 255));
	
	D3DXMATRIX temp;
	D3DXMatrixIdentity(&temp);
	m_pD3DSprite->SetTransform(&temp);
}

void Enemy::Lerp(float &inX, float &inY, float DestX, float DestY, float interp){
		inX = inX + interp*(DestX-inX);
		inY = inY + interp*(DestY-inY);
}

void Enemy::CreateEnemy(int level){
	int Skills = level * 3;
	int choice = 0;
	while(Skills){
		choice = rand()%4;
		switch(choice){
		case 0:
			Speed++;
			Skills--;
			break;
		case 1:
			Dmg++;
			Skills--;
			break;
		case 2:
			Armor++;
			Skills--;
			break;
		case 3:
			HitPoints++;
			Skills--;
			break;
		}
	}
	switch(Type){
	case 0:
		Armor *= 1.25f;
		HitPoints *= 1.25f;
		break;
	case 1:
		Dmg *= 1.5f;
		break;
	case 2:
		Speed *= 1.5f;
		break;
	}
}