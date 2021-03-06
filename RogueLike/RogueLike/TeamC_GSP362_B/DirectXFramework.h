//////////////////////////////////////////////////////////////////////////
// Name:	DirectXFramework.h
// Date:	April 2nd, 2010
// Author:	Kyle Lauing [klauing@devry.edu] or [kylelauing@gmail.com]
// Purpose: This file is used to create a very simple framework for using
//			DirectX 9 for the GSP 381 course for DeVry University.
// Disclaimer:	
//			Copyright © 2010 by DeVry Educational Development Corporation.
//			All rights reserved.  No part of this work may be reproduced 
//			or used in any form or by any means – graphic, electronic, or 
//			mechanical, including photocopying, recording, Web distribution 
//			or information storage and retrieval systems – without the 
//			prior consent of DeVry Educational Development Corporation.
//////////////////////////////////////////////////////////////////////////
#pragma once
#pragma comment(lib, "winmm.lib")
//////////////////////////////////////////////////////////////////////////
// Direct3D 9 headers and libraries required
//////////////////////////////////////////////////////////////////////////
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "fmodex.dll")
#pragma comment(lib, "fmodex_vc.lib")
#include "fmod.hpp"
#include "fmod_dsp.h"
// Macro to release COM objects fast and safely
#define SAFE_RELEASE(x) if(x){x->Release(); x = 0;}
#define MaxEnt 100
#define MaxLoot 100
#define SpawnTimerSeconds 0.001f
#include <string>
#include <queue>
using namespace std;
#include "V2D.h"
#include "PathHelper.h"
#include "PlaceHolder.h"
#include "Map.h"
#include "Button.h"
#include "Player.h"
#include "Exit.h"
#include "Loot.h"
#define DeckSize 52
#define HandSize 13


class CDirectXFramework
{
	//////////////////////////////////////////////////////////////////////////
	// Application Variables
	//////////////////////////////////////////////////////////////////////////
	HWND				m_hWnd;			// Handle to the window
	bool				m_bVsync;		// Boolean for vertical syncing

	//////////////////////////////////////////////////////////////////////////
	// Direct3D Variables
	//////////////////////////////////////////////////////////////////////////
	IDirect3D9*				m_pD3DObject;	// Direct3D 9 Object
	IDirect3DDevice9*		m_pD3DDevice;	// Direct3D 9 Device
	D3DCAPS9				m_D3DCaps;		// Device Capabilities
	D3DPRESENT_PARAMETERS	D3Dpp;

	//////////////////////////////////////////////////////////////////////////
	// Font Variables
	//////////////////////////////////////////////////////////////////////////
	ID3DXFont*			m_pD3DFont;		// Font Object
	ID3DXFont*			m_pD3DFontLarge;
	ID3DXFont*			m_pD3DFontCool;
	ID3DXFont*			m_pD3DFontCoolFine;
	ID3DXFont*			m_pD3DFontCoolUltraFine;

	char fps[256];
	char x[256];
	char y[256];
	char Spe[256];
	long mFPS;
	long long mMilliSecPerFrame;
	//////////////////////////////////////////////////////////////////////////
	// Sprite Variables
	//////////////////////////////////////////////////////////////////////////
	ID3DXSprite*		m_pD3DSprite;	// Sprite Object
	IDirect3DTexture9*	m_pTexture;		// Texture Object for a sprite
	D3DXIMAGE_INFO		m_imageInfo;	// File details of a texture

	//////////////////////////////////////////////////////////////////////////
	// Sprites
	//////////////////////////////////////////////////////////////////////////
	//static const int SpriteSize = 3;
	//SpriteTexture Sprites[SpriteSize];
	//float X,Y;
	//float Speed;
	//float Drift;
	//////////////////////////////////////////////////////////////////////////
	// input
	//////////////////////////////////////////////////////////////////////////

	IDirectInput8* m_pDIObject;
	IDirectInputDevice8* m_pDIKeyboard;
	IDirectInputDevice8* m_pDIMouse;
	DIMOUSESTATE mouseState;
	unsigned char Buffer[256];
	bool m_BoolBuf[256];
	bool LeftMouseDown;
	bool RightMouseDown;
	int scorea, scoreb;
	
