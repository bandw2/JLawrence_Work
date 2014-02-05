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
#include <math.h>

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
	EnemyVision = 5;

	Turn =0;
	NumEnt =0;

	Character.pos = V2D(4.0f,4.0f);
	Character.VisionDistance = 4;

	D3DXMatrixTranslation(&PlayerPos,64*4.0f,64*4.0f,0.0f);
	D3DXMATRIX Scale;
	D3DXMATRIX Translation;
	D3DXMatrixTranslation(&Translation,64*10.0f,0.0f,0.0f);
	D3DXMatrixScaling(&Scale,1.5f,0.625f,0.0f);
	UIPos = (Scale*Translation);
	D3DXMatrixTranslation(&WeaponPos,660.0f,20.0f,0.0f);
	D3DXMatrixTranslation(&RangedPos,744.0f,20.0f,0.0f);
	D3DXMatrixTranslation(&UtilPos,660.0f,110.0f,0.0f);
	D3DXMatrixTranslation(&ArmorPos,744.0f,110.0f,0.0f);
	D3DXMATRIX Temp;
	D3DXMatrixScaling(&Temp,0.75f,0.75f,0.0f);
	D3DXMatrixTranslation(&InventoryPos[0],700.0f,350.0f,0.0f);
	InventoryPos[0] = (Temp * InventoryPos[0]);
	D3DXMatrixTranslation(&InventoryPos[1],700.0f,408.0f,0.0f);
	InventoryPos[1] = (Temp * InventoryPos[1]);
	D3DXMatrixTranslation(&InventoryPos[2],700.0f,466.0f,0.0f);
	InventoryPos[2] = (Temp * InventoryPos[2]);
	D3DXMatrixTranslation(&InventoryPos[3],700.0f,524.0f,0.0f);
	InventoryPos[3] = (Temp * InventoryPos[3]);
	D3DXMatrixTranslation(&InventoryPos[4],700.0f,582.0f,0.0f);
	InventoryPos[4] = (Temp * InventoryPos[4]);


	Character.strength = 1;
	Character.dexterity = 1;
	Character.speed = 1;
	Character.intellegence = 1;
	Character.hitpoints = Character.strength * 5;
	Character.Ammo = 2;
	Character.skillPoints = 10;

	Level = 1;
	LevelReq = 10;
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
	m_pD3DFont->OnLostDevice();
	m_pD3DFontLarge->OnLostDevice();
	m_pD3DDevice->Reset(&D3Dpp);
	m_pD3DSprite->OnResetDevice();
	m_pD3DFont->OnResetDevice();
	m_pD3DFontLarge->OnResetDevice();
}

void CDirectXFramework::Init(HWND& hWnd, HINSTANCE& hInst, bool bWindowed)
{
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
	D3DXCreateFont(m_pD3DDevice, 30, 0, FW_BOLD, 0, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"),
		&m_pD3DFont);
	D3DXCreateFont(m_pD3DDevice, 40, 0, FW_BOLD, 0, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"),
		&m_pD3DFontLarge);
	D3DXCreateFont(m_pD3DDevice, 30, 0, FW_BOLD, 0, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("SquareFont"),
		&m_pD3DFontCool);
	D3DXCreateFont(m_pD3DDevice, 15, 0, FW_BOLD, 0, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("SquareFont"),
		&m_pD3DFontCoolFine);
	D3DXCreateFont(m_pD3DDevice, 10, 0, FW_BOLD, 0, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, TEXT("SquareFont"),
		&m_pD3DFontCoolUltraFine);
	// Load D3DXFont, each font style you want to support will need an ID3DXFont



	//////////////////////////////////////////////////////////////////////////
	// Create Sprite Object and Textures
	//////////////////////////////////////////////////////////////////////////

	// Create a sprite object, note you will only need one for all 2D sprites

	D3DXCreateSprite(m_pD3DDevice, &m_pD3DSprite);

	// Create a texture, each different 2D sprite to display to the screen
	// will need a new texture object.  If drawing the same sprite texture
	// multiple times, just call that sprite's Draw() with different 
	// transformation values.

	bool NotDone = true;
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

	result = system->createSound("sword.wav", FMOD_LOOP_OFF, 0, &Sounds[0]);
	result = system->createSound("gun.wav", FMOD_LOOP_OFF, 0, &Sounds[1]);
	result = system->createSound("medkit.wav", FMOD_LOOP_OFF, 0, &Sounds[2]);
	result = system->createSound("explosion.wav", FMOD_LOOP_OFF, 0, &Sounds[3]);
	result = system->createSound("swordmiss.wav", FMOD_LOOP_OFF, 0, &Sounds[4]);
	result = system->createStream("melody.wav", FMOD_LOOP_NORMAL | FMOD_2D, 0, &Sounds[5]);

	system->playSound(FMOD_CHANNEL_FREE,Sounds[5],false,&SChannel[1]);
	/*
	system->playSound(FMOD_CHANNEL_FREE, Sounds[5], false, &SChannel[1]);*/
	//###################### MAP ##################################
	World.Init(m_pD3DDevice);
	//########################## AI ################################

	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Tank.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfo, 0, &MonTex[0]);
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Wolfie.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfo, 0, &MonTex[1]);
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Ferretty.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfo, 0, &MonTex[2]);
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Player.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfo, 0, &PlayerTex);
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"UI.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfo, 0, &UI);
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Sword.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfo, 0, &WeaponTex);
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Gun.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfo, 0, &RangedTex);
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Medkit.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfo, 0, &MedkitTex);
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Goggles.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfo, 0, &GogglesTex);
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Grenade.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfo, 0, &GrenadeTex);
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Armor.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfo, 0, &ArmorTex);
	D3DXCreateTextureFromFileEx(m_pD3DDevice, L"Ladder.png", 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
		D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
		&m_imageInfo, 0, &ExitTex);
	Turn = -1;
	Weapon* Whld;
	Whld = new Weapon();
	Whld->create(1);
	RangedWeapon* Rhld;
	Rhld = new RangedWeapon();
	Rhld->create(1);
	Armor* Ahld;
	Ahld = new Armor();
	Ahld->create(1);
	Utility* Uhld;
	Uhld = new Utility();
	Uhld->create(1);

	Character.meleeWeapon = Whld;
	Character.rangedWeapon = Rhld;
	Character.armor = Ahld;
	Character.utilityItem = Uhld;
	for(int i = 0; i<5; i++){
		ItemGen(Character.inventory[i]);
	}
	int ItemPerLevel = pow(Level,1.5);
	for(int i = 0; i < ItemPerLevel;i++){
		AddLoot();
	}
	LeftMouseDown = false;
	RightMouseDown = false;
	m_Exit.Init(World);
	CalcVision();
}

