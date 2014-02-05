#include "Player.h"

Player::Player() 
	:maxInventorySize(5)
{
	dexterity = 3;
	intellegence = 3;
	speed = 3;
	strength = 3;
	skillPoints = 8;
	AccumulatedSpeed = 0;
}

Player::~Player()
{
	if(texture)
	{
	texture->Release();
	texture = 0;
	}
}

void Player::Init(LPDIRECT3DDEVICE9 device)
{
	setTexture(device);
}

//void Player::Update()
//{
//
//}

void Player::MeleeAttack()
{
	meleeWeapon->Special();
}

void Player::RangedAttack()
{
	rangedWeapon->Special();
}

void Player::Use()
{
	utilityItem->Special();
}

void Player::setTexture(LPDIRECT3DDEVICE9 device)
{
	D3DXCreateTextureFromFileEx(device, L"Player.jpg", 0, 0, 0, 0,
	D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
	D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255),
	&m_imageInfo, 0, &texture);
}

void Player::Draw(ID3DXSprite* m_pD3DSprite,D3DXIMAGE_INFO m_imageInfo, V2D Draw)
{
	D3DXMatrixTranslation(&m_MatrixTran2,(pos.x*(64*scaleFactor)-(Draw.x*64)) ,(pos.y*(64*scaleFactor)-(Draw.y*64)) ,0);
	D3DXMatrixRotationZ(&m_MatrixRot, rotationFactor);
	D3DXMatrixScaling(&m_MatrixScale, scaleFactor, scaleFactor, 0);
	m_Matrix = (m_MatrixScale*m_MatrixRot*m_MatrixTran2);

	m_pD3DSprite->SetTransform(&m_Matrix);

	m_pD3DSprite->Draw(texture, 0,   &D3DXVECTOR3(/*(m_imageInfo.Width * 0.5f)*/0.0f, /*(m_imageInfo.Height * 0.5f)*/0.0f, 0.0f), 
			&D3DXVECTOR3(0,0, 0.0f),
			D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMATRIX temp;
	D3DXMatrixIdentity(&temp);
	m_pD3DSprite->SetTransform(&temp);
}