	//////////////////////////////////////////////////////////////////////////
	// state data
	//////////////////////////////////////////////////////////////////////////
	int State;
	bool options[10];
	bool windowed;

	void EnableFullscreen(bool);
	/////////////////////////////////////////////////////////////////////////
	//Speakers
	///////////////////////////////////////////////////////////////////////
	FMOD::System *system;
	FMOD_RESULT result;
	unsigned int version;
	int numdrivers;
	FMOD_SPEAKERMODE speakermode;
	FMOD_CAPS caps;
	FMOD::Channel *SChannel[2];
	FMOD::Sound *Sounds[6];
	char name[256];
	/////////////////////////////////////////////////////////////////////////
	//Mouse
	///////////////////////////////////////////////////////////////////////
	long m_Mousex, m_Mousey;
	/////////////////////////////////////////////////////////////////////////
	//button
	///////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	//Map
	///////////////////////////////////////////////////////////////////////
	Map World;
	V2D Displacement;
	int Level;
	IDirect3DTexture9* ExitTex;
	Exit m_Exit;
	/////////////////////////////////////////////////////////////////////////
	//Entities and AI and Loot
	///////////////////////////////////////////////////////////////////////
	Enemy* EntArray[MaxEnt];
	Loot* LootArray[MaxLoot];
	int Turn;
	int NumEnt;
	int NumLoot;
	int EnemyVision;
	
	IDirect3DTexture9* MonTex[3];
	/////////////////////////////////////////////////////////////////////////
	//Player
	///////////////////////////////////////////////////////////////////////
	Player Character;
	D3DXMATRIX PlayerPos;
	IDirect3DTexture9* PlayerTex;
	char Str_c[256];
	char Dex_c[256];
	char Spd_c[256];
	char Int_c[256];
	char Health_c[256];
	char Ammo_c[256];
	int SpawnCounter;
	int LevelReq;
	/////////////////////////////////////////////////////////////////////////
	//UI
	///////////////////////////////////////////////////////////////////////
	D3DXMATRIX UIPos;
	D3DXMATRIX WeaponPos;
	D3DXMATRIX RangedPos;
	D3DXMATRIX UtilPos;
	D3DXMATRIX ArmorPos;
	D3DXMATRIX InventoryPos[5];
	IDirect3DTexture9* UI;
	IDirect3DTexture9* WeaponTex;
	IDirect3DTexture9* RangedTex;
	IDirect3DTexture9* MedkitTex;
	IDirect3DTexture9* GogglesTex;
	IDirect3DTexture9* GrenadeTex;
	IDirect3DTexture9* ArmorTex;
	int Score;
public:
	//////////////////////////////////////////////////////////////////////////
	// Init and Shutdown are preferred to constructors and destructor,
	// due to having more control when to explicitly call them when global.
	//////////////////////////////////////////////////////////////////////////
	CDirectXFramework(void);
	~CDirectXFramework(void);