void CDirectXFramework::Update(float dt)
{
	Item* Holder;
	void* VoidHolder;
	HRESULT result = 0;
	result = m_pDIKeyboard->GetDeviceState(sizeof(Buffer), (LPVOID)&Buffer);
	if(FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_pDIKeyboard->Acquire();
		}
	}


	result = m_pDIMouse->GetDeviceState(sizeof(mouseState), (LPVOID)&mouseState);
	if(FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_pDIMouse->Acquire();
		}
	}



	// Make static so that their values persist across
	// function calls.
	static float numFrames     = 0.0f;
	static float timeElapsed = 0.0f;
	static float Timer = 0.0f;
	static float Counter = 0.0f;
	Counter += 1.0f;
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
	if(hr == -2147024884){
		hr = m_pDIKeyboard->Acquire();
		hr = m_pDIKeyboard->GetDeviceState(sizeof(Buffer), &Buffer);}
	hr = m_pDIMouse->GetDeviceState(sizeof(mouseState),&mouseState);
	if(hr == -2147024884){
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
					State = 1;
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
					EnableFullscreen(D3Dpp.Windowed);
				}
			}
			else{
				m_BoolBuf[DIK_RETURN] = false;
			}
		}/*
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
		 Windowed_Fullscreen.bottom = D3Dpp.BackBufferHeight;*/
		RECT recta;
		GetWindowRect(m_hWnd, &recta);
		width = recta.right - recta.left;
		height = recta.bottom - recta.top;	

		POINT CursorPosa;
		GetCursorPos(&CursorPosa);
		m_Mousex = CursorPosa.x - recta.left;
		m_Mousey = CursorPosa.y - recta.top;





		break;
	case 1://################################################################################################# GAME LOOP UPDATE
		RECT rect;
		GetWindowRect(m_hWnd, &rect);
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;	
		POINT CursorPos;//########### MOUSE POS ########
		GetCursorPos(&CursorPos);
		m_Mousex = CursorPos.x - rect.left;
		m_Mousey = CursorPos.y - rect.top;
		if(Turn > -1){
			if(Counter > SpawnTimerSeconds*mFPS){// ##### TURN #####
				AI(World);
				Counter = 0;
			}
			CalcTurn();
		}
		else
		{
			if(Buffer[DIK_W]){
				if(!m_BoolBuf[DIK_W]){
					m_BoolBuf[DIK_W] = true;				
					if(Buffer[DIK_LSHIFT])
					{
						V2D hold = Character.pos;
						hold.y--;
						for(int i = 0; i < NumEnt; i++){
							if(EntArray[i]->PosLoc == hold){
								Attack(EntArray[i],true);
							}
						}
					}
					else
					{
						if(World.GetTile(Character.pos.x, Character.pos.y-1).isTraversable())
						{
							Displacement.y--;
							Character.pos.y--;
							EndTurn();
							CheckFinish();
						}
					}
				}
			}
			else
			{
				m_BoolBuf[DIK_W] = false;
			}
			if(Buffer[DIK_S]){
				if(!m_BoolBuf[DIK_S]){
					m_BoolBuf[DIK_S] = true;	
					if(Buffer[DIK_LSHIFT])
					{
						V2D hold = Character.pos;
						hold.y++;
						for(int i = 0; i < NumEnt; i++){
							if(EntArray[i]->PosLoc == hold){
								Attack(EntArray[i],true);
							}
						}
					}
					else
					{
						if(World.GetTile(Character.pos.x, Character.pos.y+1).isTraversable())
						{
							Displacement.y++;
							Character.pos.y++;
							EndTurn();
							CheckFinish();
						}
					}
				}
			}
			else
			{
				m_BoolBuf[DIK_S] = false;
			}
			if(Buffer[DIK_A]){
				if(!m_BoolBuf[DIK_A]){
					m_BoolBuf[DIK_A] = true;
					if(Buffer[DIK_LSHIFT])
					{
						V2D hold = Character.pos;
						hold.x--;
						for(int i = 0; i < NumEnt; i++){
							if(EntArray[i]->PosLoc == hold){
								Attack(EntArray[i],true);
							}
						}
					}
					else
					{
						if(World.GetTile(Character.pos.x-1, Character.pos.y).isTraversable())
						{
							Displacement.x--;
							Character.pos.x--;
							EndTurn();
							CheckFinish();
						}
					}
				}
			}
			else
			{
				m_BoolBuf[DIK_A] = false;
			}
			if(Buffer[DIK_D]){
				if(!m_BoolBuf[DIK_D]){
					m_BoolBuf[DIK_D] = true;
					if(Buffer[DIK_LSHIFT])
					{
						V2D hold = Character.pos;
						hold.x++;
						for(int i = 0; i < NumEnt; i++){
							if(EntArray[i]->PosLoc == hold){
								Attack(EntArray[i],true);
							}
						}
					}
					else
					{
						if(World.GetTile(Character.pos.x+1, Character.pos.y).isTraversable())
						{
							Displacement.x++;
							Character.pos.x++;
							EndTurn();
							CheckFinish();
						}
					}
				}
			}
			else
			{
				m_BoolBuf[DIK_D] = false;
			}
			if(Buffer[DIK_UP]){
				if(!m_BoolBuf[DIK_UP]){
					m_BoolBuf[DIK_UP] = true;
					V2D hold = Character.pos;
					hold.y--;
					for(int i = 0; i < NumEnt; i++){
						if(EntArray[i]->PosLoc == hold){
							Attack(EntArray[i],true);
						}
					}
					EndTurn();
				}
			}
			else
			{
				m_BoolBuf[DIK_UP] = false;
			}
			if(Buffer[DIK_DOWN]){
				if(!m_BoolBuf[DIK_DOWN]){
					m_BoolBuf[DIK_DOWN] = true;
					V2D hold = Character.pos;
					hold.y++;
					for(int i = 0; i < NumEnt; i++){
						if(EntArray[i]->PosLoc == hold){
							Attack(EntArray[i],true);
						}
					}
					EndTurn();
				}
			}
			else
			{
				m_BoolBuf[DIK_DOWN] = false;
			}
			if(Buffer[DIK_LEFT]){
				if(!m_BoolBuf[DIK_LEFT]){
					m_BoolBuf[DIK_LEFT] = true;
					V2D hold = Character.pos;
					hold.x--;
					for(int i = 0; i < NumEnt; i++){
						if(EntArray[i]->PosLoc == hold){
							Attack(EntArray[i],true);
						}
					}
					EndTurn();
				}
			}
			else
			{
				m_BoolBuf[DIK_LEFT] = false;
			}
			if(Buffer[DIK_RIGHT]){
				if(!m_BoolBuf[DIK_RIGHT]){
					m_BoolBuf[DIK_RIGHT] = true;
					V2D hold = Character.pos;
					hold.x++;
					for(int i = 0; i < NumEnt; i++){
						if(EntArray[i]->PosLoc == hold){
							Attack(EntArray[i],true);
						}
					}
					EndTurn();
				}
			}
			else
			{
				m_BoolBuf[DIK_RIGHT] = false;
			}
		}
		//#### LEVEL UP STR ####
		if(Buffer[DIK_O]){
			if(!m_BoolBuf[DIK_O]){
				m_BoolBuf[DIK_O] = true;
				if(Character.skillPoints){
					Character.strength++;
					Character.skillPoints--;
					Character.hitpoints += 5;
				}
			}
		}
		else
		{
			m_BoolBuf[DIK_O] = false;
		}
		//#### LEVEL UP DEX
		if(Buffer[DIK_P]){
			if(!m_BoolBuf[DIK_P]){
				m_BoolBuf[DIK_P] = true;
				if(Character.skillPoints){
					Character.dexterity++;
					Character.skillPoints--;
				}
			}
		}
		else
		{
			m_BoolBuf[DIK_P] = false;
		}
		//#### LEVEL UP SPD ####
		if(Buffer[DIK_K]){
			if(!m_BoolBuf[DIK_K]){
				m_BoolBuf[DIK_K] = true;
				if(Character.skillPoints){
					Character.speed++;
					Character.skillPoints--;
				}
			}
		}
		else
		{
			m_BoolBuf[DIK_K] = false;
		}
		//#### LEVEL UP INT ####
		if(Buffer[DIK_L]){
			if(!m_BoolBuf[DIK_L]){
				m_BoolBuf[DIK_L] = true;
				if(Character.skillPoints){
					Character.intellegence++;
					Character.skillPoints--;
				}
			}
		}
		else
		{
			m_BoolBuf[DIK_L] = false;
		}
		//#### DEBUG LEVEL CREATION ####
		if(Buffer[DIK_M]){
			if(!m_BoolBuf[DIK_M]){
				m_BoolBuf[DIK_M] = true;
				World.NextLevel();
			}
		}
		else
		{
			m_BoolBuf[DIK_M] = false;
		}
		//##### EQUIP INVENTORY #####
		if(Buffer[DIK_1]){
			if(!m_BoolBuf[DIK_1]){
				m_BoolBuf[DIK_1] = true;
				if(Buffer[DIK_LSHIFT]){
					Pickup(0);
				}
				else if(Buffer[DIK_LCONTROL])
				{
					if(Character.skillPoints){
						Character.strength++;
						Character.skillPoints--;
						Character.hitpoints += 5;
					}
				}
				else{
					Holder = (Item*)Character.inventory[0];
					if(Holder->IntReq <= Character.intellegence){
						switch(Holder->Type){
						case 0:
							VoidHolder = Character.inventory[0];
							Character.inventory[0] = Character.meleeWeapon;
							Character.meleeWeapon = (Weapon*)VoidHolder;
							break;
						case 1:
							VoidHolder = Character.inventory[0];
							Character.inventory[0] = Character.rangedWeapon;
							Character.rangedWeapon = (RangedWeapon*)VoidHolder;
							break;
						case 2:
							VoidHolder = Character.inventory[0];
							Character.inventory[0] = Character.armor;
							Character.armor = (Armor*)VoidHolder;
							break;
						default:
							VoidHolder = Character.inventory[0];
							Character.inventory[0] = Character.utilityItem;
							Character.utilityItem = (Utility*)VoidHolder;
							break;
						}
					}
				}
			}
		}
		else
		{
			m_BoolBuf[DIK_1] = false;
		}
		if(Buffer[DIK_2]){
			if(!m_BoolBuf[DIK_2]){
				m_BoolBuf[DIK_2] = true;
				if(Buffer[DIK_LSHIFT]){
					Pickup(1);
				}
				else if(Buffer[DIK_LCONTROL])
				{
					if(Character.skillPoints){
						Character.dexterity++;
						Character.skillPoints--;
					}
				}
				else{
					Holder = (Item*)Character.inventory[1];
					if(Holder->IntReq <= Character.intellegence){
						switch(Holder->Type){
						case 0:
							VoidHolder = Character.inventory[1];
							Character.inventory[1] = Character.meleeWeapon;
							Character.meleeWeapon = (Weapon*)VoidHolder;
							break;
						case 1:
							VoidHolder = Character.inventory[1];
							Character.inventory[1] = Character.rangedWeapon;
							Character.rangedWeapon = (RangedWeapon*)VoidHolder;
							break;
						case 2:
							VoidHolder = Character.inventory[1];
							Character.inventory[1] = Character.armor;
							Character.armor = (Armor*)VoidHolder;
							break;
						default:
							VoidHolder = Character.inventory[1];
							Character.inventory[1] = Character.utilityItem;
							Character.utilityItem = (Utility*)VoidHolder;
							break;
						}
					}
				}
			}
		}
		else
		{
			m_BoolBuf[DIK_2] = false;
		}
		if(Buffer[DIK_3]){
			if(!m_BoolBuf[DIK_3]){
				m_BoolBuf[DIK_3] = true;
				if(Buffer[DIK_LSHIFT]){
					Pickup(2);
				}
				else if(Buffer[DIK_LCONTROL])
				{
					if(Character.skillPoints){
						Character.intellegence++;
						Character.skillPoints--;
					}
				}
				else{
					Holder = (Item*)Character.inventory[2];
					if(Holder->IntReq <= Character.intellegence){
						switch(Holder->Type){
						case 0:
							VoidHolder = Character.inventory[2];
							Character.inventory[2] = Character.meleeWeapon;
							Character.meleeWeapon = (Weapon*)VoidHolder;
							break;
						case 1:
							VoidHolder = Character.inventory[2];
							Character.inventory[2] = Character.rangedWeapon;
							Character.rangedWeapon = (RangedWeapon*)VoidHolder;
							break;
						case 2:
							VoidHolder = Character.inventory[2];
							Character.inventory[2] = Character.armor;
							Character.armor = (Armor*)VoidHolder;
							break;
						default:
							VoidHolder = Character.inventory[2];
							Character.inventory[2] = Character.utilityItem;
							Character.utilityItem = (Utility*)VoidHolder;
							break;
						}
					}
				}
			}
		}
		else
		{
			m_BoolBuf[DIK_3] = false;
		}
		if(Buffer[DIK_4]){
			if(!m_BoolBuf[DIK_4]){
				m_BoolBuf[DIK_4] = true;
				if(Buffer[DIK_LSHIFT]){
					Pickup(3);
				}
				else if(Buffer[DIK_LCONTROL])
				{
					if(Character.skillPoints){
						Character.speed++;
						Character.skillPoints--;
					}
				}
				else{
					Holder = (Item*)Character.inventory[3];
					if(Holder->IntReq <= Character.intellegence){
						switch(Holder->Type){
						case 0:
							VoidHolder = Character.inventory[3];
							Character.inventory[3] = Character.meleeWeapon;
							Character.meleeWeapon = (Weapon*)VoidHolder;
							break;
						case 1:
							VoidHolder = Character.inventory[3];
							Character.inventory[3] = Character.rangedWeapon;
							Character.rangedWeapon = (RangedWeapon*)VoidHolder;
							break;
						case 2:
							VoidHolder = Character.inventory[3];
							Character.inventory[3] = Character.armor;
							Character.armor = (Armor*)VoidHolder;
							break;
						default:
							VoidHolder = Character.inventory[3];
							Character.inventory[3] = Character.utilityItem;
							Character.utilityItem = (Utility*)VoidHolder;
							break;
						}
					}
				}
			}
		}
		else
		{
			m_BoolBuf[DIK_4] = false;
		}
		if(Buffer[DIK_5]){
			if(!m_BoolBuf[DIK_5]){
				m_BoolBuf[DIK_5] = true;
				if(Buffer[DIK_LSHIFT]){
					Pickup(4);
				}
				else{
					Holder = (Item*)Character.inventory[4];
					if(Holder->IntReq <= Character.intellegence){
						switch(Holder->Type){
						case 0:
							VoidHolder = Character.inventory[4];
							Character.inventory[4] = Character.meleeWeapon;
							Character.meleeWeapon = (Weapon*)VoidHolder;
							break;
						case 1:
							VoidHolder = Character.inventory[4];
							Character.inventory[4] = Character.rangedWeapon;
							Character.rangedWeapon = (RangedWeapon*)VoidHolder;
							break;
						case 2:
							VoidHolder = Character.inventory[4];
							Character.inventory[4] = Character.armor;
							Character.armor = (Armor*)VoidHolder;
							break;
						default:
							VoidHolder = Character.inventory[4];
							Character.inventory[4] = Character.utilityItem;
							Character.utilityItem = (Utility*)VoidHolder;
							break;
						}
					}
				}
			}
		}
		else
		{
			m_BoolBuf[DIK_5] = false;
		}

		if(Buffer[DIK_Q]){
			if(!m_BoolBuf[DIK_Q]){
				m_BoolBuf[DIK_Q] = true;
				if(Character.utilityItem->Type == 3 && Character.utilityItem->uses > 0){
					Character.hitpoints += Character.utilityItem->heal;
					Character.utilityItem->uses--;
					system->playSound(FMOD_CHANNEL_FREE, Sounds[2], false, &SChannel[0]);
				}
			}
		}
		else
		{
			m_BoolBuf[DIK_Q] = false;
		}
		//if(Timer >= 5.0f)
		//ShowMessage = false;
		for(int i = 0; i < NumEnt;i++){
			if(EntArray[i]->ME.IsCursorOnMe(m_Mousex,m_Mousey) && mouseState.rgbButtons[0])
			{
				if(!LeftMouseDown)
				{
					Attack(EntArray[i],false);
					LeftMouseDown = true;
				}
			}
			else if(EntArray[i]->ME.IsCursorOnMe(m_Mousex,m_Mousey) && mouseState.rgbButtons[1])
			{
				if(!RightMouseDown)
				{
					Grenade(EntArray[i]->PosLoc.x,EntArray[i]->PosLoc.y);
					RightMouseDown = true;
				}
			}
		}
		if((LeftMouseDown || RightMouseDown) && (!mouseState.rgbButtons[0] || !mouseState.rgbButtons[1]))
		{
			LeftMouseDown = false;
			RightMouseDown = false;
		}


		if(Score > LevelReq){
			Character.skillPoints++;
			LevelReq *= 2;
		}

		World.Reset();
		system->update();
		Timer += dt;
		break;
	case 2://END GAME ############################################# TODO make map reform between levels
		if(Buffer[DIK_SPACE] & 0x80){
			if(!m_BoolBuf[DIK_SPACE]){
				m_BoolBuf[DIK_SPACE] = true;
				State = 0;
			}
		}
		else 
		{
			m_BoolBuf[DIK_SPACE] = false;
		}
		break;
	}


	if(Character.hitpoints > (Character.strength+Character.armor->statBoost[0]) * 5){
		Character.hitpoints = (Character.strength+Character.armor->statBoost[0]) * 5;
	}
	if(Character.hitpoints <= 0){
		State = 2;
	}
}

