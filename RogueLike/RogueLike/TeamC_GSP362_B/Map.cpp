#include <fstream>
#include "Map.h"
#include "PathHelper.h"

/*
Justin Lawrence 
was here
*/

Map::Map(){
}


Tile& Map::GetTile(int x,int y)
{
	return FloorPlan[x+(y*(int)Size.x)];
}
Tile& Map::GetTile(int x)
{
	return FloorPlan[x];
}
/*
0 1 2 3 4 5 6 7 8 9 
10111213






*/


void Map::Draw(ID3DXSprite* m_pD3DSprite,D3DXIMAGE_INFO m_imageInfo, V2D Draw){

	for(int i = 0; i < Size.x*Size.y; ++i)
	{
		if(FloorPlan[i].isVisable()){
			FloorPlan[i].Draw(m_pD3DSprite,m_imageInfo, Draw, FloorTex);
		}
	}
}

void Map::Reset()
{
	for(int i = 0; i < 3201 ; ++i)
	{
		FloorPlan[i].setVisited(false);
	}
}
void Map::ResetVision()
{
	for(int i = 0; i < 3201 ; ++i)
	{
		FloorPlan[i].SetVisable(false);
	}
}

void Map::Init(LPDIRECT3DDEVICE9 device){
	
	
	D3DXCreateTextureFromFileEx(device, L"Floor.png", 0, 0, 0, 0,
	D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
	D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
	&m_imageInfo, 0, &FloorTex[0]);

	D3DXCreateTextureFromFileEx(device, L"Wall1.png", 0, 0, 0, 0,
	D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
	D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
	&m_imageInfo, 0, &FloorTex[1]);
	
	int BacktrackCounter = 0;
	int k = 0;
	srand( time(NULL));
	Size = V2D(80,40);
	int size_i = (Size.x * Size.y);
	int Size_map;
	int Map_Hld;
	int Rand_Hld;
	Path Paths [8][4];
	//FloorPlan = new Tile[(Size.x * Size.y)];

	fstream FILE;
	fstream DEBUG;
	Tile Set[15][100];
	FILE.open("Map.txt", fstream::in);
	DEBUG.open("DEBUG.txt", fstream::out, fstream::trunc);
	FILE >> Size_map;



	for(int i =0; i < 10; i++)
	{
		for(int j =0; j <100;j++)
		{
			FILE >> Map_Hld;
			Set[i][j].SET(j%10, j/10,10,10,Map_Hld);
		}
	}
	for(int j=0; j < 8; j++){//move down 10
		for(int i =0; i < 4; i++){//move left 10
			Rand_Hld = rand();//pick random set
			for(int k=((i*10)*Size.x)+(j*10),l =0;l < 100;k++,l++){
				FloorPlan[k].SET(k%(int)Size.x, k/(int)Size.x,(int)Size.x,(int)Size.y,Set[Rand_Hld%Size_map][l].GetType());
				if(l%10 == 9)
					k += Size.x-10; 
			}
		}
	}
	/*
	0(0,0)	1(10,0)	2(20,0)	3(30,0)	4(40,0)	5(50,0)	6(60,0)	7(70,0)

	8(0,10)	9	10	11	12	13	14	15

	16	17	18	19	20	21	22	23

	24	25	26	27	28	29	30	31
	*/

	for(int i = 0; i < Size.x ; i++)
	{
		FloorPlan[i].SET(i%(int)Size.x, i/(int)Size.x,(int)Size.x,(int)Size.y,1);
	}

	for(int i = 0; i < Size.y ; i++)
	{
		k = k+(Size.x-1);
		FloorPlan[k].SET(k%(int)Size.x, k/(int)Size.x,(int)Size.x,(int)Size.y,1);
		k++;
		FloorPlan[k].SET(k%(int)Size.x, k/(int)Size.x,(int)Size.x,(int)Size.y,1);
	}

	for(int i1 = (Size.x * (Size.y-1)); i1 < size_i ; i1++)
	{
		FloorPlan[i1].SET(i1%(int)Size.x, i1/Size.x,Size.x,Size.y,1);
	}
	int Sel[4];
	int SelSize=4;
	Sel[0] =0;
	Sel[1] =1;
	Sel[2] =2;
	Sel[3] =3;
	//0 - up, 1 - left, 2 - down, 3 - down, 4 been here
	int rooms=0,i=rand()%8, j=rand()%4;
	while( rooms <= 32) 
	{
		if(SelSize == 0)
		{
			BacktrackCounter++;
			if(BacktrackCounter > 1000){
				break;
			}
			DEBUG << "x,y:" << i << " " << j << " Backtrack:";
			if(Paths[i][j].path[4] == false)
				rooms++;
			Paths[i][j].path[4] = true;
			int tempi=i,tempj=j;
			i = Paths[tempi][tempj].x;
			j = Paths[tempi][tempj].y;
			DEBUG << i << " " << j << " " << rooms << endl;
			SelSize=4;
			Sel[0] =0;
			Sel[1] =1;
			Sel[2] =2;
			Sel[3] =3;
		}
		k = Sel[rand()%SelSize];
		switch(k){
		case 0:
			if(i-1 >= 0 && Paths[i-1][j].path[4] == false){
				DEBUG << "x,y:" << i << " " << j << " Left:" ;
				Paths[i][j].path[4] = true;
				Paths[i][j].path[0] = true;
				rooms++;
				Paths[i-1][j].x =i;
				Paths[i-1][j].y =j;

				Paths[i-1][j].path[2] = true;
				i--;
				DEBUG << i << " " << j << " " << rooms << endl;
				SelSize=4;
				Sel[0] =0;
				Sel[1] =1;
				Sel[2] =2;
				Sel[3] =3;
			}
			else{
				for(int i =0; i < SelSize;i++)
				{
					if(Sel[i] == 0)
					{
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						SelSize--;
					}
				}
			}
			break;
		case 1:
			if((j > 0) && (Paths[i][j-1].path[4] == false)){
				DEBUG << "x,y:" << i << " " << j << " Up:" ;

				Paths[i][j].path[4] = true;
				Paths[i][j].path[1] = true;
				rooms++;
				Paths[i][j-1].x =i;
				Paths[i][j-1].y =j;

				Paths[i][j-1].path[3] = true;
				j--;

				DEBUG << i << " " << j << " " << rooms << endl;
				SelSize=4;
				Sel[0] =0;
				Sel[1] =1;
				Sel[2] =2;
				Sel[3] =3;
			}
			else{
				for(int i =0; i < SelSize;i++)
				{
					if(Sel[i] == 1)
					{
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						SelSize--;
					}
				}
			}
			break;
		case 2:
			if(i+1 < 8 && Paths[i+1][j].path[4] == false){
				DEBUG << "x,y:" << i << " " << j << " Right:" ;

				Paths[i][j].path[4] = true;
				Paths[i][j].path[2] = true;
				rooms++;
				Paths[i+1][j].x =i;
				Paths[i+1][j].y =j;

				Paths[i+1][j].path[0] = true;
				i++;
				DEBUG << i << " " << j << " " << rooms << endl;
				SelSize=4;
				Sel[0] =0;
				Sel[1] =1;
				Sel[2] =2;
				Sel[3] =3;
			}
			else{
				for(int i =0; i < SelSize;i++)
				{
					if(Sel[i] == 2)
					{
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						SelSize--;
					}
				}
			}
			break;
		case 3:
			if(j+1 < 4 && Paths[i][j+1].path[4] == false){
				DEBUG << "x,y:" << i << " " << j << " Down:" ;

				Paths[i][j].path[4] = true;
				Paths[i][j].path[3] = true;
				rooms++;
				Paths[i][j+1].x =i;
				Paths[i][j+1].y =j;

				Paths[i][j+1].path[1] = true;
				j++;
				DEBUG << i << " " << j << " " << rooms << endl;
				SelSize=4;
				Sel[0] =0;
				Sel[1] =1;
				Sel[2] =2;
				Sel[3] =3;
			}
			else{
				for(int i =0; i < SelSize;i++)
				{
					if(Sel[i] == 3)
					{
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						SelSize--;
					}
				}
			}
			break;
		}
	}

	//ok now i'm making random paths
	for(int l =0,x,y; l < 16;l++)
	{
		x = rand()%8;
		y = rand()%4;
		SelSize=4;
		Sel[0] =0;
		Sel[1] =1;
		Sel[2] =2;
		Sel[3] =3;
		for(; SelSize > 0; SelSize--){
			k = Sel[rand()%SelSize];
			switch(k){
			case 0:
				if(x-1 >= 0 && Paths[x][y].path[0] == false){

					Paths[x][y].path[4] = true;
					Paths[x][y].path[0] = true;
					Paths[x-1][y].x =i;
					Paths[x-1][y].y =j;

					Paths[x-1][y].path[2] = true;
					SelSize--;
				}
				else{
					SelSize--;
					SelSize--;
					SelSize--;
				}
				break;
			case 1:
				if((y > 0) && (Paths[x][j-1].path[1] == false)){

					Paths[x][y].path[4] = true;
					Paths[x][y].path[1] = true;
					Paths[x][y-1].x =i;
					Paths[x][y-1].y =j;

					Paths[x][y-1].path[3] = true;
					SelSize--;
				}
				else{
					SelSize--;
					SelSize--;
					SelSize--;
				}
				break;
			case 2:
				if(x+1 < 8 && Paths[i+1][y].path[2] == false){
					Paths[x][y].path[4] = true;
					Paths[x][y].path[2] = true;
					Paths[x+1][y].x =i;
					Paths[x+1][y].y =j;

					Paths[x+1][y].path[0] = true;
					SelSize--;
				}
				else{
					SelSize--;
					SelSize--;
					SelSize--;
				}
				break;
			case 3:
				if(y+1 < 4 && Paths[x][j+1].path[3] == false){

					Paths[x][y].path[4] = true;
					Paths[x][y].path[3] = true;
					Paths[x][y+1].x =i;
					Paths[x][y+1].y =j;

					Paths[x][y+1].path[1] = true;
					SelSize--;
				}
				else{
					SelSize--;
					SelSize--;
					SelSize--;
				}
				break;
			}
		}
	}

	Paths[0][0].path[2] = true;
	Paths[0][0].path[3] = true;


	for(int i =0; i < 8; i++)
		for(int j =0; j <4; j++)
		{
			int k;
			k=((j*10)*Size.x)+(i*10);
			if(Paths[i][j].path[1] == true)
			{
				k += 5;
				FloorPlan[k].SET(k%(int)Size.x, k/Size.x,Size.x,Size.y,0);
				k=((j*10)*Size.x)+(i*10);
			}
			if(Paths[i][j].path[0] == true)
			{
				k+= (5*Size.x);
				FloorPlan[k].SET(k%(int)Size.x, k/Size.x,Size.x,Size.y,0);
				k=((j*10)*Size.x)+(i*10);
			}
			if(Paths[i][j].path[3] == true)
			{
				k+= (10*Size.x)+5;
				FloorPlan[k].SET(k%(int)Size.x, k/Size.x,Size.x,Size.y,0);
				k=((j*10)*Size.x)+(i*10);
			}
			if(Paths[i][j].path[2] == true)
			{
				k+= (5*Size.x)+9;
				FloorPlan[k].SET(k%(int)Size.x, k/Size.x,Size.x,Size.y,0);
				k=((j*10)*Size.x)+(i*10);
			}
		}


}

