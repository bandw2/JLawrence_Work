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
#define Num_Nations 100
#include <string>
using namespace std;
#include "Pallette.h"
#include "Button.h"
#include "Map.h"
#include "Loc.h"
#include "Nation.h"
#include "Pathfinding.h"
#include "Army.h"

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
	ID3DXFont*			m_pD3DFontSmall;	
	ID3DXFont*			m_pD3DFont;		// Font Object
	ID3DXFont*			m_pD3DFontLarge;
	char fps[256];
	char x[256];
	char y[256];
	char Spe[256];
	unsigned long long mFPS;
	long long mMilliSecPerFrame;
	//////////////////////////////////////////////////////////////////////////
	// Sprite Variables
	//////////////////////////////////////////////////////////////////////////
	ID3DXSprite*		m_pD3DSprite;	// Sprite Object
	IDirect3DTexture9*	m_pTexture;		// Texture Object for a sprite
	D3DXIMAGE_INFO		m_imageInfoSmall;	// File details of a texture
	D3DXIMAGE_INFO		m_imageInfoUI;	// File details of a texture
	static const int SpriteSize = 32;
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
	pallette* Pallette[3];
	Button* Buttons[3];
	enum {Map,RightHand,UI};
	/////////////////////////////////////////////////////////////////////////
	//Map
	///////////////////////////////////////////////////////////////////////
	WorldMap World;
	

	Nation* Nations[100];
	Nation* m_Player;

	int Test;
	Army PH[100];
	
	//Pathfinding variables.  Don't worry about them.  Just use them where they are defined in the functions
	int source;
	int pathSize;
	list<int> path;
	vector<double> min_dist;
	vector<int> previous;
	Graph g;


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
};
