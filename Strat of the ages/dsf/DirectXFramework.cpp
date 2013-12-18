//////////////////////////////////////////////////////////////////////////
// Name:	DirectXFramework.cpp
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
#include "DirectXFramework.h"
#include <queue>
#include "MapGen.h"
#include <time.h>

CDirectXFramework::CDirectXFramework(void)
{
	// Init or NULL objects before use to avoid any undefined behavior
	m_bVsync		= false;
	m_pD3DObject	= 0;
	m_pD3DDevice	= 0;
	scorea = 0;
	scoreb = 0;
	State = 0;
	for(int i = 0; i < 10; i++){
		options[i] = false;
	}
	options[0] = true;
	for(int i = 0; i < 256; i++){
		m_BoolBuf[i] = false;
	}
	windowed = true;
	m_Mousex = m_Mousey = 0;
	LeftMouseDown = false;
	RightMouseDown = false;

	Test = -1;


}
CDirectXFramework::~CDirectXFramework(void)
{
	// If Shutdown is not explicitly called correctly, call it when 
	// this class is destroyed or falls out of scope as an error check.
	Shutdown();
}

void CDirectXFramework::EnableFullscreen(bool FullScrn){
	if(FullScrn)
	{
		if(!D3Dpp.Windowed)
			return;
		//get the screen Resolution
		int width  = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN); 

		D3Dpp.BackBufferFormat = D3DFMT_X8R8G8B8; //not specifically sure what this is
		D3Dpp.BackBufferWidth  = width;//setting up back buffer size
		D3Dpp.BackBufferHeight = height;
		D3Dpp.Windowed         = false;//I AM NOT WINDOWED

		SetWindowLongPtr(m_hWnd, GWL_STYLE, WS_POPUP); //set window to POPUP which removes the borders
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0,
			width, height, SWP_NOZORDER | SWP_SHOWWINDOW); //Basically, reset the window size and refresh the above option for use, or "use the new settings"

	}
	else//GOING WINDOWED
	{
		// Are we already in windowed mode?
		if( D3Dpp.Windowed )
			return;
		// Default to a client rectangle of 800x600.
		RECT R = {0, 0, 800, 600};
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
		D3Dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		D3Dpp.BackBufferWidth  = 800;
		D3Dpp.BackBufferHeight = 600;
		D3Dpp.Windowed         = true;
		SetWindowLongPtr(m_hWnd,GWL_STYLE,WS_OVERLAPPEDWINDOW);
		SetWindowPos(m_hWnd, HWND_TOP, 100, 100,
			R.right, R.bottom, SWP_NOZORDER | SWP_SHOWWINDOW);

	}
	// Reset the device with the changes.
	m_pD3DSprite->OnLostDevice();
	m_pD3DFontSmall->OnLostDevice();
	m_pD3DFont->OnLostDevice();
	m_pD3DFontLarge->OnLostDevice();
	m_pD3DDevice->Reset(&D3Dpp);
	m_pD3DSprite->OnResetDevice();
	m_pD3DFont->OnResetDevice();
	m_pD3DFontLarge->OnResetDevice();
}