void Map::Shutdown(){
	for(int i = 0; i < 3201; i++){
		if(FloorTex[0])
		FloorTex[0]->Release();
		FloorTex[0]=0;
		
		if(FloorTex[1])
		FloorTex[1]->Release();
		FloorTex[1]=0;
	}
}

V2D Map::GetSize(){
	return Size;
}

void Map::NextLevel(){
	int BacktrackCounter = 0;
	int k = 0;
	srand( time(NULL));
	Size = V2D(80,40);
	int size_i = (Size.x * Size.y);
	int Size_map;
	int Map_Hld;
	int Rand_Hld;
	Path Paths [8][4];
	//FloorPlan = new Tile[(Size.x * Size.y)];

	fstream FILE;
	fstream DEBUG;
	Tile Set[15][100];
	FILE.open("Map.txt", fstream::in);
	DEBUG.open("DEBUG.txt", fstream::out, fstream::trunc);
	FILE >> Size_map;



	for(int i =0; i < 10; i++)
	{
		for(int j =0; j <100;j++)
		{
			FILE >> Map_Hld;
			Set[i][j].SET(j%10, j/10,10,10,Map_Hld);
		}
	}
	for(int j=0; j < 8; j++){//move down 10
		for(int i =0; i < 4; i++){//move left 10
			Rand_Hld = rand();//pick random set
			for(int k=((i*10)*Size.x)+(j*10),l =0;l < 100;k++,l++){
				FloorPlan[k].SET(k%(int)Size.x, k/(int)Size.x,(int)Size.x,(int)Size.y,Set[Rand_Hld%Size_map][l].GetType());
				if(l%10 == 9)
					k += Size.x-10; 
			}
		}
	}
	/*
	0(0,0)	1(10,0)	2(20,0)	3(30,0)	4(40,0)	5(50,0)	6(60,0)	7(70,0)

	8(0,10)	9	10	11	12	13	14	15

	16	17	18	19	20	21	22	23

	24	25	26	27	28	29	30	31
	*/

	for(int i = 0; i < Size.x ; i++)
	{
		FloorPlan[i].SET(i%(int)Size.x, i/(int)Size.x,(int)Size.x,(int)Size.y,1);
	}

	for(int i = 0; i < Size.y ; i++)
	{
		k = k+(Size.x-1);
		FloorPlan[k].SET(k%(int)Size.x, k/(int)Size.x,(int)Size.x,(int)Size.y,1);
		k++;
		FloorPlan[k].SET(k%(int)Size.x, k/(int)Size.x,(int)Size.x,(int)Size.y,1);
	}

	for(int i1 = (Size.x * (Size.y-1)); i1 < size_i ; i1++)
	{
		FloorPlan[i1].SET(i1%(int)Size.x, i1/Size.x,Size.x,Size.y,1);
	}
	int Sel[4];
	int SelSize=4;
	Sel[0] =0;
	Sel[1] =1;
	Sel[2] =2;
	Sel[3] =3;
	//0 - up, 1 - left, 2 - down, 3 - down, 4 been here
	int rooms=0,i=rand()%8, j=rand()%4;
	while( rooms <= 32) 
	{
		if(SelSize == 0)
		{
			BacktrackCounter++;
			if(BacktrackCounter > 1000){
				break;
			}
			DEBUG << "x,y:" << i << " " << j << " Backtrack:";
			if(Paths[i][j].path[4] == false)
				rooms++;
			Paths[i][j].path[4] = true;
			int tempi=i,tempj=j;
			i = Paths[tempi][tempj].x;
			j = Paths[tempi][tempj].y;
			DEBUG << i << " " << j << " " << rooms << endl;
			SelSize=4;
			Sel[0] =0;
			Sel[1] =1;
			Sel[2] =2;
			Sel[3] =3;
		}
		k = Sel[rand()%SelSize];
		switch(k){
		case 0:
			if(i-1 >= 0 && Paths[i-1][j].path[4] == false){
				DEBUG << "x,y:" << i << " " << j << " Left:" ;
				Paths[i][j].path[4] = true;
				Paths[i][j].path[0] = true;
				rooms++;
				Paths[i-1][j].x =i;
				Paths[i-1][j].y =j;

				Paths[i-1][j].path[2] = true;
				i--;
				DEBUG << i << " " << j << " " << rooms << endl;
				SelSize=4;
				Sel[0] =0;
				Sel[1] =1;
				Sel[2] =2;
				Sel[3] =3;
			}
			else{
				for(int i =0; i < SelSize;i++)
				{
					if(Sel[i] == 0)
					{
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						SelSize--;
					}
				}
			}
			break;
		case 1:
			if((j > 0) && (Paths[i][j-1].path[4] == false)){
				DEBUG << "x,y:" << i << " " << j << " Up:" ;

				Paths[i][j].path[4] = true;
				Paths[i][j].path[1] = true;
				rooms++;
				Paths[i][j-1].x =i;
				Paths[i][j-1].y =j;

				Paths[i][j-1].path[3] = true;
				j--;

				DEBUG << i << " " << j << " " << rooms << endl;
				SelSize=4;
				Sel[0] =0;
				Sel[1] =1;
				Sel[2] =2;
				Sel[3] =3;
			}
			else{
				for(int i =0; i < SelSize;i++)
				{
					if(Sel[i] == 1)
					{
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						SelSize--;
					}
				}
			}
			break;
		case 2:
			if(i+1 < 8 && Paths[i+1][j].path[4] == false){
				DEBUG << "x,y:" << i << " " << j << " Right:" ;

				Paths[i][j].path[4] = true;
				Paths[i][j].path[2] = true;
				rooms++;
				Paths[i+1][j].x =i;
				Paths[i+1][j].y =j;

				Paths[i+1][j].path[0] = true;
				i++;
				DEBUG << i << " " << j << " " << rooms << endl;
				SelSize=4;
				Sel[0] =0;
				Sel[1] =1;
				Sel[2] =2;
				Sel[3] =3;
			}
			else{
				for(int i =0; i < SelSize;i++)
				{
					if(Sel[i] == 2)
					{
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						SelSize--;
					}
				}
			}
			break;
		case 3:
			if(j+1 < 4 && Paths[i][j+1].path[4] == false){
				DEBUG << "x,y:" << i << " " << j << " Down:" ;

				Paths[i][j].path[4] = true;
				Paths[i][j].path[3] = true;
				rooms++;
				Paths[i][j+1].x =i;
				Paths[i][j+1].y =j;

				Paths[i][j+1].path[1] = true;
				j++;
				DEBUG << i << " " << j << " " << rooms << endl;
				SelSize=4;
				Sel[0] =0;
				Sel[1] =1;
				Sel[2] =2;
				Sel[3] =3;
			}
			else{
				for(int i =0; i < SelSize;i++)
				{
					if(Sel[i] == 3)
					{
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						i++;
						if(i < SelSize)
						{
							Sel[i-1] = Sel[i];
						}
						SelSize--;
					}
				}
			}
			break;
		}
	}

	//ok now i'm making random paths
	for(int l =0,x,y; l < 16;l++)
	{
		x = rand()%8;
		y = rand()%4;
		SelSize=4;
		Sel[0] =0;
		Sel[1] =1;
		Sel[2] =2;
		Sel[3] =3;
		for(; SelSize > 0; SelSize--){
			k = Sel[rand()%SelSize];
			switch(k){
			case 0:
				if(x-1 >= 0 && Paths[x][y].path[0] == false){

					Paths[x][y].path[4] = true;
					Paths[x][y].path[0] = true;
					Paths[x-1][y].x =i;
					Paths[x-1][y].y =j;

					Paths[x-1][y].path[2] = true;
					SelSize--;
				}
				else{
					SelSize--;
					SelSize--;
					SelSize--;
				}
				break;
			case 1:
				if((y > 0) && (Paths[x][j-1].path[1] == false)){

					Paths[x][y].path[4] = true;
					Paths[x][y].path[1] = true;
					Paths[x][y-1].x =i;
					Paths[x][y-1].y =j;

					Paths[x][y-1].path[3] = true;
					SelSize--;
				}
				else{
					SelSize--;
					SelSize--;
					SelSize--;
				}
				break;
			case 2:
				if(x+1 < 8 && Paths[i+1][y].path[2] == false){
					Paths[x][y].path[4] = true;
					Paths[x][y].path[2] = true;
					Paths[x+1][y].x =i;
					Paths[x+1][y].y =j;

					Paths[x+1][y].path[0] = true;
					SelSize--;
				}
				else{
					SelSize--;
					SelSize--;
					SelSize--;
				}
				break;
			case 3:
				if(y+1 < 4 && Paths[x][j+1].path[3] == false){

					Paths[x][y].path[4] = true;
					Paths[x][y].path[3] = true;
					Paths[x][y+1].x =i;
					Paths[x][y+1].y =j;

					Paths[x][y+1].path[1] = true;
					SelSize--;
				}
				else{
					SelSize--;
					SelSize--;
					SelSize--;
				}
				break;
			}
		}
	}


	for(int i =0; i < 8; i++)
		for(int j =0; j <4; j++)
		{
			int k;
			k=((j*10)*Size.x)+(i*10);
			if(Paths[i][j].path[1] == true)
			{
				k += 5;
				FloorPlan[k].SET(k%(int)Size.x, k/Size.x,Size.x,Size.y,0);
				k=((j*10)*Size.x)+(i*10);
			}
			if(Paths[i][j].path[0] == true)
			{
				k+= (5*Size.x);
				FloorPlan[k].SET(k%(int)Size.x, k/Size.x,Size.x,Size.y,0);
				k=((j*10)*Size.x)+(i*10);
			}
			if(Paths[i][j].path[3] == true)
			{
				k+= (10*Size.x)+5;
				FloorPlan[k].SET(k%(int)Size.x, k/Size.x,Size.x,Size.y,0);
				k=((j*10)*Size.x)+(i*10);
			}
			if(Paths[i][j].path[2] == true)
			{
				k+= (5*Size.x)+9;
				FloorPlan[k].SET(k%(int)Size.x, k/Size.x,Size.x,Size.y,0);
				k=((j*10)*Size.x)+(i*10);
			}
		}



}