	//////////////////////////////////////////////////////////////////////////
	// Name:		Init
	// Parameters:	HWND hWnd - Handle to the window for the application
	//				HINSTANCE hInst - Handle to the application instance
	//				bool bWindowed - Boolean to control windowed or full-screen
	// Return:		void
	// Description:	Ran once at the start.  Initialize DirectX components and 
	//				variables to control the application.  
	//////////////////////////////////////////////////////////////////////////
	void Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed);

	//////////////////////////////////////////////////////////////////////////
	// Name:		Update
	// Parameters:	float elapsedTime - Time that has elapsed since the last
	//					update call.
	// Return:		void
	// Description: Runs every frame, use dt to limit functionality called to
	//				a certain amount of elapsed time that has passed.  Used 
	//				for updating variables and processing input commands prior
	//				to calling render.
	//////////////////////////////////////////////////////////////////////////
	void Update(float dt);

	//////////////////////////////////////////////////////////////////////////
	// Name:		Render
	// Parameters:	float elapsedTime - Time that has elapsed since the last
	//					render call.
	// Return:		void
	// Description: Runs every frame, use dt to limit functionality called to
	//				a certain amount of elapsed time that has passed.  Render
	//				calls all draw call to render objects to the screen.
	//////////////////////////////////////////////////////////////////////////
	void Render();

	//////////////////////////////////////////////////////////////////////////
	// Name:		Shutdown
	// Parameters:	void
	// Return:		void
	// Description:	Runs once at the end of an application.  Destroy COM 
	//				objects and deallocate dynamic memory.
	//////////////////////////////////////////////////////////////////////////
	void Shutdown();
	void StartButton();
	void ExitButton();
	void WindowedButton();

	//////////////////////////////////////////////////////////////////////////
	// Name:		AI
	// Parameters:	void
	// Return:		void
	// Description:	Makes AI GO 
	//				
	//////////////////////////////////////////////////////////////////////////
	void AI(Map& World);

	//////////////////////////////////////////////////////////////////////////
	// Name:		AddAI
	// Parameters:	void
	// Return:		void
	// Description:	Makes an AI 
	//				
	//////////////////////////////////////////////////////////////////////////
	void AddAI();	//////////////////////////////////////////////////////////////////////////
	// Name:		AddAI
	// Parameters:	void
	// Return:		void
	// Description:	Makes an Loot Drop 
	//				
	//////////////////////////////////////////////////////////////////////////
	void AddLoot();
	//////////////////////////////////////////////////////////////////////////
	// Name:		CalcVision
	// Parameters:	void
	// Return:		void
	// Description:	figures out what the Player can see 
	//				
	//////////////////////////////////////////////////////////////////////////
	void CalcVision();
	//////////////////////////////////////////////////////////////////////////
	// Name:		CalcMove
	// Parameters:	void
	// Return:		int
	// Description:	figures out if the monster can see the player, and then returns which directins to move 
	//				
	//////////////////////////////////////////////////////////////////////////
	int CalcMove(V2D Loc);
	//////////////////////////////////////////////////////////////////////////
	// Name:		AIAttack
	// Parameters:	void
	// Return:		void
	// Description:	takes in enemy to apply damage to player 
	//				
	//////////////////////////////////////////////////////////////////////////
	void AIAttack(Enemy him);
	//////////////////////////////////////////////////////////////////////////
	// Name:		Melee
	// Parameters:	Enemy, bool
	// Return:		void
	// Description:	takes in enemy to apply damage to that enemy using player stats,
	//				bool for melee or ranged		
	//////////////////////////////////////////////////////////////////////////
	void Attack(Enemy* him, bool Melee);
	//////////////////////////////////////////////////////////////////////////
	// Name:		CalcTurn
	// Parameters:	void
	// Return:		void
	// Description: whos turn is it?
	//						
	/////////////////////////////////////////////////////////////////////////
	void CalcTurn();
	//////////////////////////////////////////////////////////////////////////
	// Name:		EndTurn
	// Parameters:	void
	// Return:		void
	// Description: do end of player turn stuff
	//						
	/////////////////////////////////////////////////////////////////////////
	void EndTurn();
	//////////////////////////////////////////////////////////////////////////
	// Name:		CheckFinish
	// Parameters:	void
	// Return:		void
	// Description: checks to see if you won the level and,
	//				does the code for setting up the next level		
	/////////////////////////////////////////////////////////////////////////
	void CheckFinish();
	//////////////////////////////////////////////////////////////////////////
	// Name:		ItemGen
	// Parameters:	(in)void*&
	// Return:		void
	// Description: returns a random item of a random type
	//						
	/////////////////////////////////////////////////////////////////////////
	void ItemGen(void*&);
	//////////////////////////////////////////////////////////////////////////
	// Name:		ItemText
	// Parameters:	(in)void*&
	// Return:		void
	// Description: uses the (in) to draw parameters, at x and y, with teh color UItext
	//						
	/////////////////////////////////////////////////////////////////////////
	void ItemText(void*&, int x, int y, D3DCOLOR UItext);
	//////////////////////////////////////////////////////////////////////////
	// Name:		Pickup
	// Parameters:	int
	// Return:		void
	// Description: Swaps the item with the key you want to pick up
	//						
	/////////////////////////////////////////////////////////////////////////
	void Pickup(int);
	//////////////////////////////////////////////////////////////////////////
	// Name:		Pickup
	// Parameters:	int
	// Return:		void
	// Description: does a grenade originating from x y
	//						
	/////////////////////////////////////////////////////////////////////////
	void Grenade(int x,int y);

};