void CDirectXFramework::Render()//##### RENDER
{
	Item* Holder;
	// If the device was not created successfully, return
	if(!m_pD3DDevice)
		return;
	//*************************************************************************

	RECT rect;
	D3DCOLOR UItext = D3DCOLOR_ARGB(255, 200, 100, 255);

	//////////////////////////////////////////////////////////////////////////
	// All draw calls between swap chain's functions, and pre-render and post- 
	// render functions (Clear and Present, BeginScene and EndScene)
	//////////////////////////////////////////////////////////////////////////
	m_pD3DDevice->Clear(0,0,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_ARGB(0,0,0,0), 1,0);
	// Clear the back buffer, call BeginScene()
	m_pD3DDevice->BeginScene();
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
		m_pD3DSprite->Begin(D3DXSPRITE_ALPHABLEND);
		//////////////////////////////////////////////////////////////////////////
		// Matrix Transformations to control sprite position, scale, and rotate
		// Set these matrices for each object you want to render to the screen
		//////////////////////////////////////////////////////////////////////////
		//m_pD3DSprite->GetTransform(

		World.Draw(m_pD3DSprite, m_imageInfo, Displacement);
		for(int i =0; i < NumEnt; i++){
			if(World.GetTile(EntArray[i]->PosLoc.x,EntArray[i]->PosLoc.y).isVisable()){
				EntArray[i]->Draw(m_pD3DSprite,MonTex[EntArray[i]->Type],m_imageInfo,Displacement);
			}
		}
		if(World.GetTile(m_Exit.Loc.x,m_Exit.Loc.y).isVisable()){
			m_Exit.Draw(m_pD3DSprite,m_imageInfo,Displacement,ExitTex);
		}
		m_pD3DSprite->SetTransform(&PlayerPos);
		m_pD3DSprite->Draw(PlayerTex, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
			&D3DXVECTOR3(0,0, 0.0f),
			D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pD3DSprite->SetTransform(&UIPos);
		m_pD3DSprite->Draw(UI, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
			&D3DXVECTOR3(0,0, 0.0f),
			D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pD3DSprite->SetTransform(&WeaponPos);
		m_pD3DSprite->Draw(WeaponTex, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
			&D3DXVECTOR3(0,0, 0.0f),
			D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pD3DSprite->SetTransform(&RangedPos);
		m_pD3DSprite->Draw(RangedTex, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
			&D3DXVECTOR3(0,0, 0.0f),
			D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pD3DSprite->SetTransform(&UtilPos);
		switch(Character.utilityItem->Type){
		case 3:
			m_pD3DSprite->Draw(MedkitTex, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
				&D3DXVECTOR3(0,0, 0.0f),
				D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case 4:
			m_pD3DSprite->Draw(GrenadeTex, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
				&D3DXVECTOR3(0,0, 0.0f),
				D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case 5:
			m_pD3DSprite->Draw(GogglesTex, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
				&D3DXVECTOR3(0,0, 0.0f),
				D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		}
		m_pD3DSprite->SetTransform(&ArmorPos);
		m_pD3DSprite->Draw(ArmorTex, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
			&D3DXVECTOR3(0,0, 0.0f),
			D3DCOLOR_ARGB(255, 255, 255, 255));
		for(int i = 0;i < 5;i++){
			if(Character.inventory[i]){
				m_pD3DSprite->SetTransform(&InventoryPos[i]);
				Holder = (Item*)Character.inventory[i];
				switch(Holder->Type){
				case 0:
					m_pD3DSprite->Draw(WeaponTex, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
						&D3DXVECTOR3(0,0, 0.0f),
						D3DCOLOR_ARGB(255, 255, 255, 255));
					break;
				case 1:
					m_pD3DSprite->Draw(RangedTex, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
						&D3DXVECTOR3(0,0, 0.0f),
						D3DCOLOR_ARGB(255, 255, 255, 255));
					break;
				case 2:
					m_pD3DSprite->Draw(ArmorTex, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
						&D3DXVECTOR3(0,0, 0.0f),
						D3DCOLOR_ARGB(255, 255, 255, 255));
					break;
				case 3:
					m_pD3DSprite->Draw(MedkitTex, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
						&D3DXVECTOR3(0,0, 0.0f),
						D3DCOLOR_ARGB(255, 255, 255, 255));
					break;
				case 4:
					m_pD3DSprite->Draw(GrenadeTex, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
						&D3DXVECTOR3(0,0, 0.0f),
						D3DCOLOR_ARGB(255, 255, 255, 255));
					break;
				case 5:
					m_pD3DSprite->Draw(GogglesTex, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
						&D3DXVECTOR3(0,0, 0.0f),
						D3DCOLOR_ARGB(255, 255, 255, 255));
					break;
				}
			}
			for(int i = 0; i < NumLoot;i++){
				if(World.GetTile(LootArray[i]->PosLoc.x,LootArray[i]->PosLoc.y).isVisable()){
					Holder = (Item*)LootArray[i]->m_Item;
					switch(Holder->Type){
					case 0:
						LootArray[i]->Draw(m_pD3DSprite,m_imageInfo,Displacement,WeaponTex);
						break;
					case 1:
						LootArray[i]->Draw(m_pD3DSprite,m_imageInfo,Displacement,RangedTex);
						break;
					case 2:
						LootArray[i]->Draw(m_pD3DSprite,m_imageInfo,Displacement,ArmorTex);
						break;
					case 3:
						LootArray[i]->Draw(m_pD3DSprite,m_imageInfo,Displacement,MedkitTex);
						break;
					case 4:
						LootArray[i]->Draw(m_pD3DSprite,m_imageInfo,Displacement,GrenadeTex);
						break;
					case 5:
						LootArray[i]->Draw(m_pD3DSprite,m_imageInfo,Displacement,GogglesTex);
						break;
					}
				}
			}
		}


		// Scaling
		// Rotation on Z axis, value in radians, converting from degrees
		// Translation
		// Multiply scale and rotation, store in scale
		// Multiply scale and translation, store in world

		// Set Transform
		// End drawing 2D sprites
		m_pD3DSprite->End();

		//////////////////////////////////////////////////////////////////////////
		// Draw Text
		//////////////////////////////////////////////////////////////////////////

		// Calculate RECT structure for text drawing placement, using whole screen
		//########## Health ######
		rect.top = 220;
		rect.left = 680;
		ltoa(Character.hitpoints,Health_c,10);
		m_pD3DFontCool->DrawTextA(0, Health_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			D3DCOLOR_ARGB(255, 255, 255, 255));
		//########## Ammo ######
		rect.top = 220;
		rect.left = 780;
		ltoa(Character.Ammo,Ammo_c,10);
		m_pD3DFontCool->DrawTextA(0, Ammo_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			D3DCOLOR_ARGB(255, 255, 255, 255));
		//########## Str ######
		rect.top = 250;
		rect.left = 730;
		ltoa(Character.strength,Str_c,10);
		m_pD3DFontCool->DrawTextA(0, Str_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			D3DCOLOR_ARGB(255, 255, 255, 255));
		if(Character.armor->statBoost[0]){
			rect.left = 760;
			ltoa(Character.armor->statBoost[0],Str_c,10);
			for(int i = 255; i > 0; i--){
				Str_c[i] = Str_c[i-1];
			}
			Str_c[0] = '+';
			m_pD3DFontCool->DrawTextA(0, Str_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		//########## Dex ######
		rect.top = 275;
		rect.left = 730;
		ltoa(Character.dexterity,Dex_c,10);
		m_pD3DFontCool->DrawTextA(0, Dex_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			D3DCOLOR_ARGB(255, 255, 255, 255));
		if(Character.armor->statBoost[1]){
			rect.left = 760;
			ltoa(Character.armor->statBoost[1],Dex_c,10);			
			for(int i = 255; i > 0; i--){
				Dex_c[i] = Dex_c[i-1];
			}
			Dex_c[0] = '+';
			m_pD3DFontCool->DrawTextA(0, Dex_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		//########## Int ######
		rect.top = 300;
		rect.left = 730;
		ltoa(Character.intellegence,Int_c,10);
		m_pD3DFontCool->DrawTextA(0, Int_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			D3DCOLOR_ARGB(255, 255, 255, 255));
		if(Character.armor->statBoost[2]){
			rect.left = 760;
			ltoa(Character.armor->statBoost[2],Int_c,10);
			for(int i = 255; i > 0; i--){
				Int_c[i] = Int_c[i-1];
			}
			Int_c[0] = '+';
			m_pD3DFontCool->DrawTextA(0, Int_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		//########## Spd ######
		rect.top = 325;
		rect.left = 730;
		ltoa(Character.speed,Spd_c,10);
		m_pD3DFontCool->DrawTextA(0, Spd_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			D3DCOLOR_ARGB(255, 255, 255, 255));
		if(Character.armor->statBoost[3]){
			rect.left = 760;
			ltoa(Character.armor->statBoost[3],Spd_c,10);
			for(int i = 255; i > 0; i--){
				Spd_c[i] = Spd_c[i-1];
			}
			Spd_c[0] = '+';
			m_pD3DFontCool->DrawTextA(0, Spd_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		//########## SkillPoints ######
		if(Character.skillPoints){
			rect.top = 235;
			rect.left = 640;
			LPCSTR plus;
			plus = "+";
			m_pD3DFontCool->DrawTextA(0, plus, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		for(int i = 0; i < NumEnt; i++){
			if(EntArray[i]->ME.IsCursorOnMe(m_Mousex,m_Mousey)){
				rect.top = m_Mousey+10;
				rect.left = m_Mousex+10;
				m_pD3DFontCoolFine->DrawTextA(0, "HP:", -1, &rect,
					DT_TOP | DT_LEFT | DT_NOCLIP, 
					D3DCOLOR_ARGB(255, 255, 0, 0));
				rect.top = m_Mousey+10;
				rect.left = m_Mousex+40;
				ltoa(EntArray[i]->HitPoints,Int_c,10);
				m_pD3DFontCoolFine->DrawTextA(0, Int_c, -1, &rect,
					DT_TOP | DT_LEFT | DT_NOCLIP, 
					D3DCOLOR_ARGB(255, 255, 0, 0));


				rect.top = m_Mousey+30;
				rect.left = m_Mousex+10;
				m_pD3DFontCoolFine->DrawTextA(0, "Hit%:", -1, &rect,
					DT_TOP | DT_LEFT | DT_NOCLIP, 
					D3DCOLOR_ARGB(255, 0, 255, 0));
				rect.top = m_Mousey+30;
				rect.left = m_Mousex+50;
				ltoa((Character.dexterity+Character.armor->statBoost[1] - EntArray[i]->Speed)+50,Int_c,10);
				m_pD3DFontCoolFine->DrawTextA(0, Int_c, -1, &rect,
					DT_TOP | DT_LEFT | DT_NOCLIP, 
					D3DCOLOR_ARGB(255, 0, 255, 0));
			}
		}
		rect.top =0;
		rect.left = 0;
		if(Turn == -1){
			m_pD3DFontCool->DrawTextA(0, "PLAYERS TURN", -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				D3DCOLOR_ARGB(255, 0, 255, 0));
		}
		else
		{
			m_pD3DFontCool->DrawTextA(0, "ENEMIES' TURN", -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				D3DCOLOR_ARGB(255, 255, 0, 0));
		}
		//##### TEXT DRAW
		if(WeaponPos){
			rect.top = WeaponPos._42;
			rect.left = WeaponPos._41;

			m_pD3DFontCoolFine->DrawTextA(0, "Sword \nDmg: \nA-Spd: \nIntReq:", -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			rect.left += 50;
			rect.top += 15;
			ltoa(Character.meleeWeapon->damage,Int_c,10);
			m_pD3DFontCoolFine->DrawTextA(0, Int_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			rect.top += 15;
			ltoa(Character.meleeWeapon->attackSpeed,Int_c,10);
			m_pD3DFontCoolFine->DrawTextA(0, Int_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			rect.top += 15;
			ltoa(Character.meleeWeapon->IntReq,Int_c,10);
			m_pD3DFontCoolFine->DrawTextA(0, Int_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
		}
		if(RangedPos){
			rect.top = RangedPos._42;
			rect.left = RangedPos._41;

			m_pD3DFontCoolFine->DrawTextA(0, "Gun \nDmg: \nA-Spd: \nRange: \nIntReq: ", -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			rect.left += 50;
			rect.top += 15;
			ltoa(Character.rangedWeapon->damage,Int_c,10);
			m_pD3DFontCoolFine->DrawTextA(0, Int_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			rect.top += 15;
			ltoa(Character.rangedWeapon->fireRate,Int_c,10);
			m_pD3DFontCoolFine->DrawTextA(0, Int_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			rect.top += 15;
			ltoa(Character.rangedWeapon->range,Int_c,10);
			m_pD3DFontCoolFine->DrawTextA(0, Int_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			rect.top += 15;
			ltoa(Character.rangedWeapon->IntReq,Int_c,10);
			m_pD3DFontCoolFine->DrawTextA(0, Int_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
		}
		if(ArmorPos){
			rect.top = ArmorPos._42;
			rect.left = ArmorPos._41;

			m_pD3DFontCoolFine->DrawTextA(0, "Armor \nValue: ", -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			rect.left += 50;
			rect.top += 15;
			ltoa(Character.armor->value,Int_c,10);
			m_pD3DFontCoolFine->DrawTextA(0, Int_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
		}

		if(UtilPos){
			rect.top = UtilPos._42;
			rect.left = UtilPos._41;
		}
		switch(Character.utilityItem->Type){
		case 3:
			m_pD3DFontCoolFine->DrawTextA(0, "Medkit \nHeal: \nUses:", -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			rect.left += 50;
			rect.top += 15;
			ltoa(Character.utilityItem->heal,Int_c,10);
			m_pD3DFontCoolFine->DrawTextA(0, Int_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			rect.top += 15;
			ltoa(Character.utilityItem->uses,Int_c,10);
			m_pD3DFontCoolFine->DrawTextA(0, Int_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			break;
		case 4:
			m_pD3DFontCoolFine->DrawTextA(0, "Grenade \nDmg: \nUses: \nRange:", -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			rect.left += 50;
			rect.top += 15;
			ltoa(Character.utilityItem->damageOnHit,Int_c,10);
			m_pD3DFontCoolFine->DrawTextA(0, Int_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			rect.top += 15;
			ltoa(Character.utilityItem->uses,Int_c,10);
			m_pD3DFontCoolFine->DrawTextA(0, Int_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			rect.top += 15;
			ltoa(Character.utilityItem->splashRange,Int_c,10);
			m_pD3DFontCoolFine->DrawTextA(0, Int_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			break;
		case 5:
			m_pD3DFontCoolFine->DrawTextA(0, "Goggles \nVision: ", -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			rect.left += 50;
			rect.top += 15;
			ltoa(Character.utilityItem->vision,Int_c,10);
			m_pD3DFontCoolFine->DrawTextA(0, Int_c, -1, &rect,
				DT_TOP | DT_LEFT | DT_NOCLIP, 
				UItext);
			break;
		}

		for(int i = 0; i < 5; i++){
			ItemText(Character.inventory[i],InventoryPos[i]._41,InventoryPos[i]._42,UItext);
		}



		//if(LootArray[0]){ //DEBUG CODE FOR DRAW
		//	rect.top = 0;
		//	rect.left = 0;
		//	ltoa((LootArray[0]->PosLoc.x-Character.pos.x),Int_c,10);
		//	m_pD3DFontCool->DrawTextA(0, Int_c, -1, &rect,
		//		DT_TOP | DT_LEFT | DT_NOCLIP, 
		//		D3DCOLOR_ARGB(255, 255, 0, 0));
		//	rect.top = 35;
		//	rect.left = 0;
		//	ltoa((LootArray[0]->PosLoc.y-Character.pos.y),Int_c,10);
		//	m_pD3DFontCool->DrawTextA(0, Int_c, -1, &rect,
		//		DT_TOP | DT_LEFT | DT_NOCLIP, 
		//		D3DCOLOR_ARGB(255, 255, 0, 0));
		//}
		break;
	case 2:
		GetWindowRect(m_hWnd, &rect);
		rect.top = 0;
		rect.left = 0;
		rect.right = D3Dpp.BackBufferWidth;
		rect.bottom = D3Dpp.BackBufferHeight;
		m_pD3DFontCool->DrawTextA(0, "YOU LOSE", -1, &rect,
			DT_CENTER | DT_VCENTER | DT_NOCLIP, 
			D3DCOLOR_ARGB(255, 255, 0, 0));
		rect.top = 70;
		rect.left = 0;
		rect.right = D3Dpp.BackBufferWidth;
		rect.bottom = D3Dpp.BackBufferHeight;
		char word[255];
		ltoa(Score, word,10);	
		for(int i = 254; i > 6;i--){
			word[i] = word[i-7];
		}
		word[0] = 'S';
		word[1] = 'C';
		word[2] = 'O';
		word[3] = 'R';
		word[4] = 'E';
		word[5] = ':';
		word[6] = ' ';
		m_pD3DFontCool->DrawTextA(0, word, -1, &rect,
			DT_CENTER | DT_VCENTER | DT_NOCLIP, 
			D3DCOLOR_ARGB(255, 255, 0, 0));
	}
	// EndScene, and Present the back buffer to the display buffer
	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present(NULL,NULL,NULL,NULL);
}


//*************************************************************************

//}

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
		m_pDIObject =0;
	}
	// Sprite
	World.Shutdown();
	if(m_pD3DSprite){
		m_pD3DSprite->Release();
		m_pD3DSprite=0;}
	// Font
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
	//*************************************************************************
	//Sound

	for(int i =0; i < 6; i++)
	{
		if(Sounds[i])
			result = Sounds[i]->release();
		Sounds[i] = 0;
	}
	result = system->close();
	result = system->release();
}

//**************************AI****************************************

void CDirectXFramework::AddAI(){
	if(NumEnt < MaxEnt){
		EntArray[NumEnt] = new Enemy;
		EntArray[NumEnt]->CreateEnemy(Level);
		EntArray[NumEnt]->SetLoc(m_pD3DDevice,World);
		NumEnt++;
	}
}

void CDirectXFramework::AddLoot(){
	if(NumLoot < MaxLoot){
		LootArray[NumLoot] = new Loot;
		ItemGen(LootArray[NumLoot]->m_Item);
		LootArray[NumLoot]->Place(World);
		NumLoot++;
	}
}


void CDirectXFramework::AI(Map &World){
	if(Turn > -1 && EntArray[Turn])
	{
		Tile ME;
		ME = World.GetTile(EntArray[Turn]->PosLoc.x,EntArray[Turn]->PosLoc.y);
		int count = 0;
		bool NotDone = true;
		switch(CalcMove(EntArray[Turn]->PosLoc))
		{
		case 0:
			while(NotDone && count < 4){
				int  num = rand()%4;
				if(World.GetTile(ME.Neighbors[num]).isTraversable() && World.GetTile(ME.Neighbors[num]).Vacant)
				{
					World.GetTile(EntArray[Turn]->PosLoc.x,EntArray[Turn]->PosLoc.y).Vacant = true;
					EntArray[Turn]->PosLoc =  World.GetTile(ME.Neighbors[num]).Loc;
					NotDone = false;
					World.GetTile(EntArray[Turn]->PosLoc.x,EntArray[Turn]->PosLoc.y).Vacant = false;
					CalcVision();
				}
				count++;
			}
			break;
		case 1:
			if(World.GetTile(ME.Neighbors[0]).Loc == Character.pos){
				AIAttack(*EntArray[Turn]);
				break;
			}
			World.GetTile(EntArray[Turn]->PosLoc.x,EntArray[Turn]->PosLoc.y).Vacant = true;
			EntArray[Turn]->PosLoc =  World.GetTile(ME.Neighbors[0]).Loc;
			World.GetTile(EntArray[Turn]->PosLoc.x,EntArray[Turn]->PosLoc.y).Vacant = false;
			CalcVision();
			break;
		case 2:
			if(World.GetTile(ME.Neighbors[1]).Loc == Character.pos){
				AIAttack(*EntArray[Turn]);
				break;
			}
			World.GetTile(EntArray[Turn]->PosLoc.x,EntArray[Turn]->PosLoc.y).Vacant = true;
			EntArray[Turn]->PosLoc =  World.GetTile(ME.Neighbors[1]).Loc;
			World.GetTile(EntArray[Turn]->PosLoc.x,EntArray[Turn]->PosLoc.y).Vacant = false;
			CalcVision();
			break;
		case 3:
			if(World.GetTile(ME.Neighbors[2]).Loc == Character.pos){
				AIAttack(*EntArray[Turn]);
				break;
			}
			World.GetTile(EntArray[Turn]->PosLoc.x,EntArray[Turn]->PosLoc.y).Vacant = true;
			EntArray[Turn]->PosLoc =  World.GetTile(ME.Neighbors[2]).Loc;
			World.GetTile(EntArray[Turn]->PosLoc.x,EntArray[Turn]->PosLoc.y).Vacant = false;
			CalcVision();
			break;
		case 4:
			if(World.GetTile(ME.Neighbors[3]).Loc == Character.pos){
				AIAttack(*EntArray[Turn]);
				break;
			}
			World.GetTile(EntArray[Turn]->PosLoc.x,EntArray[Turn]->PosLoc.y).Vacant = true;
			EntArray[Turn]->PosLoc =  World.GetTile(ME.Neighbors[3]).Loc;
			World.GetTile(EntArray[Turn]->PosLoc.x,EntArray[Turn]->PosLoc.y).Vacant = false;
			CalcVision();
			break;
		}
		EntArray[Turn]->AccumulatedSpeed = 0;
	}
}

void CDirectXFramework::CalcVision(){
	World.ResetVision();
	queue<int,deque<int>> Vision;
	int Held;
	World.GetTile(Character.pos.x,Character.pos.y).SetVisable(true);
	World.GetTile(Character.pos.x,Character.pos.y).setVisited(true);
	for(int i = 0; i < 4; i++){
		if(Character.utilityItem->Type == 5)
			World.GetTile(World.GetTile(Character.pos.x,Character.pos.y).Neighbors[i]).setDistance(Character.VisionDistance+Character.utilityItem->vision);
		else
			World.GetTile(World.GetTile(Character.pos.x,Character.pos.y).Neighbors[i]).setDistance(Character.VisionDistance);
		Held = World.GetTile(Character.pos.x,Character.pos.y).Neighbors[i];
		Vision.push(Held);
	}
	while(!Vision.empty()){
		World.GetTile(Vision.front()).SetVisable(true);
		World.GetTile(Vision.front()).setVisited(true);
		if(World.GetTile(Vision.front()).isTraversable() && World.GetTile(Vision.front()).getDistance() != 0 && World.GetTile(Vision.front()).Vacant){
			for(int i = 0; i < 4; i++){
				if(!World.GetTile(World.GetTile(Vision.front()).Neighbors[i]).getVisited()){
					World.GetTile(World.GetTile(Vision.front()).Neighbors[i]).setDistance(World.GetTile(Vision.front()).getDistance()-1);
					Held = World.GetTile(Vision.front()).Neighbors[i];
					Vision.push(Held);
				}
			}
		}
		Vision.pop();
	}
	World.Reset();
}

int CDirectXFramework::CalcMove(V2D Loc){
	struct Dual{
		int ID;
		int Dir;
	};

	queue<Dual,deque<Dual>> Vision;
	Dual Held;
	World.GetTile(Loc.x,Loc.y).setVisited(true);
	for(int i = 0; i < 4; i++){
		World.GetTile(World.GetTile(Loc.x,Loc.y).Neighbors[i]).setDistance(EnemyVision);
		Held.ID = World.GetTile(Loc.x,Loc.y).Neighbors[i];
		Held.Dir = i+1;
		Vision.push(Held);
	}
	while(!Vision.empty()){
		World.GetTile(Vision.front().ID).setVisited(true);
		if(World.GetTile(Vision.front().ID).getLoc() == Character.pos)
			return Vision.front().Dir;

		if(World.GetTile(Vision.front().ID).isTraversable() && World.GetTile(Vision.front().ID).getDistance() != 0  && World.GetTile(Vision.front().ID).Vacant){
			for(int i = 0; i < 4; i++){
				if(!World.GetTile(World.GetTile(Vision.front().ID).Neighbors[i]).getVisited()){
					World.GetTile(World.GetTile(Vision.front().ID).Neighbors[i]).setDistance(World.GetTile(Vision.front().ID).getDistance()-1);
					Held.ID = World.GetTile(Vision.front().ID).Neighbors[i];
					Held.Dir = Vision.front().Dir;
					Vision.push(Held);
				}
			}
		}
		Vision.pop();
	}
	World.Reset();
	return 0;
}

void CDirectXFramework::AIAttack(Enemy him){
	int hitChance = (Character.dexterity+Character.armor->statBoost[1] - him.Speed)+50;
	if ((rand()%100) > hitChance){
		switch(him.Type){
		case 0:
			if(him.Dmg > Character.armor->value)
			{
				Character.hitpoints -= (him.Dmg-Character.armor->value)*((float)Character.armor->resists[2]/100.0f);
			}
			break;
		case 1:	
			if(him.Dmg > Character.armor->value)
			{
				Character.hitpoints -= him.Dmg-Character.armor->value*((float)Character.armor->resists[1]/100.0f);
			}
			break;
		case 2:			
			if(him.Dmg > Character.armor->value)
			{
				Character.hitpoints -= him.Dmg-Character.armor->value*((float)Character.armor->resists[0]/100.0f);
			}
			break;
		}
	}
}

void CDirectXFramework::Attack(Enemy* him, bool Melee){
	if(Melee){
		int hitChance = (him->Speed - Character.strength+Character.armor->statBoost[0])+50;
		if((rand()%100) > hitChance){
			him->HitPoints -= Character.meleeWeapon->damage+(Character.strength+Character.armor->statBoost[0]/10);
			system->playSound(FMOD_CHANNEL_FREE, Sounds[0], false, &SChannel[0]);
		}
		else system->playSound(FMOD_CHANNEL_FREE, Sounds[4], false, &SChannel[0]);
	}
	else{//Ranged
		int hitChance = (Character.dexterity+Character.armor->statBoost[1] - him->Speed)+50;
		if(Character.rangedWeapon->range >= sqrt((pow(him->PosLoc.x-Character.pos.x,2)+(pow(him->PosLoc.y-Character.pos.y,2)))) && Character.Ammo > 0){
			if(((rand()%100) > hitChance))
			{
				him->HitPoints -= Character.rangedWeapon->damage+((Character.dexterity+Character.armor->statBoost[1])/10);
			}
			Character.Ammo--;
			system->playSound(FMOD_CHANNEL_FREE, Sounds[1], false, &SChannel[0]);
		}
	}

	if(him->HitPoints < 1){
		World.GetTile(him->PosLoc.x,him->PosLoc.y).Vacant = true;
		CalcVision();
		him->dead = true;
		Score += Level*1.25f;
		int counter = 0;
		for(int i = 0; i < NumEnt; i++){
			if(EntArray[i]->dead == true)
				counter = i;					
		}
		for(int i = counter; i < NumEnt-1; i++){
			EntArray[i] = EntArray[i+1];
		}
		NumEnt--;
	}
}

void CDirectXFramework::CalcTurn(){
	Turn = 0;	
	if(NumEnt == 0){
		AddAI();
	}
	for(int i = 0; i < NumEnt; i++){
		EntArray[i]->AccumulatedSpeed += EntArray[i]->Speed;
	}
	Character.AccumulatedSpeed += Character.speed+Character.armor->statBoost[3];

	for(int i = 0; i < NumEnt; i++){
		if(EntArray[i]->AccumulatedSpeed > EntArray[Turn]->AccumulatedSpeed){
			Turn = i;
		}
	}
	if(Character.AccumulatedSpeed >= EntArray[Turn]->AccumulatedSpeed){
		Turn = -1;
	}
}

void CDirectXFramework::EndTurn(){

	Character.AccumulatedSpeed = 0;
	CalcTurn();
	CalcVision();
	SpawnCounter++;
	if(SpawnCounter == 5){
		AddAI();
		SpawnCounter = 0;
	}
}

void CDirectXFramework::CheckFinish(){
	if(Character.pos == m_Exit.Loc){
		Displacement = V2D(0,0);
		Character.pos = V2D(4,4);
		for(int i = 0; i < NumEnt; i++){
			World.GetTile(EntArray[i]->PosLoc.x,EntArray[i]->PosLoc.y).Vacant = true;
			EntArray[i] = NULL;

		}
		NumEnt = 0;
		Score *= 1.1f;
		Level++;
		World.NextLevel();
		int ItemPerLevel = pow(Level,1.5);
		Character.Ammo = Character.strength;
		for(int i = 0; i < ItemPerLevel;i++){
			AddLoot();
		}
		m_Exit.Init(World);
		CalcVision();
	}
}

void CDirectXFramework::ItemGen(void* &Holder){
	int percent = rand()%100;
	if(percent < 10)
	{
		Weapon* NewWeapon;
		NewWeapon = new Weapon;
		NewWeapon->create(Level);
		Holder = NewWeapon;
	}
	else if(percent < 20)
	{
		RangedWeapon* NewWeapon;
		NewWeapon = new RangedWeapon;
		NewWeapon->create(Level);
		Holder = NewWeapon;
	}
	else if(percent < 30)
	{
		Armor* NewArmor;
		NewArmor = new Armor;
		NewArmor->create(Level);
		Holder = NewArmor;
	}
	else{
		Utility* NewUtil;
		NewUtil = new Utility;
		NewUtil->create(Level);
		Holder = NewUtil;
	}

}

void CDirectXFramework::ItemText(void* &Holder, int x,int y, D3DCOLOR UItext){
	Item* ItemHolder = (Item*)Holder;
	Weapon* WeaponHolder;
	RangedWeapon* RangedHolder;
	Armor* ArmorHolder;
	Utility* UtilityHolder;
	RECT rect;
	switch(ItemHolder->Type){
		{
	case 0:
		WeaponHolder = (Weapon*)Holder;
		rect.top = y+7;
		rect.left = x;
		m_pD3DFontCoolUltraFine->DrawTextA(0, "Sword \nDmg: \nA-Spd: \nIntReq:", -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		rect.left += 50;
		rect.top += 10;
		ltoa(WeaponHolder->damage,Int_c,10);
		m_pD3DFontCoolUltraFine->DrawTextA(0, Int_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		rect.top += 10;
		ltoa(WeaponHolder->attackSpeed,Int_c,10);
		m_pD3DFontCoolUltraFine->DrawTextA(0, Int_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		rect.top += 10;
		ltoa(WeaponHolder->IntReq,Int_c,10);
		m_pD3DFontCoolUltraFine->DrawTextA(0, Int_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		break;
	case 1:
		RangedHolder = (RangedWeapon*)Holder;
		rect.top = y+7;
		rect.left = x;
		m_pD3DFontCoolUltraFine->DrawTextA(0, "Gun \nDmg: \nA-Spd: \nRange: \nIntReq: ", -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		rect.left += 50;
		rect.top += 10;
		ltoa(RangedHolder->damage,Int_c,10);
		m_pD3DFontCoolUltraFine->DrawTextA(0, Int_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		rect.top += 10;
		ltoa(RangedHolder->fireRate,Int_c,10);
		m_pD3DFontCoolUltraFine->DrawTextA(0, Int_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		rect.top += 10;
		ltoa(RangedHolder->range,Int_c,10);
		m_pD3DFontCoolUltraFine->DrawTextA(0, Int_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		rect.top += 10;
		ltoa(RangedHolder->IntReq,Int_c,10);
		m_pD3DFontCoolUltraFine->DrawTextA(0, Int_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		break;
	case 2:
		ArmorHolder = (Armor*)Holder;
		rect.top = y+7;
		rect.left = x;
		m_pD3DFontCoolUltraFine->DrawTextA(0, "Armor \nValue: ", -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		rect.left += 50;
		rect.top += 10;
		ltoa(ArmorHolder->value,Int_c,10);
		m_pD3DFontCoolUltraFine->DrawTextA(0, Int_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		break;
	case 3:
		UtilityHolder = (Utility*)Holder;
		rect.top = y+7;
		rect.left = x;
		m_pD3DFontCoolUltraFine->DrawTextA(0, "Medkit \nHeal: \nUses:", -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		rect.left += 50;
		rect.top += 10;
		ltoa(UtilityHolder->heal,Int_c,10);
		m_pD3DFontCoolUltraFine->DrawTextA(0, Int_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		rect.top += 10;
		ltoa(UtilityHolder->uses,Int_c,10);
		m_pD3DFontCoolUltraFine->DrawTextA(0, Int_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		break;
	case 4:
		UtilityHolder = (Utility*)Holder;
		rect.top = y+7;
		rect.left = x;
		m_pD3DFontCoolUltraFine->DrawTextA(0, "Grenade \nDmg: \nUses: \nRange:", -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		rect.left += 50;
		rect.top += 10;
		ltoa(UtilityHolder->damageOnHit,Int_c,10);
		m_pD3DFontCoolUltraFine->DrawTextA(0, Int_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		rect.top += 10;
		ltoa(UtilityHolder->uses,Int_c,10);
		m_pD3DFontCoolUltraFine->DrawTextA(0, Int_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		rect.top += 10;
		ltoa(UtilityHolder->splashRange,Int_c,10);
		m_pD3DFontCoolUltraFine->DrawTextA(0, Int_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		break;
	case 5:
		UtilityHolder = (Utility*)Holder;
		rect.top = y+7;
		rect.left = x;
		m_pD3DFontCoolUltraFine->DrawTextA(0, "Goggles \nVision: ", -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		rect.left += 50;
		rect.top += 10;
		ltoa(UtilityHolder->vision,Int_c,10);
		m_pD3DFontCoolUltraFine->DrawTextA(0, Int_c, -1, &rect,
			DT_TOP | DT_LEFT | DT_NOCLIP, 
			UItext);
		break;
		}
	}
}

void CDirectXFramework::Pickup(int choice){
	void* Holder;
	for(int i = 0; i < NumLoot;i++){
		if(LootArray[i]->PosLoc == Character.pos){
			Holder = Character.inventory[choice];
			Character.inventory[choice] = LootArray[i]->m_Item;
			LootArray[i]->m_Item = Holder;
		}
	}
}

void CDirectXFramework::Grenade(int x,int y){
	queue<int,deque<int>> Vision;
	int Held;
	World.GetTile(x,y).setVisited(true);
	if(Character.utilityItem->uses > 0)
	{
		for(int i = 0; i < NumEnt; i++){
			if(EntArray[i]->PosLoc == V2D(x,y))
			{
				EntArray[i]->HitPoints -= Character.utilityItem->damageOnHit;
				if(EntArray[i]->HitPoints < 1){
					World.GetTile(EntArray[i]->PosLoc.x,EntArray[i]->PosLoc.y).Vacant = true;
					EntArray[i]->dead = true;
					Score += Level*1.25f;
					int counter = 0;
					for(int i = 0; i < NumEnt; i++)
					{
						if(EntArray[i]->dead == true)
							counter = i;					
					}
					for(int i = counter; i < NumEnt-1; i++)
					{
						EntArray[i] = EntArray[i+1];
					}
					NumEnt--;
				}
			}
		}
		for(int i = 0; i < 4; i++){
			World.GetTile(World.GetTile(x,y).Neighbors[i]).setDistance(Character.utilityItem->splashRange);
			Held = World.GetTile(x,y).Neighbors[i];
			Vision.push(Held);
		}
		while(!Vision.empty()){
			World.GetTile(Vision.front()).setVisited(true);
			for(int i = 0; i < NumEnt; i++){
				if(EntArray[i]->PosLoc == World.GetTile(Vision.front()).Loc){
					EntArray[i]->HitPoints -= Character.utilityItem->damageOnHit;
					if(EntArray[i]->HitPoints < 1){
						World.GetTile(EntArray[i]->PosLoc.x,EntArray[i]->PosLoc.y).Vacant = true;
						EntArray[i]->dead = true;
						Score += Level*1.25f;
						int counter = 0;
						for(int i = 0; i < NumEnt; i++){
							if(EntArray[i]->dead == true)
								counter = i;					
						}
						for(int i = counter; i < NumEnt-1; i++){
							EntArray[i] = EntArray[i+1];
						}
						NumEnt--;
					}
				}
			}
			if(World.GetTile(Vision.front()).isTraversable() && World.GetTile(Vision.front()).getDistance() != 0 && World.GetTile(Vision.front()).Vacant){
				for(int i = 0; i < 4; i++){
					if(!World.GetTile(World.GetTile(Vision.front()).Neighbors[i]).getVisited()){
						World.GetTile(World.GetTile(Vision.front()).Neighbors[i]).setDistance(World.GetTile(Vision.front()).getDistance()-1);
						Held = World.GetTile(Vision.front()).Neighbors[i];
						Vision.push(Held);
					}
				}
			}
			Vision.pop();
		}
		World.Reset();
		Character.utilityItem->uses--;
		CalcVision();
		system->playSound(FMOD_CHANNEL_FREE, Sounds[3], false, &SChannel[0]);
	}
}