void CDirectXFramework::Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed)
{
	srand(time(NULL));
	LARGE_INTEGER Timer;
	QueryPerformanceCounter(&Timer);
	srand(Timer.QuadPart);
	m_hWnd = hWnd;

	//////////////////////////////////////////////////////////////////////////
	// Direct3D Foundations - D3D Object, Present Parameters, and D3D Device
	//////////////////////////////////////////////////////////////////////////

	// Create the D3D Object
	m_pD3DObject = Direct3DCreate9(D3D_SDK_VERSION);

	// Find the width and height of window using hWnd and GetWindowRect()
	RECT rect;
	GetWindowRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// Set D3D Device presentation parameters before creating the device
	ZeroMemory(&D3Dpp, sizeof(D3Dpp));  // NULL the structure's memory

	D3Dpp.hDeviceWindow					= hWnd;										// Handle to the focus window
	D3Dpp.Windowed						= bWindowed;								// Windowed or Full-screen boolean
	D3Dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;								// Format of depth/stencil buffer, 24 bit depth, 8 bit stencil
	D3Dpp.EnableAutoDepthStencil		= TRUE;										// Enables Z-Buffer (Depth Buffer)
	D3Dpp.BackBufferCount				= 1;										// Change if need of > 1 is required at a later date
	D3Dpp.BackBufferFormat				= D3DFMT_X8R8G8B8;							// Back-buffer format, 8 bits for each pixel
	D3Dpp.BackBufferHeight				= height;									// Make sure resolution is supported, use adapter modes
	D3Dpp.BackBufferWidth				= width;									// (Same as above)
	D3Dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;					// Discard back-buffer, must stay discard to support multi-sample
	D3Dpp.PresentationInterval			= m_bVsync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE; // Present back-buffer immediately, unless V-Sync is on								
	D3Dpp.Flags							= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;		// This flag should improve performance, if not set to NULL.
	D3Dpp.FullScreen_RefreshRateInHz	= bWindowed ? 0 : D3DPRESENT_RATE_DEFAULT;	// Full-screen refresh rate, use adapter modes or default
	D3Dpp.MultiSampleQuality			= 0;										// MSAA currently off, check documentation for support.
	D3Dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;						// MSAA currently off, check documentation for support.

	// Check device capabilities
	DWORD deviceBehaviorFlags = 0;
	m_pD3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_D3DCaps);

	// Determine vertex processing mode
	if(m_D3DCaps.DevCaps & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		// Hardware vertex processing supported? (Video Card)
		deviceBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	
	}
	else
	{
		// If not, use software (CPU)
		deviceBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING; 
	}

	// If hardware vertex processing is on, check pure device support
	if(m_D3DCaps.DevCaps & D3DDEVCAPS_PUREDEVICE && deviceBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		deviceBehaviorFlags |= D3DCREATE_PUREDEVICE;	
	}

	// Create the D3D Device with the present parameters and device flags above
	m_pD3DObject->CreateDevice(
		D3DADAPTER_DEFAULT,		// which adapter to use, set to primary
		D3DDEVTYPE_HAL,			// device type to use, set to hardware rasterization
		hWnd,					// handle to the focus window
		deviceBehaviorFlags,	// behavior flags
		&D3Dpp,					// presentation parameters
		&m_pD3DDevice);			// returned device pointer

	//*************************************************************************

	//////////////////////////////////////////////////////////////////////////
	// Create a Font Object
	//////////////////////////////////////////////////////////////////////////

	// Load a font for private use for this process
	D3DXCreateFont(m_pD3DDevice, 16, 0, FW_BOLD, 0, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"),
		&m_pD3DFontSmall);
	D3DXCreateFont(m_pD3DDevice, 30, 0, FW_BOLD, 0, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"),
		&m_pD3DFont);
	D3DXCreateFont(m_pD3DDevice, 40, 0, FW_BOLD, 0, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"),
		&m_pD3DFontLarge);
	// Load D3DXFont, each font style you want to support will need an ID3DXFont

	for(int i = 0; i < 3; i++){
		Pallette[i] = new pallette;
		Pallette[i]->DeltaX = 0;
		Pallette[i]->DeltaY = 0;
	}

	RECT rect2;
	rect2.top = -33;
	rect2.left = -33;
	rect2.bottom = 640;
	rect2.right= 640;
	Pallette[0]->Me = rect2;
	Pallette[0]->NumDraw = 10000;
	rect2.top = -1;
	rect2.left = 639;
	rect2.bottom = 640;
	rect2.right= 800;
	Pallette[1]->Me = rect2;
	Pallette[1]->NumDraw = 1;


	//////////////////////////////////////////////////////////////////////////
	// Create Sprite Object and Textures
	//////////////////////////////////////////////////////////////////////////

	// Create a sprite object, note you will only need one for all 2D sprites

	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

	// Create a texture, each different 2D sprite to display to the screen
	// will need a new texture object.  If drawing the same sprite texture
	// multiple times, just call that sprite's Draw() with different 
	// transformation values.
	Pallette[Map]->m_Textures = new IDirect3DTexture9*[6];
	Pallette[RightHand]->m_Textures = new IDirect3DTexture9*[1];
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Land.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfoSmall, 0, &m_pTexture);
	Pallette[Map]->m_Textures[0] = m_pTexture;
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Forest.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfoSmall, 0, &m_pTexture);
	Pallette[Map]->m_Textures[1] = m_pTexture;
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Desert.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfoSmall, 0, &m_pTexture);
	Pallette[Map]->m_Textures[2] = m_pTexture;
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Mountain.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfoSmall, 0, &m_pTexture);
	Pallette[Map]->m_Textures[3] = m_pTexture;
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Water.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfoSmall, 0, &m_pTexture);
	Pallette[Map]->m_Textures[4] = m_pTexture;
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Unit.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfoSmall, 0, &m_pTexture);
	Pallette[Map]->m_Textures[5] = m_pTexture;
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"RighthandUI.png", D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfoUI, 0, &m_pTexture);
	Pallette[RightHand]->m_Textures[0] = m_pTexture;
	Loc locs;
	locs.drwmx = 640;
	locs.drwmy = 0;
	locs.mx = 640;
	locs.my = 0;
	Pallette[RightHand]->Locs[0] = locs;
	for(int i = 0; i < 100;i++)
		for(int j = 0; j < 100;j++){
			if(i%2){
			locs.drwmx = j*SpriteSize;
			locs.drwmy = i*SpriteSize;
			locs.mx = j*SpriteSize;
			locs.my = i*SpriteSize;
			Pallette[Map]->Locs[j+i*100] = locs;}
			else{
			locs.drwmx = (j*SpriteSize)-16;
			locs.drwmy = (i*SpriteSize);
			locs.mx = (j*SpriteSize)-16;
			locs.my = (i*SpriteSize);
			Pallette[Map]->Locs[j+i*100] = locs;}

		}

	//MapSize defined in Map.h

	for(int i = 0; i < 100; i++){//TODO get names and such from files.
		Nations[i] = new Nation;
	}	
	
	queue<MapGenTile,deque<MapGenTile>> Mapgen;
	for(int i = 0; i < 10000; i++){
		World.getProv(i).Set = false;
	}

	// Test Army for setting nations.
	// Use setNation as needed to have it equal whatever nation you need.  Should hopefully work.
	// If not, I need to understand how to get and set Justin's nations better
	// -------> Christian
	// ->Justin, moved to map gen to place armies on capital

	for(int i = 0; i < Num_Nations; i++){//push Nation Seeds
		bool done = false;
		int ProvID = 0;
		while(!done){
			ProvID = rand()%10000;
			if(World.getProv(ProvID).mtype != Water)
				done = true;
		}
		Mapgen.push(MapGenTile(ProvID,i));
		PH[i].setNation(Nations[i]);
		PH[i].moveTo(ProvID);
	}



	while(!Mapgen.empty())//while map generator not done
	{
		if(!World.getProv(Mapgen.front().ProvID).Set){//just incase some stuff gets put on other stuff
			World.getProv(Mapgen.front().ProvID).m_Nation = Nations[Mapgen.front().Type];
			World.getProv(Mapgen.front().ProvID).Set = true;
			for(int i = 0; i < 6; i++)//push neighbors
			{
				if(World.getProv(Mapgen.front().ProvID).connections[i] != -1)
					if( World.getProv(World.getProv(Mapgen.front().ProvID).connections[i]).Set == false
						&& 
						World.getProv(World.getProv(Mapgen.front().ProvID).connections[i]).mtype != Water)
					{
						Mapgen.push(MapGenTile(World.getProv(Mapgen.front().ProvID).connections[i],Mapgen.front().Type));
					}
			}
		}
		Mapgen.pop();
	}

	// Quick test for pathfinding using Justin's Map.
	// It works.  Variables in DirectXFramework.h
	// Nothing needs to be created.  Use what's already there.
	source = 0;
	vect graph(10000);
	g.setWeights(World, graph);

	g.ComputePaths(source, graph, min_dist, previous);
	path = g.GetShortest(1000, previous);
	// Call this line when wanting to create an array for the path.
	// This is just a test
	pathSize = path.size();





	//*************************************************************************
	DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDIObject, NULL);

	m_pDIObject->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL); 
	m_pDIObject->CreateDevice(GUID_SysMouse, &m_pDIMouse, NULL);

	m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard); 
	m_pDIMouse->SetDataFormat(&c_dfDIMouse);

	m_pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); 
	m_pDIMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	//##########################################SPEAKERS#############################
	/*
	Create a System object and initialize.
	*/
	result = FMOD::System_Create(&system);
	result = system->getVersion(&version);
	result = system->getNumDrivers(&numdrivers);
	if (numdrivers == 0)
	{
		result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
	}
	else
	{
		result = system->getDriverCaps(0, &caps, 0, &speakermode);
		/*
		Set the user selected speaker mode.
		*/
		if (caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			/*
			The user has the 'Acceleration' slider set to off! This is really bad
			for latency! You might want to warn the user about this.
			*/
			result = system->setDSPBufferSize(1024, 10);
		}
		result = system->getDriverInfo(0, name, 256, 0);
		if (strstr(name, "SigmaTel"))
		{
			/*
			Sigmatel sound devices crackle for some reason if the format is PCM 16bit.
			PCM floating point output seems to solve it.
			*/
			result = system->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0,
				FMOD_DSP_RESAMPLER_LINEAR);
		}
	}
	result = system->init(100, FMOD_INIT_NORMAL, 0);
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		/*
		Ok, the speaker mode selected isn't supported by this soundcard. Switch it
		back to stereo...
		*/
		result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		/*
		... and re-init.
		*/
		result = system->init(100, FMOD_INIT_NORMAL, 0);
	}

	result = system->createSound("drop.wav", FMOD_LOOP_OFF, 0, &Sounds[0]);
	result = system->createSound("bomb.wav", FMOD_LOOP_OFF, 0, &Sounds[1]);
	result = system->createSound("nope.wav", FMOD_LOOP_OFF, 0, &Sounds[2]);
	result = system->createSound("play.wav", FMOD_LOOP_OFF, 0, &Sounds[3]);
	result = system->createSound("select.wav", FMOD_LOOP_OFF, 0, &Sounds[4]);
	result = system->createStream("song.wav", FMOD_LOOP_NORMAL | FMOD_2D, 0, &Sounds[5]);

	system->playSound(FMOD_CHANNEL_FREE, Sounds[5], false, &SChannel[1]);

	for(int i = 0; i < 3; i++)
		Buttons[i] = new Button;

	Buttons[0]->Y = (D3Dpp.BackBufferHeight/2);
	Buttons[0]->X = (D3Dpp.BackBufferWidth/2)-160;
	Buttons[0]->Height = 32;
	Buttons[0]->Width = 320;
	Buttons[0]->CalcRECT();
	Buttons[1]->Y = (D3Dpp.BackBufferHeight/2)+48;
	Buttons[1]->X = (D3Dpp.BackBufferWidth/2)-160;
	Buttons[1]->Height = 32;
	Buttons[1]->Width = 320;
	Buttons[1]->CalcRECT();
	Buttons[2]->Y = (D3Dpp.BackBufferHeight/2)+88;
	Buttons[2]->X = (D3Dpp.BackBufferWidth/2)-160;
	Buttons[2]->Height = 32;
	Buttons[2]->Width = 320;
	Buttons[2]->CalcRECT();
}

