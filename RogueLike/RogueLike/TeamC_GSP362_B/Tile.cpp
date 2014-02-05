#include "Tile.h"
#include <Windows.h>



	/*bool Traversable;
	char Look;
	int Neighbors[4];*/
Tile::Tile()
{
	Scale = 1;
	Vacant = true;

	Traversable = true;
	Visable = false;
	for(int i = 0; i < 4 ; i++)
	{
		Neighbors[0] = NULL;
	}
	Loc = V2D(0,0);
	CurrentLoc = V2D(0,0);
}
Tile::Tile(int my_x, int my_y, int x, int y, int type)
{
	//Sets whether the tile is an X or O based on whether or not it is traversable.
	if(type%10 < 9)
	{	Traversable = true;}
	else
	{	Traversable = false;	}
	Loc = V2D(my_x,my_y);
	CurrentLoc = V2D(my_x,my_y);


	if(my_y%y == 0)
		Neighbors[0] = NULL;
	else
		Neighbors[0] = ((my_y-1)*x)+my_x;



	if(my_x%x == x-1)
		Neighbors[1] = NULL;
	else
		Neighbors[1] = my_x+1;


	if(my_y%y == y-1)
		Neighbors[2] = NULL;
	else
		Neighbors[2] = ((my_y+1)*x)+my_x;


	if(my_x%x == 0)
		Neighbors[4] = NULL;
	else
		Neighbors[4] = my_x-1;


}
bool Tile::SET(int my_x, int my_y, int x, int y, int type)
{

	Type = type;
	if(type == 0)
	{	Traversable = true;}
	else if (type == 1)
	{	Traversable = false;}	
	


	Loc = V2D(my_x,my_y);
	CurrentLoc = V2D(my_x,my_y);

	if(my_y%y == 0)
		Neighbors[0] = NULL;
	else
		Neighbors[0] = ((my_y-1)*x)+my_x;
	


	if(my_x%x == x-1)
		Neighbors[1] = NULL;
	else
		Neighbors[1] = ((my_y)*x)+my_x+1;



	if(my_y%y == y-1)
		Neighbors[2] = NULL;
	else
		Neighbors[2] = ((my_y+1)*x)+my_x;



	if(my_x%x == 0)
		Neighbors[3] = NULL;
	else
		Neighbors[3] = ((my_y)*x)+my_x-1;
	


	return true;
}





bool Tile::isTraversable()
{
	return Traversable;
}
bool Tile::isVisable()
{
	return Visable;
}
int Tile::GetType()
{
	return Type;
}


void Tile::Draw(ID3DXSprite* m_pD3DSprite,D3DXIMAGE_INFO m_imageInfo, V2D Draw, IDirect3DTexture9* FloorTex[])
{		
	Lerp(CurrentLoc.x,CurrentLoc.y,Draw.x,Draw.y,0.01f);


	D3DXMatrixTranslation(&m_MatrixTran2,(Loc.x*(64*Scale)-(CurrentLoc.x*64)) ,(Loc.y*(64*Scale)-(CurrentLoc.y*64)) ,0);
	D3DXMatrixRotationZ(&m_MatrixRot, 0.0f);
	D3DXMatrixScaling(&m_MatrixScale, Scale, Scale, 0);
	m_Matrix = (m_MatrixScale*m_MatrixRot*m_MatrixTran2);

	
	m_pD3DSprite->SetTransform(&m_Matrix);
	if(Traversable){
		m_pD3DSprite->Draw(FloorTex[0], 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
			&D3DXVECTOR3(0,0, 0.0f),
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else{
		m_pD3DSprite->Draw(FloorTex[1], 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
			&D3DXVECTOR3(0,0, 0.0f),
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	D3DXMATRIX temp;
	D3DXMatrixIdentity(&temp);
	m_pD3DSprite->SetTransform(&temp);
}

void Tile::setVisited(bool visit)
{
	Visited = visit;
}
void Tile::SetVisable(bool visit)
{
	Visable = visit;
}
V2D Tile::getLoc()
{
	return Loc;
}

void Tile::setDistance(int other)
{
	Distance = other;
}

void Tile::Lerp(float &inX, float &inY, float DestX, float DestY, float interp){
		inX = inX + interp*(DestX-inX);
		inY = inY + interp*(DestY-inY);
}

int Tile::getDistance(){
	return Distance;
}
bool Tile::getVisited(){
	return Visited;
}