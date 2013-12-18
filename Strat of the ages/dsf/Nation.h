#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <string>
using namespace std;

struct Nation{
	string m_Name;
	D3DXCOLOR m_Flag;
	int m_LandTech;
	int m_SeaTech;
	int m_EconomyTech;

	//Unit Stats
	int ArmyAtk;
	int ArmyDef;
	int ArmyMAtk;
	int ArmyMDef;
	float ArmyMaxMorale;

	Nation():m_LandTech(0),m_SeaTech(0),m_EconomyTech(0){

		m_Name = "NULL";
		m_Flag = D3DCOLOR_ARGB(255,rand()%255,rand()%255,rand()%255);

		ArmyAtk = 0;
		ArmyDef = 0;
		ArmyMAtk = 0;
		ArmyMDef = 0;
		ArmyMaxMorale = 1.0;

	}
	
	Nation(string a_Name,int A, int R, int G,int B):m_LandTech(0),m_SeaTech(0),m_EconomyTech(0){

		m_Name = a_Name;
		m_Flag = D3DCOLOR_ARGB(A,R,G,B);

		ArmyAtk = 0;
		ArmyDef = 0;
		ArmyMAtk = 0;
		ArmyMDef = 0;
		ArmyMaxMorale = 1.0;

	}

	void UpdateUnitStats(){
		ArmyAtk = m_LandTech+1;
		ArmyDef = m_LandTech+1;
		ArmyMAtk = m_LandTech+1;
		ArmyMDef = m_LandTech+1;
		ArmyMaxMorale = m_LandTech+2;
	}

};