void CDirectXFramework::Update(float dt)
{
	// Make static so that their values persist across
	// function calls.
	static float numFrames     = 0.0f;
	static float timeElapsed = 0.0f;
	static float Timer = 0.0f;
	// Increment the frame count.
	numFrames += 1.0f;
	// Accumulate how much time has passed.
	timeElapsed += dt;
	// Has one second passed?--we compute the frame statistics once
	// per second.   Note that the time between frames can vary, so
	// these stats are averages over a second.
	if( timeElapsed >= 1.0f )
	{
		// Frames Per Second = numFrames / timeElapsed,
		// but timeElapsed approx. equals 1.0, so
		// frames per second = numFrames.
		mFPS = numFrames;
		// Average time, in milliseconds, it took to render a
		// single frame.
		mMilliSecPerFrame = 1000.0f / mFPS;
		// Reset time counter and frame count to prepare
		// for computing the average stats over the next second.
		timeElapsed = 0.0f;
		numFrames     = 0.0f;
	}
	int width, height;
	//KEYBOARD
	HRESULT hr;
	hr = m_pDIKeyboard->GetDeviceState(sizeof(Buffer), &Buffer);
	if(hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED){
		hr = m_pDIKeyboard->Acquire();
		hr = m_pDIKeyboard->GetDeviceState(sizeof(Buffer), &Buffer);}
	//mouse
	hr = m_pDIMouse->GetDeviceState(sizeof(mouseState),&mouseState);
	if(hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED){
		hr = m_pDIMouse->Acquire();
		hr = m_pDIMouse->GetDeviceState(sizeof(mouseState),&mouseState);}
	switch(State){
	case 0:
		if(options[0])
		{
			if(Buffer[DIK_UP] & 0x80){//PRESS UP KEY, WHILE ON START
				if(!m_BoolBuf[DIK_UP]){//FIRST DOWN PRESS, SET PRESSED
					m_BoolBuf[DIK_UP] = true;
					options[0] = false;
					options[2] = true;
				}
			}
			else
			{
				m_BoolBuf[DIK_UP] = false;
			}
			//DOWN
			if(Buffer[DIK_DOWN] & 0x80){
				if(!m_BoolBuf[DIK_DOWN]){
					m_BoolBuf[DIK_DOWN] = true;
					options[0] = false;
					options[1] = true;
				}
			}
			else
			{
				m_BoolBuf[DIK_DOWN] = false;
			}
			//RETURN
			if(Buffer[DIK_RETURN] & 0x80 || mouseState.rgbButtons[0]){
				if(!m_BoolBuf[DIK_RETURN]){
					m_BoolBuf[DIK_RETURN] = true;
					State = 2;
				}
			}
			else
			{
				m_BoolBuf[DIK_RETURN] = false;
			}
		}
		if(options[1])
		{
			//UP
			if(Buffer[DIK_UP] & 0x80){
				if(!m_BoolBuf[DIK_UP]){
					m_BoolBuf[DIK_UP] = true;
					options[1] = false;
					options[0] = true;
				}
			}
			else
			{
				m_BoolBuf[DIK_UP] = false;
			}
			//DOWN
			if(Buffer[DIK_DOWN] & 0x80){
				if(!m_BoolBuf[DIK_DOWN]){
					m_BoolBuf[DIK_DOWN] = true;
					options[1] = false;
					options[2] = true;
				}
			}
			else
			{
				m_BoolBuf[DIK_DOWN] = false;
			}
			//RETURN
			if(Buffer[DIK_RETURN] & 0x80 || mouseState.rgbButtons[0]){
				PostQuitMessage(0);
			}
		}
		if(options[2]){
			//UP
			if(Buffer[DIK_UP] & 0x80){
				if(!m_BoolBuf[DIK_UP]){
					m_BoolBuf[DIK_UP] = true;
					options[2] = false;
					options[1] = true;
				}
			}
			else
			{
				m_BoolBuf[DIK_UP] = false;
			}
			//DOWN
			if(Buffer[DIK_DOWN] & 0x80){
				if(!m_BoolBuf[DIK_DOWN]){
					m_BoolBuf[DIK_DOWN] = true;
					options[2] = false;
					options[0] = true;
				}
			}
			else
			{
				m_BoolBuf[DIK_DOWN] = false;
			}
			//RETURN
			if((Buffer[DIK_RETURN] & 0x80) || mouseState.rgbButtons[0]){
				if(!m_BoolBuf[DIK_RETURN]){
					m_BoolBuf[DIK_RETURN] = true;
					LeftMouseDown = true;
					EnableFullscreen(D3Dpp.Windowed);
				}
			}
			else{
				m_BoolBuf[DIK_RETURN] = false;
				LeftMouseDown = false;
			}
		}
		RECT recta;
		GetWindowRect(m_hWnd, &recta);
		width = recta.right - recta.left;
		height = recta.bottom - recta.top;	

		POINT CursorPosa;
		GetCursorPos(&CursorPosa);
		m_Mousex = CursorPosa.x - recta.left;
		m_Mousey = CursorPosa.y - recta.top;




		if(Buttons[0]->IsCursorOnMe(m_Mousex,m_Mousey)){
			options[0] = true;
			options[1] = false;
			options[2] = false;
		}
		if(Buttons[1]->IsCursorOnMe(m_Mousex,m_Mousey)){
			options[0] = false;
			options[1] = true;
			options[2] = false;
		}
		if(Buttons[2]->IsCursorOnMe(m_Mousex,m_Mousey)){
			options[0] = false;
			options[1] = false;
			options[2] = true;
		}
		LeftMouseDown = true;
		
		break;
	case 1://#################################################################################################
		RECT rect;
		GetWindowRect(m_hWnd, &rect);
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;	

		POINT CursorPos;
		GetCursorPos(&CursorPos);
		m_Mousex = CursorPos.x - rect.left - GetSystemMetrics(SM_CXSIZEFRAME);
		m_Mousey = CursorPos.y - rect.top - GetSystemMetrics(SM_CYCAPTION);
		if(Buffer[DIK_SPACE] & 0x80){
			if(!m_BoolBuf[DIK_SPACE]){
				m_BoolBuf[DIK_SPACE] = true;
				//DO STUFF HERE
			}
		}
		else
		{
			m_BoolBuf[DIK_SPACE] = false;
		}	
		if(Buffer[DIK_W] & 0x80){
		//	if(!m_BoolBuf[DIK_W] ){
				m_BoolBuf[DIK_W] = true;
				//DO STUFF HERE
				Pallette[0]->DeltaY += (32.0f*6)/mFPS;
		//	}
		}
		//else
		//{
		//	m_BoolBuf[DIK_W] = false;
		//}	
		if(Buffer[DIK_S] & 0x80){
		//	if(!m_BoolBuf[DIK_S] ){
		//		m_BoolBuf[DIK_S] = true;
				//DO STUFF HERE
				Pallette[0]->DeltaY -= (32.0f*6)/mFPS;
		//	}
		}
		//else
		//{
		//	m_BoolBuf[DIK_S] = false;
		//}
		if(Buffer[DIK_A] & 0x80){
		//	if(!m_BoolBuf[DIK_A] ){
		//		m_BoolBuf[DIK_A] = true;
				//DO STUFF HERE
				Pallette[0]->DeltaX += (32.0f*6)/mFPS;
		//	}
		}
		//else
		//{
		//	m_BoolBuf[DIK_A] = false;
		//}
		if(Buffer[DIK_D] & 0x80 ){
		//	if(!m_BoolBuf[DIK_D]){
		//		m_BoolBuf[DIK_D] = true;
				//DO STUFF HERE
				Pallette[0]->DeltaX -= (32.0f*6)/mFPS;
		//	}
		}
		//else
		//{
		//	m_BoolBuf[DIK_D] = false;
		//}
		if(Buffer[DIK_RETURN] & 0x80){
			if(!m_BoolBuf[DIK_RETURN]){
				m_BoolBuf[DIK_RETURN] = true;
				//DO STUFF HERE
			}
		}
		else
		{
			m_BoolBuf[DIK_RETURN] = false;
		}
		if(Buffer[DIK_BACKSPACE] & 0x80){
			if(!m_BoolBuf[DIK_BACKSPACE]){
				m_BoolBuf[DIK_BACKSPACE] = true;
				//DO STUFF HERE
			}
		}
		else
		{
			m_BoolBuf[DIK_BACKSPACE] = false;
		}
		//####################################DEBUG##############################
		if(Buffer[DIK_P] & 0x80){
			if(!m_BoolBuf[DIK_P]){
				m_BoolBuf[DIK_P] = true;
				//DO STUFF HERE
			}
		}
		else
		{
			m_BoolBuf[DIK_P] = false;
		}
		system->update();	
		if(mouseState.rgbButtons[0] ){
			if(!LeftMouseDown){
				LeftMouseDown = true;
				//DO STUFF HERE
				Test = Pallette[Map]->IsCursorOnWho(m_Mousex,m_Mousey);
				if(!m_Player){
					m_Player = World.getProv(Test).m_Nation;
				}
			}
		}
		else{
			LeftMouseDown = false;
		}
		if(mouseState.rgbButtons[1] ){
			if(!RightMouseDown){
				RightMouseDown = true;
				//DO STUFF HERE
			}
		}
		else{
			RightMouseDown = false;
		}

		break;
	case 2://Between turns (which this game doesn't have)#############################################
		if(Buffer[DIK_SPACE] & 0x80 || mouseState.rgbButtons[0]){
			if(!m_BoolBuf[DIK_SPACE]){
				m_BoolBuf[DIK_SPACE] = true;
				//DO STUFF HERE
				State = 1;
			}
		}
		else
		{
			m_BoolBuf[DIK_SPACE] = false;
		}
		break;
	case 3://END GAME#####################################
		if(Buffer[DIK_SPACE] & 0x80){
			if(!m_BoolBuf[DIK_SPACE]){
				m_BoolBuf[DIK_SPACE] = true;
				//DO STUFF HERE
				State = 0;
			}
		}
		else
		{
			m_BoolBuf[DIK_SPACE] = false;
		}
		break;
	}
}

