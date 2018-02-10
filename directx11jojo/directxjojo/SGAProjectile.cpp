#include "stdafx.h"
#include "SGAProjectile.h"


SGAProjectile::SGAProjectile() : SGAActor()
{
}

SGAProjectile::SGAProjectile(SpriteBatch * pBatch, SGASpriteSheet * pSheet, SpriteFont * pFont)
	: SGAActor(pBatch, pSheet,pFont)
{
}


SGAProjectile::~SGAProjectile()
{
}

void SGAProjectile::Init(XMFLOAT2 position, XMFLOAT2 direction, float speed, float lifeTime, float distance)
{
	if (lifeTime == 0.0f && distance == 0.0f)
		DXTRACE_ERR_MSGBOX(L"Projectile's lifetime and maxdistance should ",0);

	this->mPosition = position;
	this->mvDirection = direction;
	this->mfSpeed = speed;
	this->mfLifeTIme = lifeTime;
	this->mfMaxDistance = distance;

	this->mfElapsedTime = 0.0f;
	this->mfDistance = 0.0f;

}

E_SCENE SGAProjectile::Update(float dt)
{

	SGAActor::Update(dt);

	float moveScalar = mfSpeed * dt;
	mPosition = mPosition + mvDirection * moveScalar;

	mfElapsedTime += dt;
	mfDistance += moveScalar;

	if ((mfLifeTIme != 0.0f && mfElapsedTime > mfLifeTIme) ||
		(mfMaxDistance != 0.0f && mfDistance > mfMaxDistance))
	{
		this->SetDestroyed();
	}

	return E_SCENE_NONPASS;
}