void CDirectXFramework::Render()//RENDER
{
	// If the device was not created successfully, return
	if(!m_pD3DDevice)
		return;
	//*************************************************************************

	RECT rect;

	//////////////////////////////////////////////////////////////////////////
	// All draw calls between swap chain's functions, and pre-render and post- 
	// render functions (Clear and Present, BeginScene and EndScene)
	//////////////////////////////////////////////////////////////////////////
	m_pD3DDevice->Clear(0,0,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_ARGB(0,0,0,0), 1,0);
	// Clear the back buffer, call BeginScene()
	m_pD3DDevice->BeginScene();
	std::string UI;
	char c_hlder[256];
	switch(State){
	case 0:
		RECT start;
		start.top = 0;
		start.left = 0;
		start.right = D3Dpp.BackBufferWidth;
		start.bottom = D3Dpp.BackBufferHeight;
		RECT quit;
		quit.top = 100;
		quit.left = 0;
		quit.right = D3Dpp.BackBufferWidth;
		quit.bottom = D3Dpp.BackBufferHeight;
		RECT Windowed_Fullscreen;
		Windowed_Fullscreen.top = 200;
		Windowed_Fullscreen.left = 0;
		Windowed_Fullscreen.right = D3Dpp.BackBufferWidth;
		Windowed_Fullscreen.bottom = D3Dpp.BackBufferHeight;
		if(options[0]){
			m_pD3DFont->DrawTextA(0, "Quit", -1, &quit,
				DT_CENTER | DT_VCENTER, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
			m_pD3DFont->DrawTextA(0, "Windowed/Fullscreen", -1, &Windowed_Fullscreen,
				DT_CENTER | DT_VCENTER, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
			m_pD3DFontLarge->DrawTextA(0, "Start", -1, &start,
				DT_CENTER | DT_VCENTER, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		if(options[1])
		{
			m_pD3DFont->DrawTextA(0, "Start", -1, &start,
				DT_CENTER | DT_VCENTER, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
			m_pD3DFont->DrawTextA(0, "Windowed/Fullscreen", -1, &Windowed_Fullscreen,
				DT_CENTER | DT_VCENTER, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
			m_pD3DFontLarge->DrawTextA(0, "Quit", -1, &quit,
				DT_CENTER | DT_VCENTER, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		if(options[2]){

			m_pD3DFont->DrawTextA(0, "Start", -1, &start,
				DT_CENTER | DT_VCENTER, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
			m_pD3DFontLarge->DrawTextA(0, "Windowed/Fullscreen", -1, &Windowed_Fullscreen,
				DT_CENTER | DT_VCENTER, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
			m_pD3DFont->DrawTextA(0, "Quit", -1, &quit,
				DT_CENTER | DT_VCENTER, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		break;
	case 1:
		m_pD3DSprite->Begin(0);
		//////////////////////////////////////////////////////////////////////////
		// Matrix Transformations to control sprite position, scale, and rotate
		// Set these matrices for each object you want to render to the screen
		//////////////////////////////////////////////////////////////////////////
		



		for(int i =0; i < 10000;i++){
			if(World.getProv(i).m_Nation)
				Pallette[0]->Draw(m_pD3DSprite,m_imageInfoSmall,Pallette[0]->m_Textures[World.getProv(i).mtype],World.getProv(i).m_Nation->m_Flag);
			else
				Pallette[0]->Draw(m_pD3DSprite,m_imageInfoSmall,Pallette[0]->m_Textures[World.getProv(i).mtype]);
		}

		Pallette[0]->DrawArmy(PH,m_pD3DSprite,m_imageInfoUI,Pallette[0]->m_Textures[5]);

		Pallette[1]->Draw(m_pD3DSprite,m_imageInfoUI,Pallette[1]->m_Textures[0],D3DCOLOR_ARGB(255,255,255,255),0.0,0.85,0.94);
		// Scaling
		// Rotation on Z axis, value in radians, converting from degrees
		// Translation
		// Multiply scale and rotation, store in scale
		// Multiply scale and translation, store in world

		// Set Transform


		// Draw the texture with the sprite object

		// End drawing 2D sprites
		m_pD3DSprite->End();

		//////////////////////////////////////////////////////////////////////////
		// Draw Text
		//////////////////////////////////////////////////////////////////////////

		// Calculate RECT structure for text drawing placement, using whole screen
		GetWindowRect(m_hWnd, &rect);
		rect.top = 0;
		rect.left = 0;
		rect.right = D3Dpp.BackBufferWidth;
		rect.bottom = D3Dpp.BackBufferHeight;
		ltoa(mFPS, fps, 10);
		ltoa(Test, Spe, 10);

		m_pD3DFont->DrawTextA(0, fps, -1, &rect,
			DT_BOTTOM | DT_LEFT | DT_NOCLIP, 
			D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pD3DFont->DrawTextA(0, Spe, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pD3DFont->DrawTextA(0, x, -1, &rect,
			DT_BOTTOM | DT_RIGHT | DT_NOCLIP, 
			D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pD3DFont->DrawTextA(0, y, -1, &rect,
			DT_TOP | DT_RIGHT | DT_NOCLIP, 
			D3DCOLOR_ARGB(255, 255, 255, 255));

		rect = Pallette[1]->Me;
		rect.bottom -= 16;
		rect.top += 16;
		rect.left += 16;
		rect.right -= 16;
		if(m_Player){
			m_pD3DFont->DrawTextA(0, Nations[0]->m_Name.c_str(), -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				m_Player->m_Flag);
			UI.append("\n\n\n\nTECHNOLOGY:\n");
			UI.append("Land Tech: ");
			ltoa(m_Player->m_LandTech,c_hlder,10);
			UI.append(c_hlder);
			UI.append(" \nSea Tech: ");
			ltoa(m_Player->m_SeaTech,c_hlder,10);
			UI.append(c_hlder);
			UI.append(" \nEconomy Tech: ");
			ltoa(m_Player->m_EconomyTech,c_hlder,10);
			UI.append(c_hlder);
			UI.append("\n\n\n\n");
			UI.append("ARMY:\n");
			UI.append(" \nAttack: ");
			ltoa(m_Player->ArmyAtk,c_hlder,10);
			UI.append(c_hlder);
			UI.append(" \nDefence: ");
			ltoa(m_Player->ArmyDef,c_hlder,10);
			UI.append(c_hlder);
			UI.append(" \nMorale Attack: ");
			ltoa(m_Player->ArmyMAtk,c_hlder,10);
			UI.append(c_hlder);
			UI.append(" \nMorale Defence: ");
			ltoa(m_Player->ArmyMDef,c_hlder,10);
			UI.append(c_hlder);
			UI.append(" \nMax Morale: ");
			ltoa(m_Player->ArmyMaxMorale,c_hlder,10);
			UI.append(c_hlder);

			m_pD3DFontSmall->DrawTextA(0, UI.c_str(), -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				D3DCOLOR_ARGB(255, 200, 255, 255));
		}



		break;
	case 2:
		GetWindowRect(m_hWnd, &rect);
		rect.top = 0;
		rect.left = 0;
		rect.right = D3Dpp.BackBufferWidth;
		rect.bottom = D3Dpp.BackBufferHeight;
		
		break;
	case 3:
		GetWindowRect(m_hWnd, &rect);
		rect.top = 0;
		rect.left = 0;
		rect.right = D3Dpp.BackBufferWidth;
		rect.bottom = D3Dpp.BackBufferHeight;
		//RESET STUFF

		break;
	}
	// EndScene, and Present the back buffer to the display buffer
	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present(NULL,NULL,NULL,NULL);


	//*************************************************************************

}

void CDirectXFramework::Shutdown()
{
	//*************************************************************************
	// Release COM objects in the opposite order they were created in
	if(m_pDIKeyboard){
		m_pDIKeyboard->Unacquire();
		m_pDIKeyboard->Release();
		m_pDIKeyboard = 0;}
	if(m_pDIMouse){
		m_pDIMouse->Unacquire();
		m_pDIMouse->Release();
		m_pDIMouse = 0;} 
	if(m_pDIObject){
		m_pDIObject->Release();
		m_pDIObject =0;}
	if(m_pTexture){
		// Texture
		m_pTexture->Release();
		m_pTexture= 0;}
	// Sprite

	if( m_pD3DSprite){
		m_pD3DSprite->Release();
		m_pD3DSprite=0;}
	// Font
	if(m_pD3DFontSmall){
		m_pD3DFontSmall->Release();
		m_pD3DFontSmall=0;}
	if(m_pD3DFont){
		m_pD3DFont->Release();
		m_pD3DFont=0;}
	if(m_pD3DFont){
		m_pD3DFontLarge->Release();
		m_pD3DFontLarge=0;}
	// 3DDevice	
	if(m_pD3DDevice){
		m_pD3DDevice->Release();
		m_pD3DDevice=0;}
	// 3DObject
	if(m_pD3DObject){
		m_pD3DObject->Release();
		m_pD3DObject=0;}
	for(int i = 0; i < 3; i++)
		if(Pallette[i]){
			delete Pallette[i];
			Pallette[i]=0;}
	//*************************************************************************
	//Sound
	World.~WorldMap();
	for(int i =0; i < 6; i++)
		if(Sounds[i]){
			result = Sounds[i]->release();
			Sounds[i] = 0;
		}
    result = system->close();
    result = system->release();
}

